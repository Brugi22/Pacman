#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include "PacMap.h"
#include "GhostManager.h"

GhostManager::GhostManager() : speed(150.f), distanceTraveled(0), bodyAnimation(0) {
	texture.loadFromFile("C:/Users/bruno/OneDrive/Documents/Projekti/Pac-Man-Sprite.png", sf::IntRect(456, 0, 224, 248));

	position[0] = sf::Vector2f(13.5 * BLOCK_SIZE + 12, 11 * BLOCK_SIZE + 12);
	direction[0] = Direction::Left;
	leaveHouse[0] = false;

	position[1] = sf::Vector2f(13.5 * BLOCK_SIZE + 12, 14.5 * BLOCK_SIZE + 12);
	direction[1] = Direction::Up;
	leaveHouse[1] = true;

	position[2] = sf::Vector2f(11.5 * BLOCK_SIZE + 12, 13.5 * BLOCK_SIZE + 12);
	direction[2] = Direction::Down;
	leaveHouse[2] = false;

	position[3] = sf::Vector2f(15.5 * BLOCK_SIZE + 12, 13.5 * BLOCK_SIZE + 12);
	direction[3] = Direction::Down;
	leaveHouse[3] = false;

	for (int i = 0; i < 4; i++) {
		ghosts[i].setTexture(texture);
		ghosts[i].scale(sf::Vector2f(3.0, 3.0));
	}
}

void GhostManager::DrawGhosts(sf::RenderWindow& p) {
	for (int i = 0; i < 4; i++) {
		ghosts[i].setTextureRect(sf::IntRect(static_cast<int>(direction[i])*32 + bodyAnimation*16, 64 + 16*i, 15, 15));
		ghosts[i].setOrigin(sf::Vector2f(7.5f, 7.5f));
		ghosts[i].setPosition(position[i]);

		p.draw(ghosts[i]);
	}
}

void GhostManager::UpdatePositions(sf::Vector2f pacManPosition, Direction pacManDirection) {
	GhostLogic(pacManPosition, pacManDirection);
	bodyAnimation = (bodyAnimation + 1) % 2;
	distanceTraveled -= 6;

	for (int i = 0; i < 4; i++) {
		position[i] += positionUpdate.at(direction[i]);
	}
}

std::vector<Direction> GhostManager::AllPossibleDirections(int i, int j, Direction currentDirection) {
	std::vector<Direction> allPossibleDirections;

	for (int k = 0; k < directionsToCordinates.size(); ++k) {
		int newRow = i + directionsToCordinates[k].first;
		int newCol = j + directionsToCordinates[k].second;

		if (map[newRow][newCol] != '#' && map[newRow][newCol] != 'D') {
			allPossibleDirections.push_back(directionsToEnum[k]);
		}
	}

	auto it = std::find(allPossibleDirections.begin(), allPossibleDirections.end(), reverseDirection.at(currentDirection));
	if (it != allPossibleDirections.end()) allPossibleDirections.erase(it);

	return allPossibleDirections;
}

Direction GhostManager::GhostDirectionLogic(int i, int x, int y, sf::Vector2f pacManPosition, Direction pacManDirection, std::vector<Direction> directionsToCheck) {
	std::pair<int, int> target;
	target = PacMap::positionCordinates(pacManPosition);
	std::pair<int, int> redGhostPosition = PacMap::positionCordinates(position[0]);
	std::pair<int, int> orangeGhostPosition = PacMap::positionCordinates(position[3]);

	switch (i) {
		case 0:
			break;
		case 1:
			target.first += 2 * wallCheck.at(pacManDirection).first;
			target.second += 2 * wallCheck.at(pacManDirection).second;
			target.first += abs(target.first - redGhostPosition.first);
			target.second += abs(target.second - redGhostPosition.second);
			break;
		case 2:
			target.first += 4 * wallCheck.at(pacManDirection).first;
			target.second += 4 * wallCheck.at(pacManDirection).second;
			break;
		case 3:
			if (PacMap::CalculateDistance(target.first, target.second, orangeGhostPosition.first, orangeGhostPosition.second) >= 8) break;
			target.first = 32;
			target.second = 0;
	}

	Direction targetDirection;
	double minDistance = 10000;

	std::map<Direction, int> priorityDirections = { {Direction::Up, 3}, {Direction::Left, 2}, {Direction::Down, 1}, {Direction::Right, 0} };
	for (Direction check : directionsToCheck) {
		std::pair<int, int> toCordinates = wallCheck.at(check);
		double calucalte = PacMap::CalculateDistance(x + toCordinates.first, y + toCordinates.second, target.first, target.second);

		if (minDistance > calucalte) {
			minDistance = calucalte;
			targetDirection = check;
		}
		else if (minDistance == calucalte) {
			if (priorityDirections.at(targetDirection) < priorityDirections.at(check)) targetDirection = check;
		}
	}

	return targetDirection;
}

Direction GhostManager::LeaveUsingDoors(int i) {
	if (position[i].x < 13.5 * BLOCK_SIZE + 12) return Direction::Right;
	if (position[i].x > 13.5 * BLOCK_SIZE + 12) return Direction::Left;
	if (position[i].y == 11 * BLOCK_SIZE + 12) {
		leaveHouse[i] = false;
		return Direction::Left;
	}
	return Direction::Up;
}

void GhostManager::GhostLogic(sf::Vector2f pacManPosition, Direction pacManDirection) {
	for (int i = 0; i < 4; i++) {
		int x, y;
		if (PacMap::CheckIfInTunnel(position[i])) continue;
		if (leaveHouse[i]) {
			direction[i] = LeaveUsingDoors(i);
			continue;
		}
		if (PacMap::CheckIfInGhostBox(position[i])) {
			if (position[i].y == 14.5 * BLOCK_SIZE + 12) direction[i] = Direction::Up;
			if (position[i].y == 13.5 * BLOCK_SIZE + 12) direction[i] = Direction::Down;
		}
		if (PacMap::CheckIfInBox(x, y, position[i])) {
			std::vector<Direction> allDirections = AllPossibleDirections(x, y, direction[i]);
			if (allDirections.size() == 1) direction[i] = allDirections[0];
			else direction[i] = GhostDirectionLogic(i, x, y, pacManPosition, pacManDirection, allDirections);
		}
	}
}

void GhostManager::UpdateGhosts(sf::Vector2f pacManPosition, Direction pacManDirection, sf::Time time) {
	distanceTraveled += time.asSeconds() * speed;

	for (int i = 0; i < 4; i++) {
		if (position[i].x < -6.f) position[i].x = 678.f;
		if (position[i].x > 678.f) position[i].x = -6.f;
	}

	if (distanceTraveled >= 6) UpdatePositions(pacManPosition, pacManDirection);
}

void GhostManager::ReleaseGhosts(int foodCount) {
	if (foodCount <= 214 && PacMap::CheckIfInGhostBox(position[2])) leaveHouse[2] = true;
	if (foodCount <= 162 && PacMap::CheckIfInGhostBox(position[3])) leaveHouse[3] = true;
}

bool GhostManager::CheckColisionsWithPacman(sf::Vector2f pacManPosition) {
	std::pair<int, int> pacManCord = PacMap::positionCordinates(pacManPosition), cord;
	for (int i = 0; i < 4; i++) {
		cord = PacMap::positionCordinates(position[i]);
		if(pacManCord == cord) return true;
	}
	return false;
}

void GhostManager::RestartGhosts() {
	distanceTraveled = 0;
	bodyAnimation = 0;

	position[0] = sf::Vector2f(13.5 * BLOCK_SIZE + 12, 11 * BLOCK_SIZE + 12);
	direction[0] = Direction::Left;
	leaveHouse[0] = false;

	position[1] = sf::Vector2f(13.5 * BLOCK_SIZE + 12, 14.5 * BLOCK_SIZE + 12);
	direction[1] = Direction::Up;
	leaveHouse[1] = true;

	position[2] = sf::Vector2f(11.5 * BLOCK_SIZE + 12, 13.5 * BLOCK_SIZE + 12);
	direction[2] = Direction::Down;
	leaveHouse[2] = false;

	position[3] = sf::Vector2f(15.5 * BLOCK_SIZE + 12, 13.5 * BLOCK_SIZE + 12);
	direction[3] = Direction::Down;
	leaveHouse[3] = false;
}
