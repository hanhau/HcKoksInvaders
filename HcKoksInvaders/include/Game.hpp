#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio/Music.hpp>
#include <glm/mat4x4.hpp>

#include "TextureManager.hpp"
#include "ProgramManager.hpp"
#include "ModelManager.hpp"
#include "SoundBufferManager.hpp"
#include "Camera.hpp"
#include "AmmunitionIcon.hpp"
#include "Button.hpp"
#include "Bullet.hpp"

class TileEntityBase;
class GameWorld;
class StarShip;

struct GameLaunchOptions {
	glm::ivec2 res;
	bool fullscreen;
};

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

	struct __sIngame {
		AmmunitionIcon* MunitionIconPistol;
		AmmunitionIcon* MunitionIconSMG;
		AmmunitionIcon* MunitionIconRocket;
		AmmunitionIcon* MunitionIconShotgun;

		Text* textHealth;
		Text* textPoints;
		Text* textStage;

		std::list<Bullet> bullets;
		void updateBullets(float deltaTime);
		GameWorld* gameWorld;
		StarShip* playerShip;

		int currentStage;
		int currentPoints;
		int currentHealth;

		void drawHUDText(const sf::Window& win, const Program& program);
		void prepareStage(int stage);

		bool isStageFinished();
		void startStage();

		float currentProgressY;
		static const float stageOffsetStartY;
		static const float stageOffsetEndY;

	} sIngame;

	struct __sMenu {
		Button* buttonPlay;
		Button* buttonCredits;
		Button* buttonExit;
		std::vector<Button*> buttonVec;

		Text* textTitle;
		sf::Music music;
	} sMenu;

	struct __sCredits {
		Button* buttonBack;
		std::vector<Button*> buttonVec;

		sf::Music music;
	} sCredits;

	struct __sGameOverScreen {
		Button* buttonNewGame;
		Button* buttonMainMenu;
		Button* buttonExitGame;
		std::vector<Button*> buttonVec;
	} sGameOver;

public:
	enum class GameState {
		MainMenu,
		Ingame,
		Credits,
		GameOver
	};

	void init(const GameLaunchOptions& glo);
	void run();
	void exit();

	const TextureManager& getTextureManager();
	const ProgramManager& getProgramManager();
	const ModelManager& getModelManager();
	const SoundBufferManager& getSoundBufferManager();

	StarShip* getStarShip();

	void addBullet(Bullet&& bullet);
private:
	GameState m_gameState;

	void drawFpsCounter(sf::Time timeElapsed);
	void drawMainMenu();
	void drawCredits();
	void drawGameOverScreen();
};