#pragma once
#include "Game.hpp"
#include "Timer.h"

class Weapon
{
protected:
	bool m_shouldShoot;
	Game& m_game;
	Timer m_reloadTimer;

public:
	Weapon(Game& r_game, float reloadTime);

	void SetShouldShoot(bool shootState);
	virtual void Update(float deltaTime, float posX, float posY, float dirX, float dirY) = 0;

	virtual void OnWeaponSelected();
	virtual void OnWeaponUnselected();

	virtual void Draw(sf::RenderWindow& r_window) = 0;

};

