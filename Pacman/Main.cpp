#include <iostream>
#include <string>
#include <SFML/Graphics.hpp>
#include "Pacman.h"

int main()
{
	Pacman pacman;
	sf::RenderWindow prozor(sf::VideoMode(224*3,248*3), "Pacman");
	prozor.setFramerateLimit(60);
	while (prozor.isOpen())
	{
		while (!pacman.GameOver())
		{
			sf::Event d;
			while (prozor.pollEvent(d))
			{
				if (d.type == sf::Event::Closed)
					prozor.close();
			}
			prozor.clear();
			pacman.ProcessInput();
			pacman.Render(prozor);
		}
	}
}