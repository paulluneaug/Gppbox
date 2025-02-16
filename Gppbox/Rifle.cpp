#include "Rifle.h"

Rifle::Rifle(Game& r_game, float reloadTime) : 
	Super(r_game, reloadTime)
{
}

bool Rifle::TryShoot(float deltaTime, float posX, float posY, float dirX, float dirY)
{
	if (m_shouldShoot && m_reloadTimer.Update(deltaTime))
	{
		Projectile* newProjectile = new Projectile(m_game, { posX, posY }, { dirX, dirY });
		m_shotProjectiles.push_back(newProjectile);
		return true;
	}
	return false;
}

void Rifle::Draw(sf::RenderWindow& r_window)
{
	Super::Draw(r_window);
}

Vector2f Rifle::GetKnockback()
{
	return { KNOCKBACK, 0.0f };
}
