#include "include/StarShip.hpp"
#include "include/Model.hpp"
#include "include/SoundQueue.hpp"

#include <deque>
#include <SFML/Audio/Sound.hpp>
#include <SFML/Window.hpp>

const int StarShip::maxAmmoSMG = 100;
const int StarShip::maxAmmoShotgun = 100;
const int StarShip::maxAmmoRocket = 3;

const float StarShip::coolDownPistol = 0.2f;
const float StarShip::coolDownRocket = 5.0f;
const float StarShip::coolDownShotgun = 0.5f;
const float StarShip::coolDownSMG = 0.10f;

const float StarShip::m_scale = 0.04f;

StarShip::StarShip(Game& game) :
	m_gameRef(game),
	m_ammoSMG(0),
	m_ammoShotgun(0),
	m_ammoRocket(0),
	m_pos(glm::vec3(0.0f)),
	m_activeWeapon(WeaponType::Pistol),
	m_health(100.f)
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
		m_activeWeapon = WeaponType::SMG;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num3)) {
		m_activeWeapon = WeaponType::Rocket;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num4)) {
		m_activeWeapon = WeaponType::Shotgun;
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

const float StarShip::getSMGAmmoPercent() const {
	return _getAmmoPercent(m_ammoSMG, maxAmmoSMG);
}
const float StarShip::getShotgunAmmoPercent() const {
	return _getAmmoPercent(m_ammoShotgun, maxAmmoShotgun);
}
const float StarShip::getRocketAmmoPercent() const {
	return _getAmmoPercent(m_ammoRocket, maxAmmoRocket);
}

void StarShip::setHealth(int health) {
	m_health = health;
}
const int StarShip::getHealth() {
	return m_health;
}

const glm::vec3& StarShip::getPos() const {
	return m_pos;
}
void StarShip::setPos(const glm::vec3 pos) {
	m_pos = pos;
}

WeaponType StarShip::getWeaponType() {
	return m_activeWeapon;
}

void StarShip::draw(const Camera& camera, Cubemap& cubemap) {
	static InstanceBuffer shipPos(1);
	static const Model3D& ship = ModelManager.getModel("res/models/vengabus.obj");

	shipPos[0] = ModelPosition(
		m_pos,
		glm::radians(90.f),glm::vec3(1.f,0.0001f,0.0001f),
		glm::vec3(m_scale)
	);
	shipPos.setInnerCount(1);
	shipPos.transferToGpu();
	ship.drawInstanceQueue(shipPos, camera, cubemap);
}

void StarShip::processShoot() {
	static const SoundBufferManager& sbm = m_gameRef.getSoundBufferManager();
	static const sf::SoundBuffer& soundBufPistol = sbm.get("res/audio/gunPistol.wav");
	static const sf::SoundBuffer& soundBufShotgun = sbm.get("res/audio/gunShotgun.wav");
	static const sf::SoundBuffer& soundBufSMG = sbm.get("res/audio/gunSMG.wav");
	static const sf::SoundBuffer& soundBufRocket = sbm.get("res/audio/gunSMG.wav");
	
	const glm::vec3 bulletStartPos = m_pos + glm::vec3(0.0f, 0.1f, 0.0f);

	switch (m_activeWeapon) {
		case WeaponType::Pistol:
			if (m_clockPistol.getElapsedTime().asSeconds() >= coolDownPistol) {
				m_clockPistol.restart();

				// Add Bullet
				m_gameRef.addBullet(Bullet{
					bulletStartPos,
					glm::vec3(0.f,3.5f,0.f),
					33.f,
					Bullet::Owner::Player
				});

				// Add Sound
				SoundQueue::add(soundBufPistol, 1.0f + (cosf(m_pos.y * 1000.f)) * 0.1f);
			}
			break;
		case WeaponType::Rocket:
			if (m_ammoRocket != 0) {
				if (m_clockRocket.getElapsedTime().asSeconds() >= coolDownRocket) {
					m_clockRocket.restart();

					// Add Bullet
					m_gameRef.addBullet(Bullet{
						bulletStartPos,
						glm::vec3(0.f,5.2f,0.f),
						100.f,
						Bullet::Owner::Player
					});

					// Subtract from Ammo
					m_ammoRocket--;
				}
			}
			break;
		case WeaponType::Shotgun:
			if (m_ammoShotgun != 0) {
				if (m_clockShotgun.getElapsedTime().asSeconds() >= coolDownShotgun) {
					m_clockShotgun.restart();

					// Add Bullet
					m_gameRef.addBullet(Bullet{
						bulletStartPos,
						glm::vec3(0.f,4.2f,0.f),
						50.f,
						Bullet::Owner::Player
						});
					m_gameRef.addBullet(Bullet{
						bulletStartPos,
						glm::vec3(0.2f,4.2f,0.f),
						50.f,
						Bullet::Owner::Player
						});
					m_gameRef.addBullet(Bullet{
						bulletStartPos,
						glm::vec3(-0.2f,4.2f,0.f),
						50.f,
						Bullet::Owner::Player
						});

					// Add Sound
					SoundQueue::add(soundBufShotgun, 1.0f + (cosf(m_pos.x * 1000.f)) * 0.1f);

					// Subtract from Ammo
					m_ammoShotgun--;
				}
			}
			break;
		case WeaponType::SMG:
			if (m_ammoSMG != 0) {
				if (m_clockSMG.getElapsedTime().asSeconds() >= coolDownSMG) {
					m_clockSMG.restart();

					// Add Bullet
					m_gameRef.addBullet(Bullet{
						bulletStartPos,
						glm::vec3(0.f,3.75f,0.f),
						30.f,
						Bullet::Owner::Player
					});

					// Add Sound
					SoundQueue::add(soundBufSMG, 1.0f + (cosf(m_pos.y * 234.3432f)) * 0.05f);

					// Subtract from Ammo
					m_ammoSMG--;
				}
			}
			break;
		default:
			throw "Invalid WeaponType";
			break;
	}
}

void StarShip::handleBullets(std::list<Bullet>& bullets) {
	static const Model3D& ship = ModelManager.getModel("res/models/vengabus.obj");
	static const BoundingBall bb = ship.getOuterBB();
	static const float shipRadius = bb.getRadius() * m_scale;

	std::list<Bullet>::iterator iter = bullets.begin();
	while (iter != bullets.end()) {
		if (iter->m_owner == Bullet::Owner::Player) {
			++iter;
			continue;
		}

		if (glm::distance(m_pos,iter->m_pos) <= shipRadius) {
			m_health -= iter->m_damage;

			SoundQueue::add(
				m_gameRef.getSoundBufferManager().get("res/audio/playershipHit.wav"),
				0.9f + cosf(m_pos.x*m_pos.y*1324.23f) * 0.1f
			);

			bullets.erase(iter++);
		}
		else {
			++iter;
		}
	}
}