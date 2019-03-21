#include <SFML\Graphics.hpp>
#include <cstdlib>
#define NUM 1000

class Ball
{
public:
	Ball()
	{
		radius = 1;
		mass = 1;
		startPos.x = 100;
		startPos.y = 100;
		velocity.x = 0;
		velocity.y = 0;
		acceleration.x = 0;
		acceleration.y = 0;
		position = startPos;
	}

	void UpdatePosition(float dt);
	sf::Vector2f GetImpulse() const;
	sf::Time startTime;
	sf::Vector2f startPos;
	sf::Vector2f position;
	sf::Vector2f velocity;
	sf::Vector2f acceleration;
	float mass;
	float radius;
	void UpdatePosition(sf::Time time)
	{
		velocity += acceleration * (time.asSeconds() - startTime.asSeconds());
		position = startPos + velocity * (time.asSeconds() - startTime.asSeconds());
	}
	sf::Vector2f GetImpulse()
	{
		return velocity * mass;
	}
};

struct World
{
	std::vector<Ball> balls;
};

void draw(sf::RenderWindow& window, const World& world)
{
	for (const auto& ball : world.balls)
	{
		sf::CircleShape circle(ball.radius);
		circle.setFillColor(sf::Color::Yellow);
		circle.setPosition(ball.position);
		window.draw(circle);
	}
}

float len (sf::Vector2f &a)
{
	return(sqrt(a.x * a.x + a.y * a.y));
}

float operator* (sf::Vector2f &V1, sf::Vector2f &V2)
{
	return(V1.x * V2.x + V1.y * V2.y);
}

sf::Vector2f operator* (sf::Vector2f &V1, float& a)
{
	return sf::Vector2f(V1.x * a, V1.y * a);
}

sf::Vector2f norm(sf::Vector2f &a)
{
	return(a / len(a));
}

void newMovement(sf::Time& time, World& world, int& i)
{
	world.balls[i].startPos = world.balls[i].position;
	world.balls[i].startTime = time;
}

void update(sf::Time& time, World& world)
{
	for (int j = 0; j < NUM; ++j)
	{
		if (((world.balls[j].position.x <= 0) && (world.balls[j].velocity.x < 0)) ||
			((world.balls[j].position.x >= 800 - 2 * world.balls[j].radius) && (world.balls[j].velocity.x > 0)))
		{
			world.balls[j].velocity.x *= -1;
			newMovement(time, world, j);
		}
		if (((world.balls[j].position.y <= 0) && (world.balls[j].velocity.y < 0)) ||
			((world.balls[j].position.y >= 600 - 2 * world.balls[j].radius) && (world.balls[j].velocity.y > 0)))
		{
			world.balls[j].velocity.y *= -1;
			newMovement(time, world, j);
		}
		for (int i = 0; i < j; ++i) 
		{
			sf::Vector2f a = world.balls[j].position - world.balls[i].position;
			if ((len(a) < world.balls[j].radius + world.balls[i].radius) && 
				((world.balls[j].velocity - world.balls[i].velocity) * a < 0))
			{
				sf::Vector2f dP = norm(a) * (2 * ((world.balls[i].velocity - world.balls[j].velocity) * (norm(a))) /
					(1 / world.balls[i].mass + 1 / world.balls[j].mass));
				newMovement(time, world, j);
				newMovement(time, world, i);
				world.balls[j].velocity += dP / world.balls[j].mass;
				world.balls[i].velocity -= dP / world.balls[i].mass;
			}
		}
		world.balls[j].UpdatePosition(time);
	}
	

}


int main()
{
	sf::RenderWindow window(sf::VideoMode(800, 600), "My window");
	World world;
	for (int i = 0; i < NUM; ++i)
	{
		Ball a;
		a.radius = 1 + float(rand() % 4);
		a.mass = a.radius * a.radius;
		a.startPos.x = 1 + float(rand() % 798);
		a.startPos.y = 1 + float(rand() % 598);
		a.velocity.x = -30 + float(rand() % 60);
		a.velocity.y = -30 + float(rand() % 60);
		a.acceleration.x = 0;
		a.acceleration.y = 0;
		a.position = a.startPos;
		world.balls.push_back(a);
	}
	sf::Clock clock;
	while (window.isOpen())
	{
		sf::Time time = clock.getElapsedTime();
		window.clear(sf::Color::Black);
		update(time, world);
		draw(window, world);
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}
		window.display();
	}

	return 0;
}
