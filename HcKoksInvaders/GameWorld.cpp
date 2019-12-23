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

GameWorld::GameWorld(
	Game * const game_ref,
	ModelManager * const modelMgrPtr
) :
	m_gameRef(game_ref),
	m_modelMgrRef(modelMgrPtr)
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
	
	// distance between each center of tile
	const float dist = 2.0f / (float)WidthInTiles;

	// Fill rest
	float py = 0.0f;
	for (int y = 0; y < stageheight; y++)
	{
		float px = -2.0;
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
					m_tiles[y][x] = (TileEntityBase*)new EnemySpaceShipTile(m_gameRef,rand());
					break;
				case 6: 
					m_tiles[y][x] = (TileEntityBase*)new EnemyTurretTile(
						m_gameRef,
						rand(),
						glm::vec3(px,py,0.0f),
						0.5f
					);
					break;
				}
			}
			px += dist;
		}
		py += dist;
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
	static std::vector<ModelPosition> turretBasePos(Model3D::MaxInstances);
	static std::vector<ModelPosition> turretHeadPos(Model3D::MaxInstances);
	static std::vector<ModelPosition> enemyPos(Model3D::MaxInstances);

	static std::vector<TileEntityBase*> visibleTiles(Model3D::MaxInstances * 3);

	static const Model3D& enemyShip = m_modelMgrRef->getModel("res/models/ship1.obj");
	static const Model3D& turretHead = m_modelMgrRef->getModel("res/models/turret_head.obj");
	static const Model3D& turretBase = m_modelMgrRef->getModel("res/models/turret_base.obj");
	static const Model3D& playerShip = m_modelMgrRef->getModel("res/models/vengabus.obj");

	for (size_t y = 0; y < m_tiles.size(); y++) {
		for (size_t x = 0; x < WidthInTiles; x++) {
			TileEntityBase* base = m_tiles[y][x];

			if (EnemySpaceShipTile * spaceShipTile = dynamic_cast<EnemySpaceShipTile*>(base)) {
				enemyShip.getOuterBB().transform(spaceShipTile->getSpaceshipPos()).isInViewport(camera);
			}
			else if (EnemyTurretTile * enemyTurretTile = dynamic_cast<EnemyTurretTile*>(base)) {
				
			}
		}
	}


}

void GameWorld::saveToFileAsImage(std::string path) {
	sf::Image img;
	img.create(m_tiles[0].size(), m_tiles.size(), sf::Color::Black);

	for (size_t y = 0; y < m_tiles.size(); y++) {
		for (size_t x = 0; x < WidthInTiles; x++) {
			sf::Color col;
			
			TileEntityBase* base = m_tiles[y][x];

			if (EmptyTile * emptyTile = dynamic_cast<EmptyTile*>(base)) {
				col = sf::Color::Black;
			}
			else if (EnemySpaceShipTile * spaceShipTile = dynamic_cast<EnemySpaceShipTile*>(base)) {
				col = sf::Color::Red;
			}
			else if (EnemyTurretTile * enemyTurretTile = dynamic_cast<EnemyTurretTile*>(base)) {
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