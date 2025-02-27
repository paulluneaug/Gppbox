#include "Random.h"
#include "Rifle.h"

Rifle::Rifle(Game& r_game, float reloadTime) :
	Super(r_game, reloadTime)
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

		Projectile* newProjectile = new Projectile(m_game, { posX, posY }, direction);
		m_shotProjectiles.push_back(newProjectile);

		m_muzzleFireTimer.Start();
		m_muzzleFireSprite->setPosition(posX, posY);
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

void Rifle::Draw(sf::RenderWindow& r_window)
{
	if (m_selected)
	{
		Super::Draw(r_window);
	}
}

Vector2f Rifle::GetKnockback()
{
	return { KNOCKBACK, 0.0f };
}

//sf::Shape* Rifle::CreateSprite()
//{
//}
