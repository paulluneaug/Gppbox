#include "Rifle.h"

void Rifle::Update(float deltaTime, float posX, float posY, float dirX, float dirY)
{
	if (m_shouldShoot) 
	{
		Projectile* newProjectile = new Projectile(m_game, { posX, posY }, { dirX, dirY });
		m_shotProjectiles.push_back(newProjectile);
	}
	//m_shotProjectiles.
}
