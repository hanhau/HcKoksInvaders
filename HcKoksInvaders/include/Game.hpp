#pragma once
#include <SFML/Graphics.hpp>
#include <glm/mat4x4.hpp>

#include "TextureManager.hpp"
#include "ProgramManager.hpp"
#include "ModelManager.hpp"
#include "SoundBufferManager.hpp"
#include "Camera.hpp"
#include "AmmunitionIcon.hpp"

class TileEntityBase;

class Game {
private:
	friend class TileEntityBase;
	
	sf::Window window;
	
	// global Matrices
	glm::mat4x4 matView;
	glm::mat4x4 matPerspective;

	// global Game Clock
	sf::Clock m_gameClock;

	// Cameras
	Camera cam_MainMenu;
	Camera cam_Ingame;
	Camera cam_Credits;

	// Resource Managers
	TextureManager* textureManager;
	ProgramManager* programManager;
	ModelManager* modelManager;
	SoundBufferManager* soundBufferManager;

	// Global Default Cubemap
	Cubemap* cubeMap;

	// Ingame UI
	AmmunitionIcon* ingameMunitionIconPistol;
	AmmunitionIcon* ingameMunitionIconSMG;
	AmmunitionIcon* ingameMunitionIconRocket;
	AmmunitionIcon* ingameMunitionIconShotgun;

	struct {

	} sIngame;

	struct {

	} sMenu;

	struct {

	} sCredits;

	struct {

	} sGameOverScreen;

public:
	enum class GameState {
		MainMenu,
		Ingame,
		Credits
	};

	void init();
	void run();
	void exit();

private:
	void drawFpsCounter(sf::Time timeElapsed);
	void drawMainMenu();
	void drawCredits();
	void drawGameOverScreen();
};