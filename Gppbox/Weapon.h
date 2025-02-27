#pragma once
#include "Game.hpp"
#include "Timer.h"
#include "Projectile.h"

class Weapon
{
protected:
	bool m_shouldShoot;
	Game& m_game;
	Timer m_reloadTimer;
	std::vector<Projectile*> m_shotProjectiles;

	sf::Shape* m_weaponSprite;
	sf::Shape* m_muzzleFireSprite;
	Timer m_muzzleFireTimer;

	bool m_selected = false;

public:
	Weapon(Game& r_game, float reloadTime);
	virtual ~Weapon();

	void SetShouldShoot(bool shootState);
	virtual bool Update(float deltaTime, float posX, float posY, float dirX, float dirY);

	virtual void OnWeaponSelected();
	virtual void OnWeaponUnselected();

	virtual void Draw(sf::RenderWindow& r_window);
	virtual Vector2f GetKnockback() = 0;

protected:
	virtual bool TryShoot(float deltaTime, float posX, float posY, float dirX, float dirY) = 0;

	virtual sf::Shape* CreateSprite() = 0;
	sf::Shape* CreateMuzzleFireSprite();

	virtual void UpdateSpritePosition(float posX, float posY, float dirX, float dirY);
}; 

