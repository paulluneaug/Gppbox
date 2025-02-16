#include "Projectile.h"
#include "Utils.h"
#include "C.hpp"
#include "Entity.h"

Projectile::Projectile(Game& r_game, Vector2f position, Vector2f direction, bool canCollideWithWalls) :
	m_game(r_game),
	m_position(position),
	m_angle(Utils::Angle(direction, { 1.0f, 0.0f })),
	m_speed(Utils::Magnitude(direction) * DEFAULT_SPEED),
	IsAlive(true),
	m_damage(DEFAULT_DAMAGE),
	m_canCollideWithWalls(canCollideWithWalls),
	m_currentLifetime(0.0f)
{
	float radius = 0.2f;
	m_sprite = new sf::CircleShape(radius * Consts::GRID_SIZE);
	m_sprite->setOrigin({ radius * Consts::GRID_SIZE , radius * Consts::GRID_SIZE });
}

void Projectile::Update(float deltaTime)
{
	if (!IsAlive) 
	{
		return;
	}

	m_currentLifetime += deltaTime;

	if (m_currentLifetime >= MAX_LIFETIME) 
	{
		IsAlive = false;
		return;
	}

	UpdateVelocity(deltaTime);

	m_speed = std::clamp(m_speed, 0.0f, m_maxSpeed);

	sf::Vector2f offset = 
	{ 
		cos(m_angle) * deltaTime * m_speed,
		sin(m_angle) * deltaTime * m_speed 
	};

	bool collided = false;

	if (Utils::SqrMagnitude(offset) > 1.0f) 
	{
		sf::Vector2f step = Utils::Normalize(offset);
		while (Utils::SqrMagnitude(offset) > 1.0f && !collided)
		{
			offset -= step;
			m_position += step;
			collided |= CheckCollisions();
		}
	}

	if (!collided) 
	{
		m_position += offset;
		collided |= CheckCollisions();
	}

	if (collided)
	{
		IsAlive = false;
	}

}

void Projectile::Draw(sf::RenderWindow& r_window)
{
	if (!IsAlive)
	{
		return;
	}
	m_sprite->setPosition(m_position);
	m_sprite->setRotation(m_angle);
	r_window.draw(*m_sprite);
}

void Projectile::UpdateVelocity(float deltaTime)
{
}

bool Projectile::CollidesWithWall()
{
	return m_game.IsWall(int(m_position.x / Consts::GRID_SIZE), int(m_position.y / Consts::GRID_SIZE));
}

bool Projectile::CollidesWithEnemy(Entity** o_hitEnemy)
{
	return m_game.CollidesWithEnemyAtPoint(m_position.x, m_position.y, o_hitEnemy);
}

bool Projectile::CheckCollisions()
{
	if (m_canCollideWithWalls && CollidesWithWall())
	{
		return true;
	}

	Entity* hitEnemy = nullptr;
	if (CollidesWithEnemy(&hitEnemy))
	{
		hitEnemy->TakeDamage(m_damage);
		return true;
	}
	return false;
}
