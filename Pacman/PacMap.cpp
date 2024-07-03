#include <SFML/Graphics.hpp>
#include "PacMap.h"
#include <math.h>

PacMap::PacMap() {
	texture.loadFromFile("C:/Users/bruno/OneDrive/Documents/Projekti/Pac-Man-Sprite.png", sf::IntRect(228, 0, 224, 248));
	pacMap.setTexture(texture);
	pacMap.scale(sf::Vector2f(3.0, 3.0));
}

double PacMap::CalculateDistance(int x1, int y1, int x2, int y2) {
	double dx = x2 - x1;
	double dy = y2 - y1;
	return sqrt(dx * dx + dy * dy);
}

bool PacMap::CheckIfInBox(int& i, int& j, sf::Vector2f position) {
	float x = (position.y - 12.f) / BLOCK_SIZE;
	float y = (position.x - 12.f) / BLOCK_SIZE;

	i = static_cast<int>(x);
	j = static_cast<int>(y);

	return (x - i == 0 && y - j == 0);
}

bool PacMap::CheckIfInTunnel(sf::Vector2f position) {
	return position.x <= 12.f || position.x >= 660.f;
}

bool PacMap::CheckIfInGhostBox(sf::Vector2f position) {
	return position.y <= 14.5 * BLOCK_SIZE + 12.f && position.y >= 13.5 * BLOCK_SIZE + 12.f && position.x <= 15.5 * BLOCK_SIZE + 12.f && position.x >= 11.5 * BLOCK_SIZE + 12.f;
}

bool PacMap::CheckIfColision(sf::Vector2f position, Direction direction) {
	int i, j;

	if (!CheckIfInBox(i, j, position) || CheckIfInTunnel(position)) return false;
	else {
		std::pair<int, int> cordinatesToCheck = wallCheck.at(direction);
		if (map[i + cordinatesToCheck.first][j + cordinatesToCheck.second] != '#' && map[i + cordinatesToCheck.first][j + cordinatesToCheck.second] != 'D') return false;
		return true;
	}
}

bool PacMap::CanChangeDirection(sf::Vector2f position, Direction currentDirection, Direction checkDirection) {
	int i, j;

	if (!CheckIfInBox(i, j, position) || CheckIfInTunnel(position)) {
		if ((currentDirection == Direction::Up || currentDirection == Direction::Down) && (checkDirection == Direction::Up || checkDirection == Direction::Down)) return true;
		else if ((currentDirection == Direction::Left || currentDirection == Direction::Right) && (checkDirection == Direction::Left || checkDirection == Direction::Right)) return true;
		return false;
	}
	else {
		switch (checkDirection) {
			case Direction::Up:
				if (map[i - 1][j] != '#' && map[i - 1][j] != 'D') return true;
				return false;
			case Direction::Down:
				if (map[i + 1][j] != '#' && map[i + 1][j] != 'D') return true;
				return false;
			case Direction::Right:
				if (map[i][j + 1] != '#' && map[i][j + 1] != 'D') return true;
				return false;
			case Direction::Left:
				if (map[i][j - 1] != '#' && map[i][j - 1] != 'D') return true;
				return false;
			}
	}
}

std::pair<int, int> PacMap::positionCordinates(sf::Vector2f position) {
	std::pair<int, int> cordinates;
	cordinates.first = lround((position.y - 12.f) / BLOCK_SIZE);
	cordinates.second = lround((position.x - 12.f) / BLOCK_SIZE);

	return cordinates;
}

void PacMap::DrawFood(sf::RenderWindow& p, std::string gameState[31]) {
	sf::CircleShape food;
	food.setRadius(FOOD_SIZE);
	food.setOrigin(food.getRadius(), food.getRadius());
	food.setFillColor(sf::Color::Yellow);

	for (int i = 0; i < 31; i++)
	{
		for (int j = 0; j < 28; j++)
		{
			if (gameState[i][j] == '.')
			{
				food.setPosition(j * BLOCK_SIZE + 12.f, i * BLOCK_SIZE + 12.f);
				p.draw(food);
			}
		}
	}
}

void PacMap::DrawPacMap(sf::RenderWindow& p, std::string gameState[31]) {
	p.draw(pacMap);
	DrawFood(p, gameState);

	return;
}