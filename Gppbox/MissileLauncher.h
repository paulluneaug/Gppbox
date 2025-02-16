#pragma once
#include "Weapon.h"
#include "HomingMissile.h"
#include "Timer.h"
#include "Entity.h"


class MissileLauncher : public Weapon
{
	typedef Weapon Super;

private:
	std::vector<HomingMissile*> m_shotMissiles;
	Timer m_reloadTimer;

public:
	MissileLauncher(Game& r_game, float reloadTime);

	void Update(float deltaTime, float posX, float posY, float dirX, float dirY) override;
	void OnWeaponSelected() override;
	void OnWeaponUnselected() override;

	virtual void Draw(sf::RenderWindow& r_window) override;
};

