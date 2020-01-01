#pragma once
#include <SFML/Graphics.hpp>
#include <glm/mat4x4.hpp>

#include "TextureManager.hpp"
#include "ProgramManager.hpp"
#include "ModelManager.hpp"
#include "SoundBufferManager.hpp"
#include "Camera.hpp"
#include "AmmunitionIcon.hpp"
#include "Button.hpp"

class TileEntityBase;

class Game {
private:
	friend class TileEntityBase;
	
	sf::Window window;
	
	// global Game Clock
	sf::Clock m_gameClock;

	// Resource Managers
	TextureManager* textureManager;
	ProgramManager* programManager;
	ModelManager* modelManager;
	SoundBufferManager* soundBufferManager;

	// Global Default Cubemap
	Cubemap* cubeMap;

	struct {
		AmmunitionIcon* MunitionIconPistol;
		AmmunitionIcon* MunitionIconSMG;
		AmmunitionIcon* MunitionIconRocket;
		AmmunitionIcon* MunitionIconShotgun;
	} sIngame;

	struct {
		Button* buttonPlay;
		Button* buttonCredits;
		Button* buttonExit;
		std::vector<Button*> buttonVec;
	} sMenu;

	struct {
		Button* buttonBack;
		std::vector<Button*> buttonVec;
	} sCredits;

	struct {

	} sGameOverScreen;

public:
	enum class GameState {
		MainMenu,
		Ingame,
		Credits,
		GameOver
	};

	void init();
	void run();
	void exit();

private:
	GameState m_gameState;

	void drawFpsCounter(sf::Time timeElapsed);
	void drawMainMenu();
	void drawCredits();
	void drawGameOverScreen();
};