#include "Enemy.h"
#include <cstdlib>


EnemyEntity::EnemyEntity(Game& r_game) : Entity(r_game, { 1, 1 })
{
	m_goingRight = std::rand() > (RAND_MAX / 2);
}

void EnemyEntity::UpdatePosition(float deltaTime)
{
	if (!IsAlive()) 
	{
		return;
	}

	UpdateDirection(deltaTime);
	Entity::UpdatePosition(deltaTime);
}

void EnemyEntity::UpdateDirection(float deltaTime)
{
	if (m_goingRight && CollidesRight(deltaTime) && Rx > 0.9f)
	{
		m_goingRight = false;
	}
	else if (!m_goingRight && CollidesLeft(deltaTime) && Rx < 0.1f)
	{
		m_goingRight = true;
	}

	Input.x = m_goingRight ? 1 : -1;
}
