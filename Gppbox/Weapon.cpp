#include "Weapon.h"

Weapon::Weapon(Game& r_game, float reloadTime) :
	m_game(r_game),
	m_reloadTimer(Timer{ reloadTime,true })
{
}

void Weapon::SetShouldShoot(bool shootState)
{
	m_shouldShoot = shootState;
}

void Weapon::OnWeaponSelected()
{
	m_reloadTimer.Start();
}

void Weapon::OnWeaponUnselected()
{
	m_reloadTimer.Stop();
}
