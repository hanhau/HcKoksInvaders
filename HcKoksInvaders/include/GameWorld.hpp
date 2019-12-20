#pragma once
#include "TileEntityBase.hpp"
#include <SFML/Graphics/RenderWindow.hpp>
#include "Game.hpp"
#include "ProgramManager.hpp"
#include "ModelManager.hpp"

class GameWorld {
	std::vector<std::vector<TileEntityBase*>> m_tiles;
	Game * const m_gameRef;	
	ModelManager* const m_modelMgrRef;

	bool isTileVisible(int x, int y, const Camera& cam);
public:
	GameWorld(Game * const game_ref,
			  ModelManager * const modelMgrPtr);

	static const int MinHeightInTiles;
	static const int WidthInTiles;

	void init(int stageheight,int seed);
	void update(const double deltaTime);
	void draw(const Camera& camera);

	void saveToFileAsImage(const std::string path);
};