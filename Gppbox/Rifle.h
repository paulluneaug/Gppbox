#pragma once
#include <vector>
#include "Weapon.h"
#include "Projectile.h"

class Rifle : public Weapon
{
private:
	std::vector<Projectile*> m_shotProjectiles;
public:
	void Update(float deltaTime, float posX, float posY, float dirX, float dirY) override;

};

