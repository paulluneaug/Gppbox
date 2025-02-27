#include "Enemy.h"
#include "Random.h"


EnemyEntity::EnemyEntity(Game& r_game) : Entity(r_game, { 1, 1 }, 3.0f, 12.0f)
{
	m_goingRight = Random::RandomBoolProb(0.5f);
	m_sprite->setFillColor(Color::Red);
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
