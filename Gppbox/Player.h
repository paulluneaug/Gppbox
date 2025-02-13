#pragma once

#include "Entity.h"
#include "Rifle.h" 

class PlayerEntity : public Entity
{
	typedef Entity Super;
private:
	Rifle m_weapon;
	sf::Vector2f m_weaponOffset;

	bool m_shootInput;

public:
	PlayerEntity(Game& r_game);

	virtual void Update(float deltaTime) override;
	virtual void Draw(sf::RenderWindow& r_window) override;

	void SetShootState(bool shootState);
};

