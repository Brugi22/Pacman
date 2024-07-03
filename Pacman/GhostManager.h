#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include "PacMap.h"
#include <vector>

enum class mode{Scatter, Chase, Frightened};

class GhostManager {
	public:
		GhostManager();
		void DrawGhosts(sf::RenderWindow& p);
		void UpdateGhosts(sf::Vector2f pacManPosition, Direction pacManDirection, sf::Time time);
		void ReleaseGhosts(int foodCount);
		bool CheckColisionsWithPacman(sf::Vector2f pacManPosition);
		void RestartGhosts();

	private:
		void UpdatePositions(sf::Vector2f pacManPosition, Direction pacManDirection);
		std::vector<Direction> AllPossibleDirections(int x, int y, Direction currentDirection);
		Direction GhostDirectionLogic(int i, int x, int y, sf::Vector2f pacManPosition, Direction pacManDirection, std::vector<Direction> directionsToCheck);
		Direction LeaveUsingDoors(int i);
		void GhostLogic(sf::Vector2f pacManPosition, Direction pacManDirection);

		float speed;
		float distanceTraveled;
		int bodyAnimation;
		bool leaveHouse[4];
		sf::Vector2f position[4];
		Direction direction[4];
		mode ghostMode;
		sf::Clock modeChangeClock;
		sf::Clock clock;
		sf::Time time;
		sf::Texture texture;
		sf::Sprite ghosts[4];
};