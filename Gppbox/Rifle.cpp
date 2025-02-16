#include "Rifle.h"

Rifle::Rifle(Game& r_game, float reloadTime) : 
	Super(r_game, reloadTime)
{
}

bool Rifle::Update(float deltaTime, float posX, float posY, float dirX, float dirY)
{
	bool shot = false;
	if (m_shouldShoot && m_reloadTimer.Update(deltaTime))
	{
		Projectile* newProjectile = new Projectile(m_game, { posX, posY }, { dirX, dirY });
		m_shotProjectiles.push_back(newProjectile);
		shot = true;
	}

	// Only deletes one projectile per frame
	int projectileToDeleteIndex = -1;
	for (int iProj = 0; iProj < m_shotProjectiles.size(); ++iProj)
	{
		Projectile* projectile = m_shotProjectiles[iProj];
		if (projectile->IsAlive)
		{
			projectile->Update(deltaTime);
		}
		else
		{
			projectileToDeleteIndex = iProj;
		}
	}

	if (projectileToDeleteIndex != -1) 
	{
		Projectile* projectile = m_shotProjectiles[projectileToDeleteIndex];
		m_shotProjectiles.erase(m_shotProjectiles.begin() + projectileToDeleteIndex);
		delete projectile;
	}

	return shot;
}

void Rifle::Draw(sf::RenderWindow& r_window)
{
	for (Projectile* projectile : m_shotProjectiles) 
	{
		if (projectile->IsAlive) 
		{
			projectile->Draw(r_window);
		}
	}
}

Vector2f Rifle::GetKnockback()
{
	return { KNOCKBACK, 0.0f };
}
