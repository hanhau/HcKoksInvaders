#include "include/GameWorld.hpp"
#include <glm/gtc/noise.hpp>
#include "include/AStar.hpp"
#include "include/EmptyTile.hpp"
#include "include/EnemySpaceShipTile.hpp"
#include "include/EnemyTurretTile.hpp"
#include "include/InstanceBuffer.hpp"
#include "include/StarShip.hpp"
#include <thread>
#include <iostream>

const int GameWorld::MinHeightInTiles = 15;
const int GameWorld::WidthInTiles = 8;

enum class TileType {
	Empty,
	Turret,
	EnemySpaceShip
};

GameWorld::GameWorld(Game& game_ref) :
	m_gameRef(game_ref),	
	m_instEnemyShip(nullptr),
	m_instTurretBase(nullptr),
	m_instTurretHead(nullptr),
	m_ndcHeight(0.0f)
{}

enum class MoveDirection {
	Left = 0, Down = 1, Right = 2
};

void GameWorld::init(int stageheight, int seed) {
	// clear everything
	m_tiles.clear();
	m_enemySpaceshipTilesPtrs.clear();
	m_enemyTurretTilesPtrs.clear();
	delete m_instTurretHead;
	delete m_instTurretBase;
	delete m_instEnemyShip;

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
		m_tiles[r][pos_x] = (TileEntityBase*)new EmptyTile(&m_gameRef);

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
		float px = -1.0;
		for (int x = 0; x < WidthInTiles; x++)
		{
			if (m_tiles[y][x] == nullptr) {
				int type = rand() % 10;
				switch (type) {
				case 2: case 3: case 4: case 5:
				case 6: case 7: case 8: case 9:
					m_tiles[y][x] = (TileEntityBase*)new EmptyTile(&m_gameRef);
					break;
				case 0: 
					m_tiles[y][x] = (TileEntityBase*)new EnemySpaceShipTile(
						&m_gameRef,
						rand(),
						glm::vec3(px,py,0.0f),
						0.025f
					);
					break;
				case 1: 
					m_tiles[y][x] = (TileEntityBase*)new EnemyTurretTile(
						&m_gameRef,
						rand(),
						glm::vec3(px,py,0.0f),
						0.075f
					);
					break;
				}
			}
			px += dist;
		}
		py += dist;
	}

	m_ndcHeight = py + dist;

	// Set Vectors
	m_enemySpaceshipTilesPtrs.clear();
	m_enemyTurretTilesPtrs.clear();

	for(const auto& row : m_tiles)
		for (const auto& column : row) {
			TileEntityBase* base = column;

			auto spaceShipTile = dynamic_cast<EnemySpaceShipTile*>(base);
			auto enemyTurretTile = dynamic_cast<EnemyTurretTile*>(base);

			if (spaceShipTile != nullptr)
				m_enemySpaceshipTilesPtrs.push_back(spaceShipTile);
			else if (enemyTurretTile != nullptr)
				m_enemyTurretTilesPtrs.push_back(enemyTurretTile);
		}

	// Init InstanceBuffers with correct capcity
	m_instTurretBase = new InstanceBuffer(m_enemyTurretTilesPtrs.size());
	m_instTurretHead = new InstanceBuffer(m_enemyTurretTilesPtrs.size());
	m_instEnemyShip = new InstanceBuffer(m_enemySpaceshipTilesPtrs.size());
}

void GameWorld::update(const double deltaTime) {
	for (auto& iter_row : m_tiles) {
		for (auto& iter_column : iter_row) {
			iter_column->update(deltaTime);
		}
	}
}

void GameWorld::updateOnBulletCollisions(std::list<Bullet>& bullets,
										 glm::vec3 camPos,
										 float visibleOffsetYPositive,
										 float visibleOffsetYNegative,
										 int& points) 
{
	static const auto& mm = m_gameRef.getModelManager();
	static const Model3D& modelTurret = mm.getModel("res/models/turret_head.obj");
	static const Model3D& modelEnemySpaceShip = mm.getModel("res/models/ship1.obj");

	for (auto& iterTurret: m_enemyTurretTilesPtrs) {
		if (iterTurret == nullptr)
			continue;

		if (iterTurret->getPos().y < camPos.y + visibleOffsetYPositive &&
			iterTurret->getPos().y > camPos.y - abs(visibleOffsetYNegative)) 
		{
			std::list<Bullet>::iterator iterBullet = bullets.begin();

			while (iterBullet != bullets.end()) {
				if (iterTurret == nullptr)
					break;

				if (iterBullet->m_owner == Bullet::Owner::Enemy)
					continue;

				auto bb = modelTurret.getOuterBB().transform(iterTurret->getHeadPos());

				if (bb.intersects(iterBullet->m_pos)) {
					std::cout << 
						"HIT_TR: B " << 
						iterBullet->m_pos.x << "/" << iterBullet->m_pos.y << "/" << iterBullet->m_pos.z <<
						" @ " << bb.getPos().x << "/" << bb.getPos().y << " R:" << bb.getRadius() << "\n";

					iterTurret->takeDamage(iterBullet->m_damage);
					if (iterTurret->getHealth() <= 0.0f) {
						iterTurret = nullptr;
						points += 25;
					}

					iterBullet = bullets.erase(iterBullet);
				}
				else
					++iterBullet;
			}
		}
	}
	for (auto& iterSpaceship : m_enemySpaceshipTilesPtrs) {
		if (iterSpaceship == nullptr)
			continue;

		if (iterSpaceship->getPos().y < camPos.y + visibleOffsetYPositive &&
			iterSpaceship->getPos().y > camPos.y - abs(visibleOffsetYNegative))
		{
			std::list<Bullet>::iterator iterBullet = bullets.begin();

			while (iterBullet != bullets.end()) {
				if (iterSpaceship == nullptr)
					break;

				if (iterBullet->m_owner == Bullet::Owner::Enemy)
					continue;

				auto bb = modelEnemySpaceShip.getOuterBB().transform(iterSpaceship->getSpaceshipPos());

				if (bb.intersects(iterBullet->m_pos)) {
					std::cout <<
						"HIT_SS: B " <<
						iterBullet->m_pos.x << "/" << iterBullet->m_pos.y << "/" << iterBullet->m_pos.z <<
						" @ " << bb.getPos().x << "/" << bb.getPos().y << " R:" << bb.getRadius() << "\n";

					iterSpaceship->takeDamage(iterBullet->m_damage);
					if (iterSpaceship->getHealth() <= 0.0f) {
						iterSpaceship = nullptr;
						points += 25;
					}

					iterBullet = bullets.erase(iterBullet);
				}
				else
					++iterBullet;
			}
		}
	}
}

void GameWorld::draw(const Camera& camera, Cubemap& cubemap) {
	static const ModelManager& mm = m_gameRef.getModelManager();
	static const Model3D& enemyShip = mm.getModel("res/models/ship1.obj");
	static const Model3D& turretHead = mm.getModel("res/models/turret_head.obj");
	static const Model3D& turretBase = mm.getModel("res/models/turret_base.obj");
	static const Model3D& playerShip = mm.getModel("res/models/vengabus.obj");

	static constexpr float aabb_relMiny = -1.0;
	static constexpr float aabb_relMaxy = 6.0;

	static sf::Clock clock;

	const float secs = clock.getElapsedTime().asSeconds();
	const glm::vec3 spaceShipPos = m_gameRef.getStarShip()->getPos();
	const glm::vec3 camPos = camera.getCameraPos();

	int drawnTurrets = 0;
	for (const auto& iter: m_enemyTurretTilesPtrs) {
		if (iter == nullptr)
			continue;

		const float dy = spaceShipPos.y - iter->getPos().y;
		const float dx = spaceShipPos.x - iter->getPos().x;

		bool visible = iter->getPos().y < camPos.y + aabb_relMaxy &&
					   iter->getPos().y > camPos.y - abs(aabb_relMiny);

		if (visible) 
		{
			const float ratio = dx / abs(dy);
			const float angle = glm::degrees(atanf(ratio));

			iter->getHeadPos().setRotationMatrix(
				glm::rotate(
					glm::rotate(
						glm::identity<glm::mat4>(),
						glm::radians(90.f), glm::vec3(1.f, 0.f, 0.f)
					),
					glm::radians(angle), glm::vec3(.0f, 1.f, .0f)
				)
			);

			(*m_instTurretHead)[drawnTurrets] = iter->getHeadPos();
			(*m_instTurretBase)[drawnTurrets] = iter->getBasePos();

			drawnTurrets++;
		}
	}
	
	m_instTurretHead->setInnerCount(drawnTurrets);
	m_instTurretHead->transferToGpu();
	
	m_instTurretBase->setInnerCount(drawnTurrets);
	m_instTurretBase->transferToGpu();

	int drawnSpaceShips = 0;
	for (const auto& iter: m_enemySpaceshipTilesPtrs) {
		if (iter == nullptr)
			continue;

		const float dy = spaceShipPos.y - iter->getPos().y;

		bool visible = iter->getPos().y < camPos.y + aabb_relMaxy &&
					   iter->getPos().y > camPos.y - abs(aabb_relMiny);

		if (visible)
		{
			(*m_instEnemyShip)[drawnSpaceShips] = iter->getSpaceshipPos();
			
			drawnSpaceShips++;
		}
	}
	
	m_instEnemyShip->setInnerCount(drawnSpaceShips);
	m_instEnemyShip->transferToGpu();

	turretBase.drawInstanceQueue(*m_instTurretBase, camera,cubemap);
	turretHead.drawInstanceQueue(*m_instTurretHead, camera, cubemap);
	enemyShip.drawInstanceQueue(*m_instEnemyShip, camera, cubemap);
}

const float GameWorld::getNDCHeight() {
	return m_ndcHeight;
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