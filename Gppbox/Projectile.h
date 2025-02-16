#pragma once
#include <imgui-SFML.h>
#include "Game.hpp"

class Projectile
{
public:
	static constexpr float DEFAULT_DAMAGE = 1.0f;
	static constexpr float DEFAULT_SPEED = 600.0f;
	static constexpr float MAX_LIFETIME = 100.0f;

	bool IsAlive;

protected:
	Game& m_game;
	sf::Shape* m_sprite;

	// Movements
	float m_angle;
	float m_speed;
	Vector2f m_position;

	float m_maxSpeed = DEFAULT_SPEED;
	float m_damage = 1.0f;

	bool m_canCollideWithWalls = true;

	float m_currentLifetime;

public:
	Projectile(Game& r_game, Vector2f position, Vector2f direction, bool canCollideWithWalls = true);
	virtual ~Projectile();
	virtual void Update(float deltaTime);
	void Draw(sf::RenderWindow& r_window);


protected:
	virtual void UpdateVelocity(float deltaTime);
private :

	bool CollidesWithWall(float x, float y);
	bool CollidesWithEnemy(float x, float y, Entity** o_hitEnemy);

	bool CheckCollisions(float x, float y);
	bool CheckCollisions(const Vector2f& position);
};

