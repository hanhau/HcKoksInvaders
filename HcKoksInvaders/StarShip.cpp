#include "include/StarShip.hpp"

const int StarShip::maxAmmoSMG = 100;
const int StarShip::maxAmmoShotgun = 100;
const int StarShip::maxRocketAmmo = 3;

StarShip::StarShip(Game* const game) :
	m_gamePtr(game)
{

}

