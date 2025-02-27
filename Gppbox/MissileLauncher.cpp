#include "MissileLauncher.h"
MissileLauncher::MissileLauncher(Game& r_game, float reloadTime) :
	Super(r_game, reloadTime, false, {0.6f * Consts::GRID_SIZE, 0.0f * Consts::GRID_SIZE })
{
}

bool MissileLauncher::TryShoot(float deltaTime, float posX, float posY, float dirX, float dirY)
{
	if (m_shouldShoot && m_reloadTimer.Update(deltaTime))
	{
		Vector2f firePosition = GetFirePosition(posX, posY, dirX, dirY);

		Entity* target = m_game.GetClosestEnemy(posX, posY);
		HomingMissile* newProjectile = new HomingMissile(m_game, firePosition, { dirX, dirY }, target);
		m_shotProjectiles.push_back(newProjectile);

		m_muzzleFireTimer.Start();
		m_muzzleFireSprite->setPosition(firePosition);
		m_game.GetCamera().ScreenShake(0.2f, 4.0f);
		return true;
	}
	return false;
}

sf::Shape* MissileLauncher::CreateSprite()
{
	sf::Shape* sprite = new sf::RectangleShape({ 1.0f * Consts::GRID_SIZE, 0.5f * Consts::GRID_SIZE });
	sprite->setFillColor(Color{ 255u, 102u, 0u, 255u });
	sprite->setOrigin(0.2f * Consts::GRID_SIZE, 0.25f * Consts::GRID_SIZE);
	return sprite;
}

Vector2f MissileLauncher::GetKnockback()
{
	return { KNOCKBACK, 0.0f };
}
