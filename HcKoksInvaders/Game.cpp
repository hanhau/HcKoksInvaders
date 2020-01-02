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

void Game::init() {
	// Init window with Context
	sf::ContextSettings cs(24,8,2,4,3,0U,false);
	window.create(sf::VideoMode(640, 960), "HcKoksInvaders", sf::Style::Close,cs);
	window.setActive(true);
	//window.setVerticalSyncEnabled(true);

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
	glClearColor(0.5, 0.75, 0.25, 1.0);

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
		sIngame.MunitionIconPistol = new AmmunitionIcon(
			"res/images/icon_munition_pistol.png",
			sf::Color::Blue,
			0.1f,
			sf::Vector2f(-0.85f, 0.9f),
			*textureManager,
			window
		);
		sIngame.MunitionIconSMG = new AmmunitionIcon(
			"res/images/icon_munition_smg.png",
			sf::Color::Yellow,
			0.1f,
			sf::Vector2f(-0.6f, 0.9f),
			*textureManager,
			window
		);
		sIngame.MunitionIconRocket = new AmmunitionIcon(
			"res/images/icon_munition_rocket.png",
			sf::Color::Green,
			0.1f,
			sf::Vector2f(-0.35f, 0.9f),
			*textureManager,
			window
		);
		sIngame.MunitionIconShotgun = new AmmunitionIcon(
			"res/images/icon_munition_shotgun.png",
			sf::Color::Red,
			0.1f,
			sf::Vector2f(-0.1f, 0.9f),
			*textureManager,
			window
		);
	}

	// Load MainMenu UI
	{
		// Buttons
		sMenu.buttonPlay = new Button(window, "SPIELEN", glm::ivec2(50, 700), 48);
		sMenu.buttonPlay->onClick = std::function<void()>([&]() {
			m_gameState = GameState::Ingame;
		});
		sMenu.buttonCredits = new Button(window, "CREDITS", glm::ivec2(50, 780), 48);
		sMenu.buttonCredits->onClick = std::function<void()>([&]() {
			m_gameState = GameState::Credits;
		});
		sMenu.buttonExit = new Button(window, "VERLASSEN", glm::ivec2(50, 860), 48);
		sMenu.buttonExit->onClick = std::function<void()>([&]() {
			this->exit();
		});
		sMenu.buttonVec = { sMenu.buttonPlay,sMenu.buttonCredits, sMenu.buttonExit };

		// Text
		sMenu.textTitle = new Text("HCKOKSINVADERS", 43, glm::ivec2(20,20));
	}

	// Load Credits UI
	{
		sCredits.buttonBack = new Button(window, "Z'RUCK", glm::ivec2(25, 25), 44);
		sCredits.buttonBack->onClick = std::function<void()>([&]() {
			m_gameState = GameState::MainMenu;
		});

		sCredits.buttonVec = { sCredits.buttonBack };
	}

	// Init GameState
	m_gameState = GameState::MainMenu;
}

void Game::run() {
	GameWorld gameWorld(*this);
	gameWorld.init(1024, 2);
	gameWorld.saveToFileAsImage("demo.bmp");

	sf::Clock gameClock;
	sf::Clock fpsClock;
	std::vector<long long> frametimes;

	Camera cam1 = Camera(
		glm::vec3(0.0f, 0.0f, 5.0f),
		glm::vec3(0.0f, 0.45f, -1.0f),
		glm::vec3(0.0f, 1.0f, 0.0f),
		glm::perspective(glm::radians(47.5f), 640.f / 960.f, 1.f, 100.f)
	);

	bool wireframe = false;
	double lastFrameTime = 0.0f;

	StarShip* playerShip = new StarShip(*this);
	playerShip->addRocketAmmo(100);
	playerShip->addShotgunAmmo(100);
	playerShip->addSMGAmmo(100);

	BulletRenderer br(*programManager);

	window.setKeyRepeatEnabled(true);

	while (window.isOpen())
	{
		// Event Polling
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
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
					default: break;
				}
			}
		}

		// Start of Frame
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		programManager->get(ProgramManager::ProgramEntry::Model3D).setUniform(
			"matProgressCubemap",
			glm::rotate(glm::identity<glm::mat4>(), glm::radians(gameClock.getElapsedTime().asSeconds()), glm::vec3(1.0, 0.1, 0.1))
		);

		switch (m_gameState) {
			// INGAME // -------------------------------
			case GameState::Ingame:
			{
				sIngame.updateBullets(lastFrameTime);
				br.drawInstances(sIngame.bullets, cam1);

				playerShip->updateOnUserInput(lastFrameTime);

				cam1.setCameraPos(glm::vec3(0.0f,0.f,5.0f));
				gameWorld.draw(cam1, *cubeMap);

				playerShip->draw(cam1, *cubeMap);

				const Program& aiProg = programManager->get(ProgramManager::ProgramEntry::AmmunitionIcon);
				sIngame.MunitionIconPistol->draw(100.f, aiProg);
				sIngame.MunitionIconRocket->draw(playerShip->getRocketAmmoPercent(), aiProg);
				sIngame.MunitionIconShotgun->draw(playerShip->getShotgunAmmoPercent(), aiProg);
				sIngame.MunitionIconSMG->draw(playerShip->getSMGAmmoPercent(), aiProg);
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
		frametimes.push_back(fpsClock.getElapsedTime().asMicroseconds());
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

void Game::addBullet(Bullet&& bullet) {
	sIngame.bullets.push_back(bullet);
}

// private functions
void Game::drawFpsCounter(sf::Time timeElapsed) {
	static const Program& textProg = programManager->get(ProgramManager::ProgramEntry::Text);
	static Text text = Text("",18,glm::ivec2(2,2));

	text.setText(std::to_string((int)(1000.0/std::max(timeElapsed.asMilliseconds(),1))));
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
	cam.setProjectionMatrix(glm::perspective(glm::radians(85.f), 640.f / 960.f, 1.f, 500.f));

	busPos[0] = ModelPosition(
		glm::vec3(),
		glm::radians(5.f), glm::vec3(cosf(secs*0.33f),sinf(secs*0.25f),cosf(secs*0.15f+3.33f)),
		glm::vec3(2.0f,2.0f,2.0f)
	);
	busPos.transferToGpu();

	starBkg.draw(programManager->get(ProgramManager::ProgramEntry::MainMenuBackground), secs);
	bus.drawInstanceQueue(busPos, cam, *cubeMap);

	for (auto& iter : sMenu.buttonVec)
		iter->draw(window, *programManager);

	
	sMenu.textTitle->draw(window, textProg, glm::vec3(
		abs(cosf(secs * 2.0f)),
		abs(cosf(secs * 2.0f + glm::radians(120.f))),
		abs(cosf(secs * 2.0f + glm::radians(240.f)))
	));
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

	static Text textCreatedBy = Text("created by",48,glm::ivec2(20,800));
	static Text textHannes = Text("Hannes H.",64,glm::ivec2(20,856));
	static Text textEhrenMann = Text("HCKOKSINVADERS V1.0",24,glm::ivec2(20,774));

	static Camera cam;
	cam.setCameraPos(glm::vec3(sinf(secs)*0.25f, cos(secs)*0.25f, 1.0f));
	cam.setCameraFront(glm::vec3(0.0f + sinf(secs) * 0.05, 0.0f + cosf(secs) * 0.05, -1.0f));
	cam.setCameraUp(glm::vec3(0.0f, 1.0f, 0.0f));
	cam.setProjectionMatrix(glm::perspective(glm::radians(65.f), 640.f / 960.f, 1.f, 500.f));

	starBkg.draw(programManager->get(ProgramManager::ProgramEntry::MainMenuBackground), secs*3.0);

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

	for (auto& iter : sCredits.buttonVec)
		iter->draw(window, *programManager);
}

void Game::drawGameOverScreen() {
	static StarBackground starBkg;
	static const Model3D& bus = modelManager->getModel("res/models/vengabus.obj");
	static InstanceBuffer busPos(1);
	static Camera cam;

	const float secs = m_gameClock.getElapsedTime().asSeconds();

	cam.setCameraPos(glm::vec3(0.0f,10.0f,10.0f));
	cam.setCameraFront(glm::vec3(0.0f, -0.5f, -1.0f));
	cam.setCameraUp(glm::vec3(0.0f, 1.0f, 0.0f));
	cam.setProjectionMatrix(glm::perspective(glm::radians(65.f), 640.f / 960.f, 1.f, 500.f));	

	busPos[0] = std::move(ModelPosition(
		glm::vec3(0.0f,0.0f,0.0f),
		secs,glm::vec3(0.000001f,1.0f,0.000001f),
		glm::vec3(0.75f,0.75f,0.75f)
	));
	busPos.transferToGpu();

	starBkg.draw(programManager->get(ProgramManager::ProgramEntry::MainMenuBackground),secs);

	bus.drawInstanceQueue(busPos, cam, *cubeMap);
}

// struct funcs
void Game::__sIngame::updateBullets(float deltaTime) {
	for (auto& iter : bullets) {
		iter.update(deltaTime);
	}
}