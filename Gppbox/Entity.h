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

private:
	Game& m_game;
	sf::Shape* m_sprite;
	std::string m_name;

	bool m_freeze;
	float m_groundFriction;
	float m_airFriction;

	// Jump
	bool m_grounded;
	float m_jumpForce = 100;

	bool m_jumpInput;

	// Collisions
	sf::Vector2i m_size;

	std::pair<int, int> m_xOffsets;

public:
	Entity(Game& r_game, sf::Vector2i size);

	void SetCoordinates(float x, float y);
	void SetGridCoordinates(int x, int y);
	void SetName(const std::string& name);

	void Update(float deltaTime);
	void Draw(sf::RenderWindow& r_window);
	void SetJumpInput(bool state);

	bool DrawImGui();

private:

	void UpdatePosition(float deltaTime);
	void TryJump();

	void ResolvePhysics(float deltaTime);
	bool HasCollisionWithCell(int x, int y);

};

