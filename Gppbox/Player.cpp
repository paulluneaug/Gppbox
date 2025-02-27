#include "Player.h"
#include <cmath>

#include "C.hpp"
#include "Rifle.h"
#include "MissileLauncher.h"
#include "Drone.h"
#include "Utils.h"

PlayerEntity::PlayerEntity(Game& r_game) :
	Super(r_game, { 1, 1 }, 3.0f),
	m_weaponOffset({ float(m_size.x) / 2 * Consts::GRID_SIZE , float(m_size.y) / 2 * Consts::GRID_SIZE }),
	m_shootInput(false),
	m_selectedWeaponIndex(0),
	m_facingDirection(1.0f)
{
	m_sprite->setFillColor(Color::Cyan);

	m_laser = new Laser(r_game, 3.0f, 2.0f, 5.0f);

	m_weapons[0] = new Rifle(r_game, 0.1f);
	m_weapons[1] = new MissileLauncher(r_game, 0.5f);
	m_weapons[2] = new Drone(r_game, 0.3f, { 5.0f, 5.0f });
	m_weapons[3] = m_laser;


	m_weapons[m_selectedWeaponIndex]->OnWeaponSelected();
}

PlayerEntity::~PlayerEntity()
{
	for (int iWeapon = 0; iWeapon < WEAPONS_COUNT; ++iWeapon) 
	{
		delete m_weapons[iWeapon];
	}
}

void PlayerEntity::Update(float deltaTime)
{
	Super::Update(deltaTime);

	if (Input.x != 0) 
	{
		m_facingDirection = Utils::Sign(Input.x);
	}

	Vector2f knockback = { 0.0f, 0.0f };
	for (Weapon* weapon : m_weapons) 
	{
		bool shot = weapon->Update(
			deltaTime,
			Xx + m_weaponOffset.x,
			Yy - m_weaponOffset.y,
			m_facingDirection,
			0);

		if (shot) 
		{
			knockback += weapon->GetKnockback();
		}
	}

	Dx += knockback.x * -m_facingDirection;
}

void PlayerEntity::Draw(sf::RenderWindow& r_window)
{
	Super::Draw(r_window);
	for (Weapon* weapon : m_weapons)
	{
		weapon->Draw(r_window);
	}
}

void PlayerEntity::SetShootState(bool shootState)
{
	m_shootInput = shootState;
	for (Weapon* weapon : m_weapons)
	{
		weapon->SetShouldShoot(shootState);
	}
}

void PlayerEntity::SelectNextWeapon()
{
	m_weapons[m_selectedWeaponIndex]->OnWeaponUnselected();
	m_selectedWeaponIndex = (m_selectedWeaponIndex + 1) % WEAPONS_COUNT;
	m_weapons[m_selectedWeaponIndex]->OnWeaponSelected();
}

void PlayerEntity::UpdatePosition(float deltaTime)
{
	if (!m_laser->IsShooting()) 
	{
		Super::UpdatePosition(deltaTime);
	}
}
