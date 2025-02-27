#pragma once
#include "Weapon.h"
#include "HomingMissile.h"
#include "Timer.h"
#include "Entity.h"
#include "C.hpp"


class MissileLauncher : public Weapon
{
	typedef Weapon Super;

private:
	static constexpr float KNOCKBACK = 0.3f * Consts::GRID_SIZE;

public:
	MissileLauncher(Game& r_game, float reloadTime);


	virtual Vector2f GetKnockback() override;

protected:
	virtual bool TryShoot(float deltaTime, float posX, float posY, float dirX, float dirY) override;
	virtual sf::Shape* CreateSprite() override;
};

