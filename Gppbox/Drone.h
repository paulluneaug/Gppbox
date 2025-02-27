#pragma once
#include "C.hpp"
#include "Projectile.h"
#include "Weapon.h"

class Drone : public Weapon
{
	typedef Weapon Super;

private:
	static constexpr float MAX_RANGE = 20.0f * Consts::GRID_SIZE;
	static constexpr float TARGET_DISTANCE_FROM_PLAYER = 4.0f * Consts::GRID_SIZE;
	static constexpr float LERP_HALF_TIME = 0.3f;

private:
	Vector2f m_position;

public:
	Drone(Game& r_game, float reloadTime, Vector2f startPosition);
	virtual ~Drone();

	virtual bool Update(float deltaTime, float posX, float posY, float dirX, float dirY) override;

	virtual Vector2f GetKnockback() override;

protected:
	virtual bool TryShoot(float deltaTime, float posX, float posY, float dirX, float dirY) override;
	virtual sf::Shape* CreateSprite() override;
	void UpdateSpritePosition(float posX, float posY, float dirX, float dirY) override;

};

