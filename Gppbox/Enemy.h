#pragma once

#include "Entity.h"

class EnemyEntity : public Entity
{
private :
	bool m_goingRight;

public:
	EnemyEntity(Game& r_game);

protected:
	void UpdatePosition(float deltaTime) override;

private:
	void UpdateDirection(float deltaTime);
};

