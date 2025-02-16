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

bool Weapon::Update(float deltaTime, float posX, float posY, float dirX, float dirY)
{
	bool shot = TryShoot(deltaTime, posX, posY, dirX, dirY);

	for (auto iterator = m_shotProjectiles.begin(); iterator != m_shotProjectiles.end();)
	{
		Projectile* projectile = *iterator;
		if (projectile->IsAlive)
		{
			projectile->Update(deltaTime);
			++iterator;
		}
		else
		{
			delete projectile;
			iterator = m_shotProjectiles.erase(iterator);
		}
	}
	return false;
}

void Weapon::OnWeaponSelected()
{
	m_reloadTimer.Start();
}

void Weapon::OnWeaponUnselected()
{
	m_reloadTimer.Stop();
}

void Weapon::Draw(sf::RenderWindow& r_window)
{
	for (Projectile* projectile : m_shotProjectiles)
	{
		if (projectile->IsAlive)
		{
			projectile->Draw(r_window);
		}
	}
}
