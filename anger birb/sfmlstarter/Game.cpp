// Author: Dr Noel O'Hara
// SFML Starter for an awesome game you can make!
#ifdef _DEBUG 
#pragma comment(lib,"sfml-graphics-d.lib") 
#pragma comment(lib,"sfml-audio-d.lib") 
#pragma comment(lib,"sfml-system-d.lib") 
#pragma comment(lib,"sfml-window-d.lib") 
#pragma comment(lib,"sfml-network-d.lib") 
#else 
#pragma comment(lib,"sfml-graphics.lib") 
#pragma comment(lib,"sfml-audio.lib") 
#pragma comment(lib,"sfml-system.lib") 
#pragma comment(lib,"sfml-window.lib") 
#pragma comment(lib,"sfml-network.lib") 
#endif 

#include <SFML/Graphics.hpp>
// I use a couple of h files from thor library.
//https://github.com/Bromeon/Thor
#include "VectorAlgebra2D.h"


#include <iostream>
#include <stdlib.h> 
#include <time.h> 
#include <list>
enum PlayerState { ready, drawingLine, Jump };
class Game
{
public:
	//create Window
	sf::RenderWindow window;
	sf::View view;
	float randomNum = 0;

	sf::CircleShape playerShape;
	sf::Vector2f moveForce;
	PlayerState playerState = ready;
	float gravityScale = 20;
	sf::Vector2f velocity = { 0,0 };
	float playerMaxSpeed = 400;
	sf::Vector2f gravity{ 0.0f, 9.8f * gravityScale };
	
	sf::Vector2f normalised ;
	
	sf::RectangleShape slingshot;
	sf::RectangleShape ground;
	sf::RectangleShape target;

	bool readyToFire;

	sf::Vector2f lineLength;

	int pointA;
	int pointB;
	int distance;

	Game()
	{
		window.create(sf::VideoMode(1000, 600), "Untitled Physics Games");
	}
	
	void init()
	{

		view = window.getDefaultView();
		playerShape.setRadius(20);
		playerShape.setPosition(160, 500);
		playerShape.setOrigin(sf::Vector2f(20, 20));
		sf::Vector2f normalised = thor::unitVector(playerShape.getPosition());
		float l = thor::length(gravity);
		float r = thor::toDegree(10.0);
		
		thor::setLength(normalised, 100.0f);
		thor::rotate(normalised, 10.0f);

		slingshot.setPosition(180, 500);
		slingshot.setSize(sf::Vector2f(10, 90));
		slingshot.setFillColor(sf::Color::Yellow);

		playerShape.setPosition(185, 400);

		ground.setSize(sf::Vector2f(1000, 10));
		ground.setFillColor(sf::Color::Green);
		ground.setPosition(0, 590);

		readyToFire = false;

		target.setFillColor(sf::Color::Red);
		target.setPosition(rand() % 700 + 300, 580);
		target.setSize(sf::Vector2f(10,10));
	}
	void run()
	{
	
		sf::Time timePerFrame = sf::seconds(1.0f / 60.0f);


		sf::Time timeSinceLastUpdate = sf::Time::Zero;

		
		sf::Clock clock;

		clock.restart();

		while (window.isOpen())
		{
			
			sf::Event event;
			while (window.pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
					window.close();
			}

			
			timeSinceLastUpdate += clock.restart();

			

			if (timeSinceLastUpdate > timePerFrame)
			{

				if (playerState == ready)
				{
					if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
					{
						playerState = drawingLine;
					}
				}
				if (playerState == drawingLine)
				{
					if (!sf::Mouse::isButtonPressed(sf::Mouse::Left))
					{
						//pointA = thor::length(playerShape.getPosition());
						//pointB = thor::length(sf::Mouse::getPosition());
						//distance = pointA - pointB;

						sf::Vector2f localPosition = (sf::Vector2f)sf::Mouse::getPosition(window);
						sf::Vector2f newVelocity;
						newVelocity = localPosition - playerShape.getPosition();
						distance = thor::length(playerShape.getPosition()) - thor::length(localPosition);
						//thor::
						newVelocity.x = distance;
						if (thor::length(moveForce) < playerMaxSpeed)
						{
							velocity = -newVelocity;
						}
						else {
							thor::setLength(moveForce, playerMaxSpeed);
							velocity = -newVelocity;
						}

						playerState = Jump;
						gravity.y = 9.8 * gravityScale;
						readyToFire = false;
					}
				}
				if (playerState == Jump)
				{
					playerState = ready;
				}

				if (playerShape.getGlobalBounds().intersects(target.getGlobalBounds()))
				{
					target.setPosition(rand() % 700 + 300, 580);
					playerShape.setPosition(185, 400);

				}

				
				if (readyToFire == false)
				{
					velocity = velocity + (gravity * timeSinceLastUpdate.asSeconds());

					playerShape.move(velocity.x * timeSinceLastUpdate.asSeconds(), velocity.y * timeSinceLastUpdate.asSeconds());
				}


				if (playerShape.getPosition().y > 600 - playerShape.getRadius() -10)
				{
					velocity.y = -velocity.y;
				}
				if (playerShape.getPosition().x > 1000 - playerShape.getRadius())
				{
					velocity.x = -velocity.x;
				}

				if (playerShape.getGlobalBounds().intersects(slingshot.getGlobalBounds()) && readyToFire == false)
				{
					readyToFire = true;
					playerShape.setPosition(185, 480);
				}

				window.clear();
				if (playerState == drawingLine)
				{
					sf::Vector2i localPosition = sf::Mouse::getPosition(window);
					sf::Vertex line[] =
					{
						sf::Vertex(sf::Vector2f(localPosition.x, localPosition.y)),
						sf::Vertex(sf::Vector2f(playerShape.getPosition().x, playerShape.getPosition().y))
					};

					

					window.draw(line, 2, sf::Lines);
				}
				window.draw(ground);
				window.draw(target);
				window.draw(slingshot);

				window.draw(playerShape);
				
				window.display();

				
				timeSinceLastUpdate = sf::Time::Zero;
			}
		}
	}
};


int main()
{


	Game game;
	

	game.init();

	game.run();



	return 0;
}

