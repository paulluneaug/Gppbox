#pragma once

#include "Entity.h"

class EnemyEntity : public Entity
{
	typedef Entity Super;

private :
	bool m_goingRight;

public:
	EnemyEntity(Game& r_game);

protected:
	void UpdatePosition(float deltaTime) override;

private:
	void UpdateDirection(float deltaTime);
};

