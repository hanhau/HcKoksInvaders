#include "include/StarShip.hpp"
#include "include/Model.hpp"

const int StarShip::maxAmmoSMG = 100;
const int StarShip::maxAmmoShotgun = 100;
const int StarShip::maxAmmoRocket = 3;

const float StarShip::coolDownPistol = 0.2f;
const float StarShip::coolDownRocket = 5.0f;
const float StarShip::coolDownShotgun = 0.5f;
const float StarShip::coolDownSMG = 0.10f;

StarShip::StarShip(Game& game) :
	m_gameRef(game),
	m_ammoSMG(0),
	m_ammoShotgun(0),
	m_ammoRocket(0),
	m_pos(glm::vec3(0.0f)),
	m_activeWeapon(WeaponType::Pistol)
{
	m_clockPistol.restart();
	m_clockRocket.restart();
	m_clockShotgun.restart();
	m_clockSMG.restart();
}

void StarShip::updateOnUserInput(float deltaSeconds) {
	const float _minx = -1.0f;
	const float _miny = 1.0f;
	const float _distPerSecond = 1.0f;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) {
		m_pos.x -= (_distPerSecond * deltaSeconds);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) {
		m_pos.x += (_distPerSecond * deltaSeconds);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) {
		processShoot();
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num1)) {
		m_activeWeapon = WeaponType::Pistol;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num2)) {
		m_activeWeapon = WeaponType::Shotgun;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num3)) {
		m_activeWeapon = WeaponType::SMG;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num4)) {
		m_activeWeapon = WeaponType::Rocket;
	}

	m_pos.x = std::clamp(m_pos.x, _minx, _miny);
}

// Add Ammunition
void StarShip::addSMGAmmo(int shots) {
	m_ammoSMG = std::clamp(m_ammoSMG + shots, 0, maxAmmoSMG);
}
void StarShip::addShotgunAmmo(int shots) {
	m_ammoShotgun = std::clamp(m_ammoShotgun + shots, 0, maxAmmoShotgun);
}
void StarShip::addRocketAmmo(int shots) {
	m_ammoRocket = std::clamp(m_ammoRocket + shots, 0, maxAmmoRocket);
}

// Get Ammo Percentages
const float _getAmmoPercent(int val, int max) {
	if (val == 0)
		return 0.0f;
	else 
		return std::roundf(((float)val / (float)max) * 100.f);
}

const float StarShip::getSMGAmmoPercent() {
	return _getAmmoPercent(m_ammoSMG, maxAmmoSMG);
}
const float StarShip::getShotgunAmmoPercent() {
	return _getAmmoPercent(m_ammoShotgun, maxAmmoShotgun);
}
const float StarShip::getRocketAmmoPercent() {
	return _getAmmoPercent(m_ammoRocket, maxAmmoRocket);
}

const glm::vec3& StarShip::getPos() {
	return m_pos;
}
void StarShip::setPos(const glm::vec3 pos) {
	m_pos = pos;
}

void StarShip::draw(const Camera& camera, Cubemap& cubemap) {
	static InstanceBuffer shipPos(1);
	static const Model3D& ship = m_gameRef.getModelManager().getModel("res/models/ship1.obj");

	shipPos[0] = ModelPosition(
		m_pos,
		glm::radians(0.0f),glm::vec3(0.f),
		glm::vec3(0.75f)
	);
	shipPos.transferToGpu();
	ship.drawInstanceQueue(shipPos, camera, cubemap);
}

void StarShip::processShoot() {
	const glm::vec3 bulletStartPos = m_pos + glm::vec3(0.0f, 0.1f, 0.0f);

	switch (m_activeWeapon) {
		case WeaponType::Pistol:
			if (m_clockPistol.getElapsedTime().asSeconds() >= coolDownPistol) {
				m_clockPistol.restart();

				m_gameRef.addBullet(Bullet{
					bulletStartPos,
					glm::vec3(0.f,2.0f,0.f),
					20.f,
					Bullet::Owner::Player
				});
			}
			break;
		case WeaponType::Rocket:
			if (m_ammoRocket != 0) {
				if (m_clockRocket.getElapsedTime().asSeconds() >= coolDownRocket) {
					m_clockRocket.restart();

					m_gameRef.addBullet(Bullet{
						bulletStartPos,
						glm::vec3(0.f,1.2f,0.f),
						100.f,
						Bullet::Owner::Player
					});

					m_ammoRocket--;
				}
			}
			break;
		case WeaponType::Shotgun:
			if (m_ammoShotgun != 0) {
				if (m_clockShotgun.getElapsedTime().asSeconds() >= coolDownShotgun) {
					m_clockShotgun.restart();

					m_ammoShotgun--;
				}
			}
			break;
		case WeaponType::SMG:
			if (m_ammoSMG != 0) {
				if (m_clockSMG.getElapsedTime().asSeconds() >= coolDownSMG) {
					m_clockSMG.restart();

					m_gameRef.addBullet(Bullet{
						bulletStartPos,
						glm::vec3(0.f,2.2f,0.f),
						16.f,
						Bullet::Owner::Player
					});

					m_ammoSMG--;
				}
			}
			break;
		default:
			throw "Invalid WeaponType";
			break;
	}
}