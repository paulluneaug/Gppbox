#pragma once
#include <vector>
#include "Weapon.h"
#include "Projectile.h"
#include "Timer.h"

class Rifle : public Weapon
{
private:
	std::vector<Projectile*> m_shotProjectiles;
	Timer m_reloadTimer;

public:
	Rifle(Game& r_game, float reloadTime);

	void Update(float deltaTime, float posX, float posY, float dirX, float dirY) override;
	void OnWeaponSelected() override;
	void OnWeaponUnselected() override;

	virtual void Draw(sf::RenderWindow& r_window) override;
};

