#include "MissileLauncher.h"
MissileLauncher::MissileLauncher(Game& r_game, float reloadTime) :
	Super(r_game, reloadTime)
{
}

bool MissileLauncher::TryShoot(float deltaTime, float posX, float posY, float dirX, float dirY)
{
	if (m_shouldShoot && m_reloadTimer.Update(deltaTime))
	{
		Entity* target = m_game.GetClosestEnemy(posX, posY);
		HomingMissile* newProjectile = new HomingMissile(m_game, { posX, posY }, { dirX, dirY }, target);
		m_shotProjectiles.push_back(newProjectile);
		return true;
	}
	return false;
}

void MissileLauncher::Draw(sf::RenderWindow& r_window)
{
	Super::Draw(r_window);
}

Vector2f MissileLauncher::GetKnockback()
{
	return { KNOCKBACK, 0.0f };
}
