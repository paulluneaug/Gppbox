#pragma once
#include "Weapon.h"
#include "Projectile.h"
#include "C.hpp"

class Drone : public Weapon
{
	typedef Weapon Super;

private:
	static constexpr float MAX_RANGE = 20.0f * Consts::GRID_SIZE;
	static constexpr float TARGET_DISTANCE_FROM_PLAYER = 4.0f * Consts::GRID_SIZE;
	static constexpr float LERP_HALF_TIME = 1.0f;

private:
	sf::Shape* m_sprite;

	Vector2f m_position;
	std::vector<Projectile*> m_shotProjectiles;

public:
	Drone(Game& r_game, float reloadTime, Vector2f startPosition);
	virtual ~Drone();

	virtual bool Update(float deltaTime, float posX, float posY, float dirX, float dirY) override;
	virtual void Draw(sf::RenderWindow& r_window) override;

	virtual Vector2f GetKnockback() override;

private:
	bool Shoot();
};

