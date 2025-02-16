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

private:
	std::vector<HomingMissile*> m_shotMissiles;

public:
	MissileLauncher(Game& r_game, float reloadTime);

	bool Update(float deltaTime, float posX, float posY, float dirX, float dirY) override;

	virtual void Draw(sf::RenderWindow& r_window) override;

	virtual Vector2f GetKnockback() override;
};

