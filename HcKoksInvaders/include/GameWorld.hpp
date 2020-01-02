#pragma once
#include "TileEntityBase.hpp"
#include <SFML/Graphics/RenderWindow.hpp>
#include "Game.hpp"
#include "ProgramManager.hpp"
#include "ModelManager.hpp"
#include "Cubemap.hpp"
#include "EnemyTurretTile.hpp"
#include "EnemySpaceShipTile.hpp"
#include "InstanceBuffer.hpp"

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

	void init(int stageheight,int seed);
	void update(const double deltaTime);
	void draw(const Camera& camera,Cubemap& cubemap);

	const float getNDCHeight();

	void saveToFileAsImage(const std::string path);
};