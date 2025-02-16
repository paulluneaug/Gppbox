#pragma once

#include "Entity.h"
#include "Weapon.h" 

class PlayerEntity : public Entity
{
	typedef Entity Super;

	static constexpr int WEAPONS_COUNT = 2;

private:
	Weapon* m_weapons[WEAPONS_COUNT];
	int m_selectedWeaponIndex;

	sf::Vector2f m_weaponOffset;

	bool m_shootInput;

public:
	PlayerEntity(Game& r_game);
	~PlayerEntity();

	virtual void Update(float deltaTime) override;
	virtual void Draw(sf::RenderWindow& r_window) override;

	void SetShootState(bool shootState);
	void SelectNextWeapon();
};

