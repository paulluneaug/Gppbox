#pragma once

#include "Entity.h"
#include "Weapon.h"
#include "Laser.h"

class PlayerEntity : public Entity
{
	typedef Entity Super;

	static constexpr int WEAPONS_COUNT = 4;

private:
	Weapon* m_weapons[WEAPONS_COUNT];
	Laser* m_laser;
	int m_selectedWeaponIndex;

	sf::Vector2f m_weaponOffset;

	bool m_shootInput;
	float m_facingDirection;

public:
	PlayerEntity(Game& r_game);
	~PlayerEntity();

	virtual void SetCoordinates(float x, float y) override;
	virtual void Update(float deltaTime) override;
	virtual void Draw(sf::RenderWindow& r_window) override;

	void SetShootState(bool shootState);
	void SelectNextWeapon();

protected:

	virtual void UpdatePosition(float deltaTime) override;
};

