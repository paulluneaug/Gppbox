#include "Projectile.h"
#include "Utils.h"
#include "C.hpp"
#include "Entity.h"

Projectile::Projectile(Game& r_game, Vector2f position, Vector2f direction, bool canCollideWithWalls) :
	m_game(r_game),
	m_position(position),
	m_angle(Utils::SignedAngle(direction, { 1.0f, 0.0f })),
	m_speed(Utils::Magnitude(direction) * DEFAULT_SPEED),
	IsAlive(true),
	m_damage(DEFAULT_DAMAGE),
	m_canCollideWithWalls(canCollideWithWalls),
	m_currentLifetime(0.0f)
{
	float radius = 0.2f * Consts::GRID_SIZE;
	m_sprite = new sf::CircleShape(radius);
	m_sprite->setOrigin({ radius , radius });
}

Projectile::~Projectile()
{
	delete m_sprite;
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

	sf::Vector2f direction =
	{
		cos(m_angle),
		sin(m_angle)
	};

	bool collided = false;

	for (std::array<float, 2>& hitPoint : Utils::Raycast(m_position, direction, deltaTime * m_speed / Consts::GRID_SIZE, { Consts::GRID_SIZE,Consts::GRID_SIZE }))
	{
		if (CheckCollisions(hitPoint[0] * Consts::GRID_SIZE, hitPoint[1] * Consts::GRID_SIZE)) 
		{
			collided = true;
			break;
		}
	}


	if (!collided) 
	{

		sf::Vector2f offset =
		{
			direction.x * deltaTime * m_speed,
			direction.y * deltaTime * m_speed
		};
		m_position += offset;
		collided |= CheckCollisions(m_position);
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

bool Projectile::CollidesWithWall(float x, float y)
{
	return m_game.IsWall(int(x / Consts::GRID_SIZE), int(y / Consts::GRID_SIZE));
}

bool Projectile::CollidesWithEnemy(float x, float y, Entity** o_hitEnemy)
{
	return m_game.CollidesWithEnemyAtPoint(x, y, o_hitEnemy);
}

bool Projectile::CheckCollisions(float x, float y)
{
	if (m_canCollideWithWalls && CollidesWithWall(x, y))
	{
		return true;
	}

	Entity* hitEnemy = nullptr;
	if (CollidesWithEnemy(x, y, &hitEnemy))
	{
		hitEnemy->TakeDamage(m_damage);
		return true;
	}
	return false;
}

bool Projectile::CheckCollisions(const Vector2f& position)
{
	return CheckCollisions(position.x, position.y);
}
