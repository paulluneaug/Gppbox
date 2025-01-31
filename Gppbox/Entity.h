#pragma once
#include "SFML/Graphics.hpp"
#include "Game.hpp"

class Entity
{
private:
	static constexpr float FRICTION = 0.9f;

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

	// Jump
	bool m_canJump;
	float m_jumpForce = 100;

	// Collisions
	sf::Vector2i m_size;

	std::pair<int, int> m_xOffsets;

public:
	Entity(Game&, sf::Vector2i size);
	void SetCoordinates(float x, float y);
	void Update(float deltaTime);
	void Draw(sf::RenderWindow& window);
	void Jump();

	bool DrawImGui();

private:

	void UpdatePhysics(float deltaTime);
	bool HasCollisionWithCell(int x, int y);

};

