#pragma once
#include "Game.hpp"

class Weapon
{
protected:
	bool m_shouldShoot;
	Game& m_game;

public:
	Weapon(Game& r_game);

	void SetShouldShoot(bool shootState);
	virtual void Update(float deltaTime, float posX, float posY, float dirX, float dirY) = 0;

	virtual void OnWeaponSelected() = 0;
	virtual void OnWeaponUnselected() = 0;

};

