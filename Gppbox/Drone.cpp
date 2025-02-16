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

	return Super::Update(deltaTime, posX, posY, dirX, dirY);
}

bool Drone::TryShoot(float deltaTime, float posX, float posY, float dirX, float dirY)
{
	if (!m_reloadTimer.Update(deltaTime)) 
	{
		return false;
	}

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

void Drone::Draw(sf::RenderWindow& r_window)
{
	Super::Draw(r_window);
	m_sprite->setPosition(m_position);
	r_window.draw(*m_sprite);

}

Vector2f Drone::GetKnockback()
{
	return { 0.0f, 0.0f };
}
