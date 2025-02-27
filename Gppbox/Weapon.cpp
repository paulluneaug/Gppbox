#include "C.hpp"
#include "Utils.h"
#include "Weapon.h"

Weapon::Weapon(Game& r_game, float reloadTime) :
	m_game(r_game),
	m_reloadTimer(Timer{ reloadTime,true }),
	m_muzzleFireTimer(Timer{ 0.001f, false })
{
	m_muzzleFireSprite = CreateMuzzleFireSprite();
}

Weapon::~Weapon()
{
	delete m_weaponSprite;
	delete m_muzzleFireSprite;
}

void Weapon::SetShouldShoot(bool shootState)
{
	m_shouldShoot = shootState;
}

bool Weapon::Update(float deltaTime, float posX, float posY, float dirX, float dirY)
{
	if (m_weaponSprite == nullptr) 
	{
		m_weaponSprite = CreateSprite();
	}

	if (m_muzzleFireTimer.Update(deltaTime))
	{
		m_muzzleFireTimer.Stop();
	}
	bool shot = TryShoot(deltaTime, posX, posY, dirX, dirY);

	for (auto iterator = m_shotProjectiles.begin(); iterator != m_shotProjectiles.end();)
	{
		Projectile* projectile = *iterator;
		if (projectile->IsAlive)
		{
			projectile->Update(deltaTime);
			++iterator;
		}
		else
		{
			delete projectile;
			iterator = m_shotProjectiles.erase(iterator);
		}
	}
	UpdateSpritePosition(posX, posY, dirX, dirY);

	return shot;
}

void Weapon::OnWeaponSelected()
{
	m_reloadTimer.Start();
	m_selected = true;
}

void Weapon::OnWeaponUnselected()
{
	m_reloadTimer.Stop();
	m_selected = false;
}

void Weapon::Draw(sf::RenderWindow& r_window)
{
	r_window.draw(*m_weaponSprite);
	if (m_muzzleFireTimer.IsRunning())
	{
		r_window.draw(*m_muzzleFireSprite);
	}

	for (Projectile* projectile : m_shotProjectiles)
	{
		if (projectile->IsAlive)
		{
			projectile->Draw(r_window);
		}
	}
}

//sf::Shape* Weapon::CreateSprite()
//{
//	return nullptr;
//}

sf::Shape* Weapon::CreateMuzzleFireSprite()
{
	float radius = 0.4f * Consts::GRID_SIZE;
	sf::Shape* sprite = new sf::CircleShape(radius);
	sprite->setFillColor(Color::Yellow);
	sprite->setOrigin({ radius , radius });
	return sprite;
}

void Weapon::UpdateSpritePosition(float posX, float posY, float dirX, float dirY)
{
	m_weaponSprite->setPosition(posX, posY);
	m_weaponSprite->setRotation(Utils::Sign(dirX) * 90 - 90);
}
