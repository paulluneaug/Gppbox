#include "Weapon.h"

Weapon::Weapon(Game& r_game) :
	m_game(r_game)
{
}

void Weapon::SetShouldShoot(bool shootState)
{
	m_shouldShoot = shootState;
}
