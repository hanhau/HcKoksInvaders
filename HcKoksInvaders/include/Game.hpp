#pragma once
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
#include "BulletRenderer.hpp"
#include "HighscoreManager.hpp"
#include "GameLaunchOptions.hpp"

class TileEntityBase;
class GameWorld;
class StarShip;

class Game {
private:
	friend class TileEntityBase;
	
	// Game Launch Options
	GameLaunchOptions m_gameLaunchOptions;

	// Window
	sf::Window window;
	
	// global Game Clock
	sf::Clock m_gameClock;

	// Resource Managers
	SoundBufferManager* soundBufferManager;

	// Global Default Cubemap
	Cubemap* cubeMap;

	struct __sIngame {
		AmmunitionIcon* MunitionIconPistol;
		AmmunitionIcon* MunitionIconSMG;
		AmmunitionIcon* MunitionIconRocket;
		AmmunitionIcon* MunitionIconShotgun;
		
		sf::Clock clockRefillSMG;
		sf::Clock clockRefillShotgun;
		sf::Clock clockRefillRocket;

		static const float intervalRefillSMG;
		static const float intervalRefillShotgun;
		static const float intervalRefillRocket;

		Text* textHealth;
		Text* textPoints;
		Text* textStage;
		
		std::list<Bullet> bullets;
		
		GameWorld* gameWorld;
		StarShip* playerShip;

		BulletRenderer* bulletRenderer;

		int currentStage;
		int currentPoints;
		int currentHealth;
		sf::Clock stageClock;
		
		void updateBullets(float deltaTime);
		void letBulletsDie(const glm::vec3 aliveCenter, 
						   const float aliveDiameter);

		float getCurrentYPos();
		void drawHUDText(const sf::Window& win, const Program& program);
		void prepareStage(int stage);
		bool isStageFinished();
		bool isGameOver();

		static const float stageOffsetStartY;
		static const float stageOffsetEndY;
		static const int stageHeight;
	} sIngame;

	struct __sMenu {
		Button* buttonPlay;
		Button* buttonCredits;
		Button* buttonExit;
		std::vector<Button*> buttonVec;

		Text* textTitle;

		Text* textHighscore;
		Text* textHighscorePoints;
		Text* textHighscoreStages;
		
		// optional if online
		Text* textLoginName;
		Text* textPlayedGames;

		sf::Music music;

		int highscorePoints = 0;
		int highscoreStages = 0;
		void refreshHighscore(const GameLaunchOptions& glo);
	} sMenu;

	struct __sCredits {
		Button* buttonBack;
		std::vector<Button*> buttonVec;

		sf::Music music;
	} sCredits;

	struct __sGameOverScreen {
		int points;
		int stages;

		Button* buttonNewGame;
		Button* buttonMainMenu;
		Button* buttonExitGame;
		std::vector<Button*> buttonVec;
	} sGameOver;

public:
	static const std::string VERSION_STRING;

	enum class GameState {
		MainMenu,
		Ingame,
		Credits,
		GameOver,
		Exit
	};

	void init(const GameLaunchOptions& glo);
	void run();
	void exit();

	const SoundBufferManager& getSoundBufferManager();

	StarShip* getStarShip();

	void addBullet(Bullet&& bullet);
	void startGame();
private:
	GameState m_gameState;

	void drawFpsCounter(sf::Time timeElapsed);
	void drawMainMenu();
	void drawCredits();
	void drawGameOverScreen();
};