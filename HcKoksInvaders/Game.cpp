#include <GL/glew.h>
#include <math.h>
#include <sfml/Audio.hpp>
#include "include/Model.hpp"
#include "include/TextureManager.hpp"
#include "include/ProgramManager.hpp"
#include <iostream>
#include "include/Button.hpp"
#include <glm\ext\matrix_transform.hpp>
#include <glm\ext\matrix_clip_space.hpp>
#include "include/Util.hpp"
#include "include/GameWorld.hpp"
#include <sstream>
#include <thread>
#include <algorithm>
#include "include/StarBackground.hpp"
#include "include/AmmunitionIcon.hpp"
#include "include/BulletRenderer.hpp"
#include "include/Text.hpp"
#include "include/StarShip.hpp"

void handleButtons_MouseLeftClicked(const std::vector<Button*>& buttons,
	const sf::Event::MouseButtonEvent& mbEvent)
{
	if (mbEvent.button != sf::Mouse::Button::Left)
		return;

	for (auto& iter : buttons) {
		if (iter->containsPoint(glm::ivec2(mbEvent.x, mbEvent.y))) {
			iter->onClick();
		}
	}
}

void Game::init(const GameLaunchOptions& glo) {
	// Init window with Context
	sf::ContextSettings cs(24, 0, 0, 4, 3, 0, false);

	window.create(
		sf::VideoMode(glo.res.x, glo.res.y), 
		"HcKoksInvaders", 
		glo.fullscreen ? sf::Style::Fullscreen : sf::Style::Close,
		cs
	);
	window.setActive(true);

	// set OpenGL Function Ptrs
	glewExperimental = GL_TRUE;
	glewInit();

	// check GL Compatibility
	GLint max_layers;
	glGetIntegerv(GL_MAX_ARRAY_TEXTURE_LAYERS, &max_layers);
	assert(max_layers >= 128);

	// global depth test
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glClearColor(0.0, 0.0, 0.0, 1.0);

	// Initialize ResourceManagers
	textureManager = new TextureManager();
	programManager = new ProgramManager();
	modelManager = new ModelManager(*textureManager,*programManager);
	soundBufferManager = new SoundBufferManager();

	// Cubemap
	cubeMap = new Cubemap(*textureManager);

	// Check OpenGL Version
	std::cout << "OpenGL-Version: " << (const char*)glGetString(GL_VERSION) << "\n";
	std::cout << "Vendor: " << (const char*)glGetString(GL_VENDOR) << "\n";
	std::cout << "Renderer: " << (const char*)glGetString(GL_RENDERER) << "\n";
	sf::ContextSettings settings = window.getSettings();
	std::cout << "depth bits:" << settings.depthBits << std::endl;
	std::cout << "stencil bits:" << settings.stencilBits << std::endl;
	std::cout << "antialiasing level:" << settings.antialiasingLevel << std::endl;

	// Check OpenGL Errors
	util::checkGlCalls(__FUNCSIG__);

	// Load IngameUI
	{
		// AmmunitionIcons
		sIngame.MunitionIconPistol = new AmmunitionIcon(
			"res/images/icon_munition_pistol.png",
			glm::vec4(1.f),
			40,
			glm::ivec2(10,10),
			*textureManager,
			window
		);
		sIngame.MunitionIconSMG = new AmmunitionIcon(
			"res/images/icon_munition_smg.png",
			glm::vec4(1.f),
			40,
			glm::ivec2(100, 10),
			*textureManager,
			window
		);
		sIngame.MunitionIconRocket = new AmmunitionIcon(
			"res/images/icon_munition_rocket.png",
			glm::vec4(0.76,0.55,0.45,1.f),
			40,
			glm::ivec2(190, 10),
			*textureManager,
			window
		);
		sIngame.MunitionIconShotgun = new AmmunitionIcon(
			"res/images/icon_munition_shotgun.png",
			glm::vec4(1.f),
			40,
			glm::ivec2(280, 10),
			*textureManager,
			window
		);

		// Texts
		sIngame.textHealth = new Text("HP: 100", 20, glm::ivec2());
		sIngame.textPoints = new Text("Pts: 1.000.000", 20, glm::ivec2());
		sIngame.textStage = new Text("Stage: 999", 20, glm::ivec2());
	}

	// Load MainMenu UI
	{
		const int windowHeight = window.getSize().y;

		// SPIELEN 
		sMenu.buttonPlay = new Button(
			window, "SPIELEN", 
			glm::ivec2(50, windowHeight-260),
		48);
		sMenu.buttonPlay->onClick = std::function<void()>([&]() {
			static sf::Sound s(soundBufferManager->get("res/audio/select.flac"));
			s.play();
			
			startGame();
		});

		// CREDITS
		sMenu.buttonCredits = new Button(
			window, "CREDITS", 
			glm::ivec2(50, windowHeight-180), 
		48);
		sMenu.buttonCredits->onClick = std::function<void()>([&]() {
			sMenu.music.setVolume(0);
			sCredits.music.setPlayingOffset(sf::Time());
			sCredits.music.setVolume(100);
			
			static sf::Sound s(soundBufferManager->get("res/audio/select.flac"));
			s.play();
			m_gameState = GameState::Credits;
		});

		// VERLASSEN
		sMenu.buttonExit = new Button(
			window, "VERLASSEN", 
			glm::ivec2(50, windowHeight-100), 
		48);
		sMenu.buttonExit->onClick = std::function<void()>([&]() {
			glClearColor(0, 0, 0, 1.0);
			glClear(GL_COLOR_BUFFER_BIT);
			window.display();
			sMenu.music.setVolume(0);

			static sf::Sound s(soundBufferManager->get("res/audio/hcDankeBye.wav"));
			s.play();
			sf::sleep(s.getBuffer()->getDuration());

			this->exit();
		});

		sMenu.buttonVec = { sMenu.buttonPlay,sMenu.buttonCredits, sMenu.buttonExit };

		// Text
		sMenu.textTitle = new Text("HCKOKSINVADERS", 43, glm::ivec2(20,20));

		sMenu.textHighscore = new Text("HIGHSCORE", 38, glm::ivec2(0, windowHeight / 2.f - 42));
		sMenu.textHighscorePoints = new Text("123423 Punkte", 40, glm::ivec2(0, windowHeight / 2.f));
		sMenu.textHighscoreStages = new Text("999 Stages", 36, glm::ivec2(0, windowHeight / 2.f + 44));
	}

	// Load Credits UI
	{
		sCredits.buttonBack = new Button(window, "Z'RUCK", glm::ivec2(25, 25), 44);
		sCredits.buttonBack->onClick = std::function<void()>([&]() {
			sCredits.music.setVolume(0);
			sMenu.music.setVolume(100);

			static sf::Sound s(soundBufferManager->get("res/audio/select.flac"));
			s.play();
			m_gameState = GameState::MainMenu;
		});

		sCredits.buttonVec = { sCredits.buttonBack };
	}

	// Load GameOver UI
	{
		// NOCHMAL
		sGameOver.buttonNewGame = new Button(window, "NOCHMAL", glm::ivec2(0,window.getSize().y/2-64), 44, true);
		sGameOver.buttonNewGame->onClick = std::function<void()>([&]() {
			startGame();
		});

		// HAUPTSCHIRM
		sGameOver.buttonMainMenu = new Button(window, "HAUPTSCHIRM", glm::ivec2(0, window.getSize().y/2), 44, true);
		sGameOver.buttonMainMenu->onClick = std::function<void()>([&]() {
			m_gameState = GameState::MainMenu;
		});

		// RAGEQUIT
		sGameOver.buttonExitGame = new Button(window, "RAGEQUIT", glm::ivec2(0,window.getSize().y/2+64), 44, true);
		sGameOver.buttonExitGame->onClick = std::function<void()>([&]() {
			window.close();
		});

		// buttonvec
		sGameOver.buttonVec = { sGameOver.buttonNewGame, sGameOver.buttonMainMenu, sGameOver.buttonExitGame };
	}

	// Load music
	{
		sMenu.music.openFromFile("res/audio/main_menu.ogg");
		sCredits.music.openFromFile("res/audio/credits.flac");
	}

	// Init sIngame
	{
		sIngame.playerShip = new StarShip(*this);
		sIngame.playerShip->addRocketAmmo(100);
		sIngame.playerShip->addShotgunAmmo(100);
		sIngame.playerShip->addSMGAmmo(100);

		sIngame.gameWorld = new GameWorld(*this);

		sIngame.bulletRenderer = new BulletRenderer(*programManager);
	}

	// Init GameState
	m_gameState = GameState::MainMenu;
}

void Game::run() {
	sMenu.music.setLoop(true);
	sMenu.music.setVolume(100);
	sMenu.music.play();
	sMenu.refreshHighscore();

	sCredits.music.setLoop(true);
	sCredits.music.setVolume(0);
	sCredits.music.play();

	sf::Clock gameClock;
	sf::Clock fpsClock;

	Camera cam1 = Camera(
		glm::vec3(0.0f, 0.0f, 5.0f),
		glm::vec3(0.0f, 0.45f, -1.0f),
		glm::vec3(0.0f, 1.0f, 0.0f),
		glm::perspective(glm::radians(47.5f), window.getSize().x / (float)window.getSize().y, 1.f, 100.f)
	);

	bool wireframe = false;
	double lastFrameTime = 0.0f;

	window.setKeyRepeatEnabled(true);

	while (window.isOpen())
	{
		// Event Polling
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
				continue;
			}

			if (event.type == sf::Event::KeyPressed) {
				if (event.key.code == sf::Keyboard::F5)
				{
					if (!wireframe) {
						glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
						wireframe = true;
					}
					else if(wireframe) {
						glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
						wireframe = false;
					}
				}
			}

			if (event.type == sf::Event::MouseButtonPressed) {
				switch (m_gameState) {
					case GameState::MainMenu :
						handleButtons_MouseLeftClicked(sMenu.buttonVec, event.mouseButton);
						break;
					case GameState::Credits:
						handleButtons_MouseLeftClicked(sCredits.buttonVec, event.mouseButton);
						break;
					case GameState::GameOver:
						handleButtons_MouseLeftClicked(sGameOver.buttonVec, event.mouseButton);
						break;
					default: break;
				}
			}
		}

		// Start of Frame
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Handle Frame
		switch (m_gameState) {
			// INGAME // -------------------------------
			case GameState::Ingame:
			{			
				static StarBackground starBkg;
				starBkg.draw(
					window, 
					programManager->get(ProgramManager::ProgramEntry::IngameBackground), 
					m_gameClock.getElapsedTime().asSeconds()
				);

				auto cpos = sIngame.playerShip->getPos();
				sIngame.playerShip->setPos(glm::vec3(cpos.x, sIngame.getCurrentYPos(), cpos.z));
				cam1.setCameraPos(glm::vec3(0, sIngame.getCurrentYPos() - 0.5f, 5));

				sIngame.updateBullets(lastFrameTime);
				sIngame.gameWorld->updateOnBulletCollisions(
					sIngame.bullets,
					sIngame.playerShip->getPos(),
					4.5f, 1.f,
					sIngame.currentPoints
				);
				sIngame.bulletRenderer->drawInstances(sIngame.bullets, cam1);

				sIngame.playerShip->updateSoundBuffers();
				sIngame.playerShip->updateOnUserInput(lastFrameTime);

				sIngame.gameWorld->draw(cam1, *cubeMap);
				sIngame.playerShip->draw(cam1, *cubeMap);

				const Program& aiProg = programManager->get(ProgramManager::ProgramEntry::AmmunitionIcon);
				const Program& textProg = programManager->get(ProgramManager::ProgramEntry::Text);

				sIngame.drawHUDText(window, textProg);

				sIngame.MunitionIconPistol->draw(window,100.f, aiProg);
				sIngame.MunitionIconRocket->draw(window,sIngame.playerShip->getRocketAmmoPercent(), aiProg);
				sIngame.MunitionIconShotgun->draw(window,sIngame.playerShip->getShotgunAmmoPercent(), aiProg);
				sIngame.MunitionIconSMG->draw(window,sIngame.playerShip->getSMGAmmoPercent(), aiProg);

				if (sIngame.isGameOver()) {
					HighscoreManager::updateEntry(
						sIngame.currentPoints, 
						sIngame.currentStage
					);
				}

				if (sIngame.isStageFinished()) {
					std::cout << "Stage finished\n";
					sIngame.bullets.clear();
					sIngame.prepareStage(sIngame.currentStage + 1);
					sIngame.currentStage++;

					sIngame.stageClock.restart();
				}
			}
			break;
			// CREDITS // ------------------------------
			case GameState::Credits:
			{
				drawCredits();
			}
			break;
			// MAINMENU // -----------------------------
			case GameState::MainMenu:
			{
				drawMainMenu();
			}
			break;
			// GAMEOVER // -----------------------------
			case GameState::GameOver:
			{
				drawGameOverScreen();
			}
			break;
			// -----------------------------------------
			default: 
				throw; 
			break;
		}

		// End of Frame
		lastFrameTime = fpsClock.getElapsedTime().asMicroseconds() / 1'000'000.0;
		drawFpsCounter(fpsClock.getElapsedTime());
		fpsClock.restart();
		window.display();
	}
}

void Game::exit() {
	window.close();
}

const TextureManager& Game::getTextureManager() {
	return *textureManager;
}
const ProgramManager& Game::getProgramManager() {
	return *programManager;
}
const ModelManager& Game::getModelManager() {
	return *modelManager;
}
const SoundBufferManager& Game::getSoundBufferManager() {
	return *soundBufferManager;
}

StarShip* Game::getStarShip() {
	return sIngame.playerShip;
}

void Game::addBullet(Bullet&& bullet) {
	sIngame.bullets.push_back(bullet);
}

void Game::startGame() {
	sIngame.bullets.clear();

	sIngame.currentStage = 1;
	sIngame.currentPoints = 0;
	sIngame.currentHealth = 100;

	sIngame.gameWorld->init(__sIngame::stageHeight, 1);

	sIngame.stageClock.restart();
	m_gameState = GameState::Ingame;
}

// private functions
void Game::drawFpsCounter(sf::Time timeElapsed) {
	static const Program& textProg = programManager->get(ProgramManager::ProgramEntry::Text);
	static Text text = Text("n",18,glm::ivec2(2,2));

	text.setText(std::to_string((int)(1'000'000.0/(double)std::max((unsigned long long)timeElapsed.asMicroseconds(),1ull))));
	text.draw(window, textProg);
}

void Game::drawMainMenu() {
	static StarBackground starBkg;
	static InstanceBuffer busPos(1);

	const float secs = m_gameClock.getElapsedTime().asSeconds() * 3.0f;
	static const Model3D& bus = modelManager->getModel("res/models/vengabus_hq.obj");
	static const Program& textProg = programManager->get(ProgramManager::ProgramEntry::Text);

	static Camera cam;
	cam.setCameraPos(glm::vec3(0.0f, 0.0f, 3.0f));
	cam.setCameraFront(glm::vec3(0.0f, 0.0f, -1.0f));
	cam.setCameraUp(glm::vec3(0.0f, 1.0f, 0.0f));
	cam.setProjectionMatrix(glm::perspective(glm::radians(85.f), window.getSize().x / (float)window.getSize().y, 1.f, 500.f));

	busPos[0] = ModelPosition(
		glm::vec3(),
		glm::radians(5.f), glm::vec3(cosf(secs*0.33f),sinf(secs*0.25f),cosf(secs*0.15f+3.33f)),
		glm::vec3(2.0f,2.0f,2.0f)
	);
	busPos.transferToGpu();

	starBkg.draw(window,programManager->get(ProgramManager::ProgramEntry::MainMenuBackground), secs);
	bus.drawInstanceQueue(busPos, cam, *cubeMap);

	for (auto& iter : sMenu.buttonVec)
		iter->draw(window, *programManager);

	sMenu.textTitle->draw(window, textProg, glm::vec3(
		abs(cosf(secs * 2.0f)),
		abs(cosf(secs * 2.0f + glm::radians(120.f))),
		abs(cosf(secs * 2.0f + glm::radians(240.f)))
	));

	sMenu.textHighscorePoints->setText(std::to_string(sMenu.highscorePoints) + std::string(" Punkte"));
	sMenu.textHighscoreStages->setText(std::to_string(sMenu.highscoreStages) + std::string(" Stages"));

	sMenu.textHighscore->centerHorizontally(window);
	sMenu.textHighscorePoints->centerHorizontally(window);
	sMenu.textHighscoreStages->centerHorizontally(window);

	sMenu.textHighscore->draw(window, textProg, glm::vec3(0.f, 1.f, 0.518f));
	sMenu.textHighscorePoints->draw(window, textProg);
	sMenu.textHighscoreStages->draw(window, textProg);
}

void Game::drawCredits() {
	static StarBackground starBkg;
	static InstanceBuffer moneyPos(100);
	static InstanceBuffer fingerPos(1);
	static InstanceBuffer busPos(1);

	const float secs = m_gameClock.getElapsedTime().asSeconds()*2*(60.f/130.f);
	static const Model3D& money = modelManager->getModel("res/models/money.obj");
	static const Model3D& finger = modelManager->getModel("res/models/finger.obj");
	static const Model3D& bus = modelManager->getModel("res/models/vengabus.obj");

	static Text textEhrenMann = Text(
		"HCKOKSINVADERS V1.0", 24,
		glm::ivec2(20, window.getSize().y - 200)
	);
	static Text textCreatedBy = Text(
		"created by",48,
		glm::ivec2(20, window.getSize().y - 160)
	);
	static Text textHannes = Text(
		"Hannes H.",64,
		glm::ivec2(20, window.getSize().y - 100)
	);	
	static Text textFlo = Text(
		"Flo is gay @ 4te Klasse", 12,
		glm::ivec2(20,window.getSize().y-13)
	);

	static Camera cam;
	cam.setCameraPos(glm::vec3(sinf(secs)*0.25f, cos(secs)*0.25f, 1.0f));
	cam.setCameraFront(glm::vec3(0.0f + sinf(secs) * 0.05, 0.0f + cosf(secs) * 0.05, -1.0f));
	cam.setCameraUp(glm::vec3(0.0f, 1.0f, 0.0f));
	cam.setProjectionMatrix(glm::perspective(glm::radians(65.f), window.getSize().x / (float)window.getSize().y, 1.f, 500.f));

	starBkg.draw(window,programManager->get(ProgramManager::ProgramEntry::MainMenuBackground), secs*3.0);

	for (double i = 0, j = 0; i < 1000.0; i += 10.0,j+=1.0) {
		float s = 0.2f + abs(cosf(secs + i)) * 0.1f;
		moneyPos[(int)j] = 
			ModelPosition(
				sf::Vector3f(
					cos(i + secs / 2.52442) * 2.0,
					sin(i + secs / 2.52442) * 2.0,
					-50.f + j*0.5
				),
				secs*cos(j)*0.25f*j, sf::Vector3f(cosf(i),cosf(j),cosf(i+j)),
				sf::Vector3f(s, s, s)
			);
	}

	fingerPos[0] = ModelPosition(
		sf::Vector3f(0.0,-0.2 + cos(secs*3)*0.4,-1.0f-abs(cosf(secs*0.5f))*40.0f),
		glm::radians(180.f+(sin(secs*1.2)+1.0)*90.f), sf::Vector3f(0.0001f,1.0,0.0001f),
		sf::Vector3f(0.35,0.35,0.35)
	);

	busPos[0] = ModelPosition(
		sf::Vector3f(0.0, -0.2 + cos(secs * 3) * 0.4, -1.0f - abs(sinf(secs*0.5f)) * 40.0f),
		glm::radians(90.f + (sin(secs*1.1) + 1.0) * 90.f), sf::Vector3f(0.0001f, 1.0, 0.0001f),
		sf::Vector3f(0.35, 0.35, 0.35)
	);

	moneyPos.transferToGpu();
	fingerPos.transferToGpu();
	busPos.transferToGpu();

	money.drawInstanceQueue(moneyPos, cam, *cubeMap);
	finger.drawInstanceQueue(fingerPos, cam, *cubeMap);
	bus.drawInstanceQueue(busPos, cam, *cubeMap);

	const Program& textProg = programManager->get(ProgramManager::ProgramEntry::Text);
	textCreatedBy.draw(window, textProg);
	textHannes.draw(window, textProg);
	textEhrenMann.draw(window, textProg, glm::vec3(
		abs(cosf(secs*2.0f)),
		abs(cosf(secs*2.0f + glm::radians(120.f))),
		abs(cosf(secs*2.0f + glm::radians(240.f)))
	));
	textFlo.draw(window, textProg, glm::vec3(
		abs(cosf(secs * 2.0f)),
		abs(cosf(secs * 2.0f + glm::radians(120.f))),
		abs(cosf(secs * 2.0f + glm::radians(240.f)))
	));

	for (auto& iter : sCredits.buttonVec)
		iter->draw(window, *programManager);
}

void Game::drawGameOverScreen() {
	static const Model3D& bus = modelManager->getModel("res/models/vengabus.obj");
	static const Program& textProg = programManager->get(ProgramManager::ProgramEntry::Text);

	static Text textTailor = Text("schade.",72,glm::ivec2(0,100));
	textTailor.centerHorizontally(window);
	static Text textHighscore = Text("Dein Highscore:", 36, glm::ivec2(0, 220));
	textHighscore.centerHorizontally(window);
	static Text textHighscorePoints = Text("1.234.567", 44, glm::ivec2(0,274));
	textHighscorePoints.centerHorizontally(window);

	static StarBackground starBkg;
	static InstanceBuffer busPos(1);
	static Camera cam;

	const float secs = m_gameClock.getElapsedTime().asSeconds();

	cam.setCameraPos(glm::vec3(0.0f,10.0f,10.0f));
	cam.setCameraFront(glm::vec3(0.0f, -0.5f, -1.0f));
	cam.setCameraUp(glm::vec3(0.0f, 1.0f, 0.0f));
	cam.setProjectionMatrix(glm::perspective(glm::radians(65.f), window.getSize().x / (float)window.getSize().y, 1.f, 500.f));	

	busPos[0] = std::move(ModelPosition(
		glm::vec3(0.0f,0.0f,0.0f),
		secs,glm::vec3(0.000001f,1.0f,0.000001f),
		glm::vec3(0.75f,0.75f,0.75f)
	));
	busPos.transferToGpu();

	starBkg.draw(window,programManager->get(ProgramManager::ProgramEntry::MainMenuBackground),secs);
	bus.drawInstanceQueue(busPos, cam, *cubeMap);

	textTailor.draw(window, textProg, glm::vec3(1.0, 0.0, 0.0));
	textHighscore.draw(window, textProg);
	textHighscorePoints.draw(window, textProg);

	for (auto& iter : sGameOver.buttonVec)
		iter->draw(window, *programManager);
}

// struct funcs
void Game::__sMenu::refreshHighscore() {
	HighscoreManager::get(highscorePoints, highscoreStages);
}

void Game::__sIngame::updateBullets(float deltaTime) {
	for (auto& iter : bullets) {
		iter.update(deltaTime);
	}
}

float Game::__sIngame::getCurrentYPos() {
	return stageOffsetStartY + stageClock.getElapsedTime().asSeconds() * 0.25f;
}

void Game::__sIngame::drawHUDText(const sf::Window& win,const Program& program) {
	const int padRight = 20;

	textPoints->setText(std::string("Pts: ") + std::to_string(currentPoints));
	textHealth->setText(std::string("HP: ") + std::to_string(currentHealth));
	textStage->setText(std::string("Stage: ") + std::to_string(currentStage));

	int textPointsWidth = textPoints->getSizePixels(win).x;
	int textHealthWidth = textHealth->getSizePixels(win).x;
	int textStageWidth = textStage->getSizePixels(win).x;

	textPoints->setPos(glm::ivec2(win.getSize().x-textPointsWidth-padRight,25));
	textHealth->setPos(glm::ivec2(win.getSize().x-textHealthWidth-padRight,47));
	textStage->setPos(glm::ivec2(win.getSize().x-textStageWidth-padRight,69));

	glm::vec3 healthCol;
	if (currentHealth >= 0 && currentHealth < 33)
		healthCol = glm::vec3(252.f / 255.f, 44.f / 255.f, 44.f / 255.f);
	else if (currentHealth >= 33 && currentHealth < 60)
		healthCol = glm::vec3(252.f / 255.f, 164.f / 255.f, 44.f / 255.f);
	else if (currentHealth >= 60 && currentHealth < 80)
		healthCol = glm::vec3(251.f / 255.f, 225.f / 255.f, 45.f / 255.f);
	else
		healthCol = glm::vec3(86.f / 255.f, 250.f / 255.f, 46.f / 255.f);

	textPoints->draw(win, program);
	textHealth->draw(win, program, healthCol);
	textStage->draw(win, program);
}

void Game::__sIngame::prepareStage(int stage) {
	gameWorld->init(stageHeight, stage);
}

bool Game::__sIngame::isStageFinished() {
	return getCurrentYPos() > gameWorld->getNDCHeight() + stageOffsetEndY;
}

bool Game::__sIngame::isGameOver() {
	return currentHealth <= 0;
}

const float Game::__sIngame::stageOffsetEndY = 1.f;
const float Game::__sIngame::stageOffsetStartY = -4.f;
const int Game::__sIngame::stageHeight = 192;