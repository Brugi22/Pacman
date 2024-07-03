#include <iostream>
#include <map>
#include <SFML/Graphics.hpp>
#include "PacMap.h"
#include "Pacman.h"
#include "GhostManager.h"

Pacman::Pacman() : position(sf::Vector2f(13.5 * BLOCK_SIZE + 12, 23 * BLOCK_SIZE + 12)), speed(150.f), distanceTraveled(0), mouthAnimation(0), foodCount(244), direction(Direction::Left), numberOfLifes(3) {
	for (int i = 0; i < 31; ++i) gameState[i] = map[i];
	texture.loadFromFile("C:/Users/bruno/OneDrive/Documents/Projekti/Pac-Man-Sprite.png", sf::IntRect(456, 0, 224, 248));
	pacman.setTexture(texture);
	pacman.scale(sf::Vector2f(3.0, 3.0));
}

void Pacman::DrawPacman(sf::RenderWindow& p) {
	int left = 32 - 16 * mouthAnimation, top = 1 + 16 * static_cast<int> (direction);
	if (left == 32) top = 1;
	pacman.setTextureRect(sf::IntRect(left, top, 14, 14));
	pacman.setOrigin(sf::Vector2f(7.f, 7.f));
	pacman.setPosition(position);

	p.draw(pacman);
}

void Pacman::RestartIfGameOver() {
	if (!foodCount || ghostManager.CheckColisionsWithPacman(position)) {
		RestartPacman();
		ghostManager.RestartGhosts();
	}
}

bool Pacman::GameOver() {
	return !numberOfLifes;
}

void Pacman::ProcessInput() {
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && PacMap::CanChangeDirection(position, direction, Direction::Up)) direction = Direction::Up;
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && PacMap::CanChangeDirection(position, direction, Direction::Down)) direction = Direction::Down;
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && PacMap::CanChangeDirection(position, direction, Direction::Left)) direction = Direction::Left;
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && PacMap::CanChangeDirection(position, direction, Direction::Right)) direction = Direction::Right;
}

void Pacman::UpdatePosition() {
	distanceTraveled -= 6;
	mouthAnimation = (mouthAnimation + 1) % 3;
	position += positionUpdate.at(direction);
}

void Pacman::UpdatePacman() {
	if (position.x < -6.f) position.x = 678.f;
	if (position.x > 678.f) position.x = -6.f;

	distanceTraveled += time.asSeconds() * speed;

	switch (direction)
	{
		case Direction::Up:
			if (!PacMap::CheckIfColision(position, direction))
			{
				if (distanceTraveled < 6) break;
				else UpdatePosition();
			}
			break;
		case Direction::Down:
			if (!PacMap::CheckIfColision(position, direction))
			{
				if (distanceTraveled < 6) break;
				else UpdatePosition();
			}
			break;
		case Direction::Left:
			if (!PacMap::CheckIfColision(position, direction))
			{
				if (distanceTraveled < 6) break;
				else UpdatePosition();
			}
			break;
		case Direction::Right:
			if (!PacMap::CheckIfColision(position, direction))
			{
				if (distanceTraveled < 6) break;
				else UpdatePosition();
			}
	}

	EatFood();
}

void Pacman::RestartPacman() {
	position = sf::Vector2f(13.5 * BLOCK_SIZE + 12, 23 * BLOCK_SIZE + 12);
	speed = 100.f;
	distanceTraveled = 0;
	mouthAnimation = 0;
	foodCount = 244;
	direction = Direction::Left;
	for (int i = 0; i < 31; ++i) {
		gameState[i] = map[i];
	}
}

void Pacman::EatFood() {
	std::pair<int, int> cordinates = PacMap::positionCordinates(position);
	if (PacMap::CheckIfInTunnel(position)) return;

	if (gameState[cordinates.first][cordinates.second] == '.') {
		gameState[cordinates.first][cordinates.second] = ' ';
		foodCount--;
	}
}

void Pacman::UpdateGame(sf::RenderWindow& p) {
	time = clock.restart();
	RestartIfGameOver();
	UpdatePacman();
	ghostManager.ReleaseGhosts(foodCount);
	ghostManager.UpdateGhosts(position, direction, time);
	pacMap.DrawPacMap(p, gameState);
	DrawPacman(p);
	ghostManager.DrawGhosts(p);
}

void Pacman::Render(sf::RenderWindow& p) {
	UpdateGame(p);
	p.display();
}