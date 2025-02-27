#pragma once
#include "Weapon.h"
class Laser : public Weapon
{
	typedef Weapon Super;
private:
	Timer m_laserShootingTimer;

	Vector2f m_currentLaserPosition;
	Vector2f m_currentLaserDirection;
	Vector2f m_lastHitPoint;

	float m_damagePerSecond;

	sf::Shape* m_laserSprite;

public:
	Laser(Game& r_game, float reloadTime, float laserDuration, float damagePerSecond);
	virtual ~Laser() override;

	virtual bool Update(float deltaTime, float posX, float posY, float dirX, float dirY) override;
	virtual Vector2f GetKnockback() override;

	bool IsShooting() const;

	virtual void Draw(sf::RenderWindow& r_window) override;

protected:

	virtual bool TryShoot(float deltaTime, float posX, float posY, float dirX, float dirY) override;
	virtual sf::Shape* CreateSprite() override;

private:
	void Shoot(float posX, float posY, float dirX, float dirY);

};

