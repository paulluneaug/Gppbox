#include "Drone.h"
#include "Entity.h"
#include "Utils.h"

Drone::Drone(Game& r_game, float reloadTime, Vector2f startPosition) :
	Super(r_game, reloadTime),
	m_position(startPosition)
{
	float spriteSize = 0.5 * Consts::GRID_SIZE;
	m_sprite = new RectangleShape({ spriteSize , spriteSize });
	m_sprite->setOrigin({ spriteSize / 2 , spriteSize / 2 });
}

Drone::~Drone()
{
	delete m_sprite;
}

bool Drone::Update(float deltaTime, float posX, float posY, float dirX, float dirY)
{
	Vector2f toPLayer = { posX - m_position.x, posY - m_position.y };
	Vector2f targetPosition = Vector2f{ posX, posY } - Utils::Normalize(toPLayer) * TARGET_DISTANCE_FROM_PLAYER;

	m_position = Utils::SmoothLerp(m_position, targetPosition, deltaTime, LERP_HALF_TIME);

	bool shot = false;
	if (m_reloadTimer.Update(deltaTime))
	{
		shot = Shoot();
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

void Drone::Draw(sf::RenderWindow& r_window)
{
	m_sprite->setPosition(m_position);
	r_window.draw(*m_sprite);

	for (Projectile* projectile : m_shotProjectiles)
	{
		if (projectile->IsAlive)
		{
			projectile->Draw(r_window);
		}
	}
}

Vector2f Drone::GetKnockback()
{
	return { 0.0f, 0.0f };
}

bool Drone::Shoot()
{
	Entity* target = m_game.GetClosestEnemy(m_position.x, m_position.y);
	if (target == nullptr || !target->IsAlive())
	{
		return false;
	}

	Vector2f targetCenter = target->GetCenter();
	Vector2f toTarget = targetCenter - m_position;
	toTarget.y = -toTarget.y;

	if (Utils::SqrMagnitude(toTarget) > MAX_RANGE * MAX_RANGE) 
	{
		return false;
	}

	Projectile* newProjectile = new Projectile(m_game, m_position, Utils::Normalize(toTarget));
	m_shotProjectiles.push_back(newProjectile);
	return true;
}
