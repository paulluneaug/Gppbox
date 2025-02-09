#pragma once
#include <imgui-SFML.h>
#include "Game.hpp"

class Projectile
{
public:
	static constexpr float DEFAULT_DAMAGE = 1.0f;
	bool IsAlive;

private:
	Game& m_game;
	sf::Shape* m_sprite;

	// Movements
	Vector2f m_velocity;
	Vector2f m_position;

	float m_maxSpeed = 1.0f;
	float m_damage = 1.0f;

public:
	Projectile(Game& r_game, Vector2f position, Vector2f direction);
	void Update(float deltaTime);
	void Draw(sf::RenderWindow& r_window);

protected:
	virtual void UpdateVelocity(float deltaTime);
private :

	bool CollidesWithWall();
	bool CollidesWithEnemy(Entity* o_hitEnemy);
};

