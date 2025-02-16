#include "MissileLauncher.h"
MissileLauncher::MissileLauncher(Game& r_game, float reloadTime) :
	Super(r_game, reloadTime)
{
}

void MissileLauncher::Update(float deltaTime, float posX, float posY, float dirX, float dirY)
{
	if (m_shouldShoot && m_reloadTimer.Update(deltaTime))
	{
		Entity* target = m_game.GetClosestEnemy(posX, posY);
		HomingMissile* newProjectile = new HomingMissile(m_game, { posX, posY }, { dirX, dirY }, target);
		m_shotMissiles.push_back(newProjectile);
	}

	// Only deletes one projectile per frame
	int missileToDeleteIndex = -1;
	for (int iMissile = 0; iMissile < m_shotMissiles.size(); ++iMissile)
	{
		Projectile* missile = m_shotMissiles[iMissile];
		if (missile->IsAlive)
		{
			missile->Update(deltaTime);
		}
		else
		{
			missileToDeleteIndex = iMissile;
		}
	}

	if (missileToDeleteIndex > 0)
	{
		HomingMissile* missile = m_shotMissiles[missileToDeleteIndex];
		m_shotMissiles.erase(m_shotMissiles.begin() + missileToDeleteIndex);
		delete missile;
	}
}

void MissileLauncher::Draw(sf::RenderWindow& r_window)
{
	for (HomingMissile* missile : m_shotMissiles)
	{
		if (missile->IsAlive)
		{
			missile->Draw(r_window);
		}
	}
}
