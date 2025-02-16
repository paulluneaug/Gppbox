#include "HomingMissile.h"
#include "Entity.h"
#include "Utils.h"
#include "C.hpp"

HomingMissile::HomingMissile(Game& r_game, Vector2f position, Vector2f direction, Entity* target):
	Super(r_game, position, direction, false),
	m_target(target)
{
	m_sprite->setFillColor(Color::Red);
}

void HomingMissile::Update(float deltaTime)
{
	Super::Update(deltaTime);
}

void HomingMissile::UpdateVelocity(float deltaTime)
{
	Super::UpdateVelocity(deltaTime);

	if (m_target == nullptr || !m_target->IsAlive()) 
	{
		m_target = m_game.GetClosestEnemy(m_position.x, m_position.y);
	}



	if (m_target != nullptr && m_target->IsAlive()) 
	{
		sf::Vector2f toTarget = { m_target->Xx - m_position.x, m_target->Yy - m_position.y };
		sf::Vector2f currentDirection = { cos(m_angle), sin(m_angle) };

		float angleDelta = Utils::SignedAngle(currentDirection, toTarget);
		float maxDelta = MAX_ROTATION_SPEED * deltaTime;
		m_angle += std::clamp(angleDelta, -maxDelta, maxDelta);
	}

}
