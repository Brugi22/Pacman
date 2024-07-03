#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include "PacMap.h"
#include "GhostManager.h"

class Pacman {
public:
	Pacman();
	void RestartIfGameOver();
	bool GameOver();
	void DrawPacman(sf::RenderWindow&);
	void EatFood();
	void ProcessInput();
	void UpdatePosition();
	void UpdatePacman();
	void RestartPacman();
	void UpdateGame(sf::RenderWindow& p);
	void Render(sf::RenderWindow&);
private:
	sf::Vector2f position;
	Direction direction;
	float speed;
	float distanceTraveled;
	int mouthAnimation;
	int foodCount;
	std::string gameState[31];
	sf::Clock clock;
	sf::Time time;
	sf::Texture texture;
	sf::Sprite pacman;
	int numberOfLifes;
	PacMap pacMap;
	GhostManager ghostManager;
};
