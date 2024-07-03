#pragma once

#include <SFML/Graphics.hpp>

#define BLOCK_SIZE 24
#define FOOD_SIZE 3

enum class Direction {Right, Left, Up, Down};

const std::map<Direction, std::pair<int, int>> wallCheck = { {Direction::Up, {-1, 0}}, {Direction::Down, {1, 0}}, {Direction::Left, {0, -1}}, {Direction::Right, {0, 1}} };
const std::map<Direction, sf::Vector2f> positionUpdate = { {Direction::Up, {0,-6}}, {Direction::Down, {0, 6}}, {Direction::Left, {-6, 0}}, {Direction::Right, {6, 0}} };
const std::vector<std::pair<int, int>> directionsToCordinates = { {1, 0}, {-1, 0}, {0, 1}, {0, -1} };
const std::vector<Direction> directionsToEnum = { Direction::Down, Direction::Up, Direction::Right, Direction::Left };
const std::map<Direction, Direction> reverseDirection = { {Direction::Up, Direction::Down}, {Direction::Down, Direction::Up}, {Direction::Left, Direction::Right}, {Direction::Right, Direction::Left} };
const std::string map[31] = {
		"############################",
		"#............##............#",
		"#.####.#####.##.#####.####.#",
		"#.####.#####.##.#####.####.#",
		"#.####.#####.##.#####.####.#",
		"#..........................#",
		"#.####.##.########.##.####.#",
		"#.####.##.########.##.####.#",
		"#......##....##....##......#",
		"######.##### ## #####.######",
		"     #.##### ## #####.#     ",
		"     #.##          ##.#     ",
		"     #.## ###DD### ##.#     ",
		"######.## #      # ##.######",
		"      .   #      #   .      ",
		"######.## #      # ##.######",
		"     #.## ######## ##.#     ",
		"     #.##          ##.#     ",
		"     #.## ######## ##.#     ",
		"######.## ######## ##.######",
		"#............##............#",
		"#.####.#####.##.#####.####.#",
		"#.####.#####.##.#####.####.#",
		"#...##.......  .......##...#",
		"###.##.##.########.##.##.###",
		"###.##.##.########.##.##.###",
		"#......##....##....##......#",
		"#.##########.##.##########.#",
		"#.##########.##.##########.#",
		"#..........................#",
		"############################"
};
const std::pair<int, int> yellowSquares[30] = { {12, 11}, {15,11}, {12, 23}, {15, 23} };

class PacMap {
	public:
		PacMap();
		static double CalculateDistance(int x1, int y1, int x2, int y2);
		static bool CheckIfInBox(int &i, int &j, sf::Vector2f position);
		static bool CheckIfInTunnel(sf::Vector2f position);
		static bool CheckIfInGhostBox(sf::Vector2f position);
		static bool CheckIfColision(sf::Vector2f position, Direction direction);
		static bool CanChangeDirection(sf::Vector2f position, Direction currentDirection, Direction checkDirection);
		static std::pair<int, int> positionCordinates(sf::Vector2f position);
		void DrawPacMap(sf::RenderWindow& p, std::string map[31]);
	private:
		void DrawFood(sf::RenderWindow& p, std::string map[31]);

		sf::Texture texture;
		sf::Sprite pacMap;
};
