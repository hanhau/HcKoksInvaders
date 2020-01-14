#pragma once
#include "Game.hpp"

struct InstanceBuffer;
struct TileEntityBase;
struct EnemyTurretTile;
struct EnemySpaceShipTile;

class GameWorld {
	Game& m_gameRef;	

	std::vector<std::vector<TileEntityBase*>> m_tiles;

	std::vector<EnemyTurretTile*> m_enemyTurretTilesPtrs;
	std::vector<EnemySpaceShipTile*> m_enemySpaceshipTilesPtrs;

	InstanceBuffer * m_instTurretHead;
	InstanceBuffer * m_instTurretBase;
	InstanceBuffer * m_instEnemyShip;

	float m_ndcHeight;
public:
	GameWorld(Game& game_ref);

	static const int MinHeightInTiles;
	static const int WidthInTiles;

	// Updated & drawn area on Y Axis
	static const float AABB_relMiny; // negative y offset
	static const float AABB_relMaxy; // positive y offset

	void init(int stageheight,int seed);
	void update(const double deltaTime);
	void updateOnBulletCollisions(std::list<Bullet>& bullets,
								  glm::vec3 camPos, 
								  float visibleOffsetYPositive,
								  float visibleOffsetYNegative,
								  int& points);

	void draw(const Camera& camera,Cubemap& cubemap);
	void letNPCsShoot(const Camera& camera);

	const float getNDCHeight();

	void saveToFileAsImage(const std::string path);
};