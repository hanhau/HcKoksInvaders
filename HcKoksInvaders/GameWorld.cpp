#include "include/GameWorld.hpp"
#include <glm/gtc/noise.hpp>
#include "include/AStar.hpp"
#include "include/EmptyTile.hpp"
#include "include/EnemySpaceShipTile.hpp"
#include "include/EnemyTurretTile.hpp"
#include <thread>
#include <iostream>

const int GameWorld::MinHeightInTiles = 15;
const int GameWorld::WidthInTiles = 10;

enum TileType {
	Empty,
	Turret,
	EnemySpaceShip
};

GameWorld::GameWorld(Game * const game_ref) :
	m_gameRef(game_ref)
{}

enum MoveDirection {
	Left, Down, Right
};

void GameWorld::init(int stageheight, int seed) {
	// Create empty Map
	for (int y = 0; y < stageheight; y++) {
		std::vector<TileEntityBase*> row;
		
		for (int x = 0; x < WidthInTiles; x++)
			row.push_back(nullptr);

		m_tiles.push_back(row);
	}

	// Create random path
	srand(seed);
	int pos_x = rand() % WidthInTiles;
	int end_y = stageheight - MinHeightInTiles;

	for (int r = 0; r < end_y;) {
		// Create empty cell
		m_tiles[r][pos_x] = (TileEntityBase*)new EmptyTile(m_gameRef);

		// move to next
		std::vector<MoveDirection> possibleMoves;
		possibleMoves.clear();

		if (pos_x - 1 >= 0)
			possibleMoves.push_back(MoveDirection::Left);

		if (pos_x + 1 < WidthInTiles)
			possibleMoves.push_back(MoveDirection::Right);

		possibleMoves.push_back(MoveDirection::Down);

		// make random move
		MoveDirection nextMove = possibleMoves[rand()%possibleMoves.size()];

		switch (nextMove) {
		case MoveDirection::Down:
			r++;
			break;
		case MoveDirection::Left:
			pos_x--;
			break;
		case MoveDirection::Right:
			pos_x++;
			break;
		}
	}
	
	// Fill rest
	for (int y = 0; y < stageheight; y++)
	{
		for (int x = 0; x < WidthInTiles; x++)
		{
			if (m_tiles[y][x] == nullptr) {
				int type = rand() % 7;
				switch (type) {
				case 0: case 1: case 2: case 3:
				case 4:
					m_tiles[y][x] = (TileEntityBase*)new EmptyTile(m_gameRef);
					break;
				case 5: 
					m_tiles[y][x] = (TileEntityBase*)new EnemySpaceShipTile(m_gameRef,seed);
					break;
				case 6: 
					m_tiles[y][x] = (TileEntityBase*)new EnemyTurretTile(m_gameRef);
					break;
				}
			}
		}
	}
}

void GameWorld::update(const double deltaTime) {
	for (auto& iter_row : m_tiles) {
		for (auto& iter_column : iter_row) {
			iter_column->update(deltaTime);
		}
	}
}

void GameWorld::draw(const Camera& camera) {
	static std::vector<ModelPosition> turretBasePos(150);
	static std::vector<ModelPosition> turretHeadPos(150);
	static std::vector<ModelPosition> enemyPos(150);

	static std::vector<TileEntityBase*> visibleTiles(300);

	std::vector<std::thread> t;// (WidthInTiles);
	const int yies = m_tiles.size();

	
}

void GameWorld::saveToFileAsImage(std::string path) {
	sf::Image img;
	img.create(m_tiles[0].size(), m_tiles.size(), sf::Color::Black);

	for (size_t y = 0; y < m_tiles.size(); y++) {
		for (size_t x = 0; x < WidthInTiles; x++) {
			sf::Color col;
			
			TileEntityBase* base = m_tiles[y][x];

			if (EmptyTile * emptyTile = dynamic_cast<EmptyTile*>(base)) {

			}
			else if (EnemySpaceShipTile * spaceShipTile = dynamic_cast<EnemySpaceShipTile*>(base)) {

			}
			else if (EnemyTurretTile * enemyTurretTile = dynamic_cast<EnemyTurretTile*>(base)) {

			}
			else {
				col = sf::Color::Magenta;
			}

			if (0 == strcmp(typeid(*m_tiles[y][x]).name(),typeid(EmptyTile).name())) {
				col = sf::Color::Black;
			}
			else if (0 == strcmp(typeid(*m_tiles[y][x]).name(),typeid(EnemySpaceShipTile).name())) {
				col = sf::Color::Red;
			}
			else if (0 == strcmp(typeid(*m_tiles[y][x]).name(),typeid(EnemyTurretTile).name())) {
				col = sf::Color::Green;
			}
			else {
				col = sf::Color::Magenta;
			}

			img.setPixel(x, y, col);
		}
	}

	img.saveToFile(path);
}