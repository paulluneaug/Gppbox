#include "Random.h"
#include "Rifle.h"

Rifle::Rifle(Game& r_game, float reloadTime) :
	Super(r_game, reloadTime, false, { 0.4f * Consts::GRID_SIZE, 0.0f * Consts::GRID_SIZE })
{
}

bool Rifle::TryShoot(float deltaTime, float posX, float posY, float dirX, float dirY)
{
	if (m_shouldShoot && m_reloadTimer.Update(deltaTime))
	{
		float dispersionAngle = Random::RandomFloat(-DISPERSION, DISPERSION);
		float shootAngle = Utils::SignedAngle({ 1, 0 }, { dirX, dirY }) + dispersionAngle;
		Vector2f direction =
		{
			cos(shootAngle),
			sin(shootAngle)
		};

		Vector2f firePosition = GetFirePosition(posX, posY, dirX, dirY);

		Projectile* newProjectile = new Projectile(m_game, firePosition, direction);
		m_shotProjectiles.push_back(newProjectile);

		m_muzzleFireTimer.Start();
		m_muzzleFireSprite->setPosition(firePosition);

		m_game.GetCamera().ScreenShake(0.1f, 2.0f);

		return true;
	}
	return false;
}

sf::Shape* Rifle::CreateSprite()
{
	sf::Shape* sprite = new sf::RectangleShape({ 0.6f * Consts::GRID_SIZE, 0.4f * Consts::GRID_SIZE });
	sprite->setFillColor(Color{ 255u, 102u, 0u, 255u });
	sprite->setOrigin(0.2f * Consts::GRID_SIZE, 0.2f * Consts::GRID_SIZE);
	return sprite;
}

Vector2f Rifle::GetKnockback()
{
	return { KNOCKBACK, 0.0f };
}

//sf::Shape* Rifle::CreateSprite()
//{
//}
