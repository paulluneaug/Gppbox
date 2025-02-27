#include "Laser.h"
#include "Utils.h"
#include "C.hpp"
#include "Entity.h"

Laser::Laser(Game& r_game, float reloadTime, float laserDuration, float damagePerSecond) :
	Super(r_game, reloadTime, false, {0.0f, 0.0f}),
	m_laserShootingTimer(Timer(laserDuration, false)),
	m_damagePerSecond(damagePerSecond)
{
	m_laserSprite = new RectangleShape({1.0f , 0.3f * Consts::GRID_SIZE });
	m_laserSprite->setFillColor(Color{ 204u, 153u, 255u, 255u });
	m_laserSprite->setOrigin({ 0.0f * Consts::GRID_SIZE, 0.15f * Consts::GRID_SIZE });
}

Laser::~Laser()
{
	Super::~Weapon();
	delete m_laserSprite;
}

bool Laser::Update(float deltaTime, float posX, float posY, float dirX, float dirY)
{
	bool shot = Super::Update(deltaTime, posX, posY, dirX, dirY);
	if (shot) 
	{
		for (std::array<float, 2>&hitPoint : Utils::Raycast(m_currentLaserPosition, m_currentLaserDirection, 300.0f, { Consts::GRID_SIZE,Consts::GRID_SIZE }))
		{
			Vector2f hitPointAbsolute = { hitPoint[0] * Consts::GRID_SIZE, hitPoint[1] * Consts::GRID_SIZE };
			if (m_game.IsWall(hitPoint[0], hitPoint[1]))
			{
				m_lastHitPoint = hitPointAbsolute;
				break;
			}

			Entity* hitEnemy;
			if (m_game.CollidesWithEnemyAtPoint(hitPointAbsolute.x, hitPointAbsolute.y, &hitEnemy))
			{
				m_lastHitPoint = hitPointAbsolute;
				hitEnemy->TakeDamage(m_damagePerSecond * deltaTime);
				break;
			}
		}
	}

	return shot;
}

Vector2f Laser::GetKnockback()
{
	return { 0.0f, 0.0f };
}

bool Laser::IsShooting() const
{
	return m_selected && m_laserShootingTimer.IsRunning();
}

void Laser::Draw(sf::RenderWindow& r_window)
{
	//float radius = 0.2f * Consts::GRID_SIZE;
	//CircleShape hitPoint = CircleShape(radius);
	//hitPoint.setOrigin({ radius, radius });
	//hitPoint.setFillColor(Color::White);
	//hitPoint.setPosition(m_lastHitPoint);
	//r_window.draw(hitPoint);

	if (m_laserShootingTimer.IsRunning()) 
	{
		float laserLength = Utils::Distance(m_lastHitPoint, m_currentLaserPosition);
		//printf("Distance from (%f; %f) to (%f; %f) : %f \n", m_lastHitPoint.x, m_lastHitPoint.y, m_currentLaserPosition.x, m_currentLaserPosition.y, laserLength);
		m_laserSprite->setScale({ laserLength, 1.0f });
		m_laserSprite->setPosition(m_currentLaserPosition);
		m_laserSprite->setRotation(Utils::SignedAngle(m_currentLaserDirection, { 1.0f, 0.0f }) * Utils::RAD_2_DEG);
		r_window.draw(*m_laserSprite);
	}
	Super::Draw(r_window);
}

bool Laser::TryShoot(float deltaTime, float posX, float posY, float dirX, float dirY)
{
	if (m_laserShootingTimer.IsRunning()) 
	{
		if (m_laserShootingTimer.Update(deltaTime)) 
		{
			m_laserShootingTimer.Stop();
		}
		return true;
	}

	bool reloadTimerFinished = m_reloadTimer.Update(deltaTime);
	if (reloadTimerFinished && !m_shouldShoot) 
	{
		m_reloadTimer.Stop();
	}
	
	if (!m_selected) 
	{
		return false;
	}

	if (!m_reloadTimer.IsRunning() && m_shouldShoot)
	{
		m_reloadTimer.Start();
		Shoot(posX, posY, dirX, dirY);
		return true;
	}

	if (m_shouldShoot && reloadTimerFinished) 
	{
		Shoot(posX, posY, dirX, dirY);
		return true;
	}
	return false;
}

sf::Shape* Laser::CreateSprite()
{
	sf::Shape* sprite = new sf::RectangleShape({ 0.9f * Consts::GRID_SIZE, 0.4f * Consts::GRID_SIZE });
	sprite->setFillColor(Color{ 128u, 0u, 128u, 255u });
	sprite->setOrigin(0.2f * Consts::GRID_SIZE, 0.2f * Consts::GRID_SIZE);
	return sprite;
}

void Laser::Shoot(float posX, float posY, float dirX, float dirY)
{
	m_laserShootingTimer.Start();
	m_currentLaserDirection = { Utils::Sign(dirX), 0.0f };
	m_currentLaserPosition = GetFirePosition(posX, posY, dirX, dirY);
}
