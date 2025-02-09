#include "Projectile.h"
#include "Utils.h"
#include "C.hpp"
#include "Entity.h"

Projectile::Projectile(Game& r_game, Vector2f position, Vector2f direction) :
	m_game(r_game),
	m_position(position),
	m_velocity(direction),
	IsAlive(true),
	m_damage(DEFAULT_DAMAGE)
{
	m_sprite = new sf::CircleShape(0.4f * Consts::GRID_SIZE);
}

void Projectile::Update(float deltaTime)
{
	if (!IsAlive) 
	{
		return;
	}

	UpdateVelocity(deltaTime);

	Vector2fUtils::ClampMagnitude(m_velocity, 0, m_maxSpeed);

	m_position += m_velocity * deltaTime;

	if (CollidesWithWall()) 
	{
		IsAlive = false;
	}

	Entity* hitEnemy = nullptr;
	if (CollidesWithEnemy(hitEnemy))
	{
		hitEnemy->TakeDamage(m_damage);
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
	m_sprite->setRotation(Vector2fUtils::Angle(m_velocity, { 1, 0 }));
	r_window.draw(*m_sprite);
}

void Projectile::UpdateVelocity(float deltaTime)
{
}

bool Projectile::CollidesWithWall()
{
	m_game.IsWall(floor(m_position.x), floor(m_position.y));
}

bool Projectile::CollidesWithEnemy(Entity* o_hitEnemy)
{
	return m_game.CollidesWithEnemyAtPoint(m_position.x, m_position.y, o_hitEnemy);
}
