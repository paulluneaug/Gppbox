#include "Drone.h"
#include "Entity.h"
#include "Utils.h"

Drone::Drone(Game& r_game, float reloadTime, Vector2f startPosition) :
	Super(r_game, reloadTime, true, { 0.0f, 0.0f }),
	m_position(startPosition)
{
}

Drone::~Drone()
{
}

bool Drone::Update(float deltaTime, float posX, float posY, float dirX, float dirY)
{
	bool shot = Super::Update(deltaTime, posX, posY, dirX, dirY);

	Vector2f toPlayer = Utils::Normalize({ posX - m_position.x, posY - m_position.y });
	Vector2f targetPosition = Vector2f{ posX, posY } - toPlayer * TARGET_DISTANCE_FROM_PLAYER;

	Vector2f newTargetPosition = Utils::SmoothLerp(m_position, targetPosition, deltaTime, LERP_HALF_TIME);
	Vector2f offset = newTargetPosition - m_position;


	for (std::array<float, 2>&hitPoint : Utils::Raycast(m_position, offset, 1.0f, { Consts::GRID_SIZE,Consts::GRID_SIZE }))
	{
		if (m_game.IsWall(hitPoint[0], hitPoint[1]))
		{
			offset = { 0.0f, 0.0f };
			break;
		}
	}

	for (std::array<float, 2>&hitPoint : Utils::Raycast(m_position, { Utils::Sign(offset.x), 0 }, 1.0f, { Consts::GRID_SIZE,Consts::GRID_SIZE }))
	{
		if (m_game.IsWall(hitPoint[0], hitPoint[1]))
		{
			offset.x = 0;
			break;
		}
	}

	for (std::array<float, 2>&hitPoint : Utils::Raycast(m_position, { 0, Utils::Sign(offset.y)}, 1.0f, { Consts::GRID_SIZE,Consts::GRID_SIZE }))
	{
		if (m_game.IsWall(hitPoint[0], hitPoint[1]))
		{
			offset.y = 0;
			break;
		}
	}
	m_position += offset;


	m_weaponSprite->setPosition(m_position);

	return shot;
}

bool Drone::TryShoot(float deltaTime, float posX, float posY, float dirX, float dirY)
{
	if (!m_selected)
	{
		return false;
	}

	if (!m_reloadTimer.IsRunning())
	{
		m_reloadTimer.Start();
	}
	else if (!m_reloadTimer.Update(deltaTime))
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

	//m_muzzleFireTimer.Start();
	//m_muzzleFireSprite->setPosition(posX, posY);

	return true;
}

sf::Shape* Drone::CreateSprite()
{
	float spriteSize = 0.5f * Consts::GRID_SIZE;
	sf::Shape* sprite = new RectangleShape({ spriteSize , spriteSize });
	sprite->setOrigin({ spriteSize / 2 , spriteSize / 2 });
	return sprite;
}

Vector2f Drone::GetKnockback()
{
	return { 0.0f, 0.0f };
}

void Drone::SetPosition(Vector2f newPosition)
{
	m_position = newPosition;
}

//sf::Shape* Drone::CreateSprite()
//{
//	float spriteSize = 0.5f * Consts::GRID_SIZE;
//	sf::Shape* sprite = new RectangleShape({ spriteSize , spriteSize });
//	sprite->setOrigin({ spriteSize / 2 , spriteSize / 2 });
//	return sprite;
//}

void Drone::UpdateSpritePosition(float posX, float posY, float dirX, float dirY)
{
	m_weaponSprite->setPosition(m_position);
}
