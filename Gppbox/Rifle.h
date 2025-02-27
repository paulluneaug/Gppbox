#pragma once
#include <vector>
#include "Weapon.h"
#include "Projectile.h"
#include "Timer.h"
#include "C.hpp"
#include "Utils.h"

class Rifle : public Weapon
{
	typedef Weapon Super;
private:
	static constexpr float KNOCKBACK = 0.1f * Consts::GRID_SIZE;
	static constexpr float DISPERSION = 5.0f * Utils::DEG_2_RAD;

public:
	Rifle(Game& r_game, float reloadTime);

	virtual void Draw(sf::RenderWindow& r_window) override;

	virtual Vector2f GetKnockback() override;

protected:

	virtual bool TryShoot(float deltaTime, float posX, float posY, float dirX, float dirY) override;
	virtual sf::Shape* CreateSprite() override;
};

