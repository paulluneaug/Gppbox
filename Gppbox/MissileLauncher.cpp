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

		m_muzzleFireTimer.Start();
		m_muzzleFireSprite->setPosition(posX, posY);
		m_game.GetCamera().ScreenShake(0.2f, 5.0f);
		return true;
	}
	return false;
}

sf::Shape* MissileLauncher::CreateSprite()
{
	sf::Shape* sprite = new sf::RectangleShape({ 0.9f * Consts::GRID_SIZE, 0.4f * Consts::GRID_SIZE });
	sprite->setFillColor(Color{ 255u, 102u, 0u, 255u });
	sprite->setOrigin(0.2f * Consts::GRID_SIZE, 0.2f * Consts::GRID_SIZE);
	return sprite;
}

void MissileLauncher::Draw(sf::RenderWindow& r_window)
{
	if (m_selected) 
	{
		Super::Draw(r_window);
	}
}

Vector2f MissileLauncher::GetKnockback()
{
	return { KNOCKBACK, 0.0f };
}
