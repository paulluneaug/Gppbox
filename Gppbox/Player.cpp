#include "Player.h"
#include <cmath>

#include "C.hpp"

PlayerEntity::PlayerEntity(Game& r_game) :
	Super(r_game, { 1, 1 }, 3.0f),
	m_weapon(Rifle(r_game, 0.1f)),
	m_weaponOffset({ 0.0f * Consts::GRID_SIZE , 0.5f * Consts::GRID_SIZE }),
	m_shootInput(false)
{
	m_weapon.OnWeaponSelected();
	m_weapon.SetShouldShoot(true);
}

void PlayerEntity::Update(float deltaTime)
{
	Super::Update(deltaTime);
	m_weapon.Update(
		deltaTime, 
		Xx + m_weaponOffset.x, 
		Yy - m_weaponOffset.y, 
		std::copysign(1, Dx),
		0);
}

void PlayerEntity::Draw(sf::RenderWindow& r_window)
{
	Super::Draw(r_window);
	m_weapon.Draw(r_window);
}

void PlayerEntity::SetShootState(bool shootState)
{
	m_shootInput = shootState;
	m_weapon.SetShouldShoot(shootState);
}
