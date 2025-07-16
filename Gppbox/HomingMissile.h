#pragma once
#include "Projectile.h"

class HomingMissile : public Projectile
{
	typedef Projectile Super;

private:
	static constexpr float MAX_ROTATION_SPEED = 5.0f;

private:
	Entity* m_target;

public:
	HomingMissile(Game& r_game, Vector2f position, Vector2f direction, Entity* target);
	virtual void Update(float deltaTime) override;

protected:
	virtual void UpdateVelocity(float deltaTime) override;
};

