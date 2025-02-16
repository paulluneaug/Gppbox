#pragma once
#include <vector>
#include "Weapon.h"
#include "Projectile.h"
#include "Timer.h"
#include "C.hpp"

class Rifle : public Weapon
{
	typedef Weapon Super;
private:
	static constexpr float KNOCKBACK = 0.1f * Consts::GRID_SIZE;

public:
	Rifle(Game& r_game, float reloadTime);

	virtual bool TryShoot(float deltaTime, float posX, float posY, float dirX, float dirY) override;

	virtual void Draw(sf::RenderWindow& r_window) override;

	virtual Vector2f GetKnockback() override;
};

