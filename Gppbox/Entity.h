#pragma once
#include "SFML/Graphics.hpp"
#include "Game.hpp"

class Entity
{
private:
	static constexpr float DEFAULT_AIR_FRICTION = 0.9f;
	static constexpr float DEFAULT_GROUND_FRICTION = 0.9f;

public:

	// Base coordinates
	int GridX;
	int GridY;
	float Rx;
	float Ry;

	// Resulting coordinates
	float Xx;
	float Yy;

	// Movements
	float Dx;
	float Dy;

	sf::Vector2f Input;

private:
	Game& m_game;
	sf::Shape* m_sprite;
	std::string m_name;

	bool m_freeze;
	float m_groundFriction;
	float m_airFriction;

	// Speed
	float m_lateralSpeed = 8.0f;
	float m_maxSpeed = 1.0f;

	// Jump
	bool m_grounded;
	float m_jumpForce = 100;

	bool m_jumpInput;

	float m_health;

	// Collisions
	sf::Vector2i m_size;

	std::pair<int, int> m_xOffsets;

public:
	Entity(Game& r_game, sf::Vector2i size, float health);

	void SetCoordinates(float x, float y);
	void SetGridCoordinates(int x, int y);
	void SetName(const std::string& name);

	void Update(float deltaTime);
	void Draw(sf::RenderWindow& r_window);
	void SetJumpInput(bool state);

	virtual bool DrawImGui();

	bool CollidesWithPoint(float x, float y);

	void TakeDamage(float damage);
	bool IsAlive();

private:

	void TryJump();

	void ResolvePhysics(float deltaTime);

protected:
	virtual void UpdatePosition(float deltaTime);
	bool CollidesLeft(float deltaTime);
	bool CollidesRight(float deltaTime);

	bool HasCollisionWithCell(int x, int y);

};

