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

sf::Font* m_fpsTextFont;
sf::Text* m_fpsText;

void Game::init() {
	// Init window with Context
	sf::ContextSettings cs(24,8,2,4,3,0U,false);
	window.create(sf::VideoMode(640, 960), "HcKoksInvaders", sf::Style::Close,cs);
	window.setActive(true);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	// fps counter
	m_fpsTextFont = new sf::Font();
	m_fpsTextFont->loadFromFile("res/fonts/PressStart2P-Regular.ttf");
	m_fpsText = new sf::Text("awd",*m_fpsTextFont,12);
	m_fpsText->setFillColor(sf::Color::Magenta);
	m_fpsText->setPosition(sf::Vector2f(10,10));

	sf::RenderTexture tx;
	//tx.getTexture().

	// Init View Matrix
	glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	
	matView = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
	matPerspective = glm::perspective(1.f, (float)window.getSize().x/(float)window.getSize().y, 0.01f, 100.f);

	// set OpenGL Function Ptrs
	glewExperimental = GL_TRUE;
	glewInit();

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
		ingameMunitionIconPistol = new AmmunitionIcon(
			"res/images/icon_munition_pistol.png",
			sf::Color::Blue,
			0.1f,
			sf::Vector2f(-0.85f, 0.9f),
			*textureManager,
			window
		);
		ingameMunitionIconSMG = new AmmunitionIcon(
			"res/images/icon_munition_smg.png",
			sf::Color::Yellow,
			0.1f,
			sf::Vector2f(-0.6f, 0.9f),
			*textureManager,
			window
		);
		ingameMunitionIconRocket = new AmmunitionIcon(
			"res/images/icon_munition_rocket.png",
			sf::Color::Green,
			0.1f,
			sf::Vector2f(-0.35f, 0.9f),
			*textureManager,
			window
		);
		ingameMunitionIconShotgun = new AmmunitionIcon(
			"res/images/icon_munition_shotgun.png",
			sf::Color::Red,
			0.1f,
			sf::Vector2f(-0.1f, 0.9f),
			*textureManager,
			window
		);
	}
}

void handleButtons_MouseMoved(const std::vector<Button> &buttons,
							  const sf::Event::MouseMoveEvent mouseMoveEvent) 
{
	std::cout << "Left Mouse moved\n";
	for (auto& button : buttons) {
		if (button.onHoverActive == false) {
			if (button.containsPoint(sf::Vector2f(mouseMoveEvent.x, mouseMoveEvent.y))) {
				button.onHover(button);
			}
		}
	}
}
void handleButtons_MouseLeftClicked(const std::vector<Button>& buttons,
									const sf::Event::MouseButtonEvent& mouseButtonEvent) 
{
	for (auto& button : buttons) {
		if (button.containsPoint(sf::Vector2f(mouseButtonEvent.x, mouseButtonEvent.y))) {
			std::cout << "Left Mouse clicked\n";
			button.onClick(button);
		}
	}
}

void Game::run() {
	GameState gameState = GameState::MainMenu;

	GameWorld gameWorld(this,modelManager);
	gameWorld.init(256, 2);
	gameWorld.saveToFileAsImage("demo.bmp");

	BulletRenderer br(*programManager);
	std::vector<Bullet> bullets(1000);
	for (int i = 0; i < 1000;i++) {
		bullets[i].m_pos.x = std::cosf(i);
		bullets[i].m_pos.y = std::sinf(i);
		bullets[i].m_pos.z = std::cosf(i*0.33f);
	}

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

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				this->exit();
				break;
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

			if (gameState == GameState::MainMenu) {
				if (event.type == sf::Event::MouseMoved) {
					//handleButtons_MouseMoved(std::vector<Button>{button}, event.mouseMove);
				}
				if (event.type == sf::Event::MouseButtonPressed) {
					if (event.mouseButton.button == sf::Mouse::Left) {
						//handleButtons_MouseLeftClicked(std::vector<Button>{button}, event.mouseButton);
					}
				}
			}

			if (gameState == GameState::Ingame) {
				if (event.type == sf::Event::KeyPressed) {

				}
			}
		}

		glClearColor(0.5, 0.75, 0.25, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		programManager->get(ProgramManager::ProgramEntry::Model3D).setUniform("matProgressCubemap", glm::rotate(glm::identity<glm::mat4>(), gameClock.getElapsedTime().asSeconds() * 2, glm::vec3(0.1, 0.1, 1.0)));

		//drawCredits();

		cam1.setCameraPos(glm::vec3(0.0f,m_gameClock.getElapsedTime().asSeconds(),4.5f));
		gameWorld.draw(cam1,*cubeMap);

		//br.drawInstances(bullets,cam);
		frametimes.push_back(fpsClock.getElapsedTime().asMicroseconds());
		fpsClock.restart();

		const Program& prog = programManager->get(ProgramManager::ProgramEntry::AmmunitionIcon);
		ingameMunitionIconPistol->draw(50.f, prog);
		ingameMunitionIconSMG->draw(50.f, prog);
		ingameMunitionIconRocket->draw(50.f, prog);
		ingameMunitionIconShotgun->draw(50.f, prog);

		window.display();
	}
}

void Game::exit() {
	window.close();
}

// private functions
void Game::drawFpsCounter(sf::Time timeElapsed) {
	
}

void Game::drawMainMenu() {
	
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
}