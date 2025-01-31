#include <iostream>

#include "C.hpp"
#include "Entity.h"
#include "GlobalParameters.h"


Entity::Entity(Game& game, sf::Vector2i size) :
	m_game(game),
	m_canJump(true),
	m_size(size),
	m_sprite(nullptr)
{
	sf::Texture texture = {};
	texture.loadFromFile("res/Player.png");
	//m_sprite.setTexture(texture);

	m_sprite = new sf::RectangleShape({ float(size.x * Consts::GRID_SIZE), float(size.y * Consts::GRID_SIZE) });
	m_sprite->setFillColor(sf::Color::Red);
	m_sprite->setOutlineColor(sf::Color::Magenta);
	m_sprite->setOutlineThickness(0.0f);

	sf::Vector2f origin = sf::Vector2f{ float(int(size.x / 2) * Consts::GRID_SIZE), float(size.y * Consts::GRID_SIZE) };
	m_sprite->setOrigin(origin);

	m_xOffsets = size.x % 2 == 0 ?
		std::pair<int, int>{ -size.x / 2, size.x / 2} :
		std::pair<int, int>{ -size.x / 2, size.x / 2 + 1 };

	std::cout << "X offsets : " << origin.x << "; " << origin.y << std::endl;
}

void Entity::SetCoordinates(float x, float y)
{
	Xx = x;
	Yy = y;

	GridX = (int)(Xx / Consts::GRID_SIZE);
	GridY = (int)(Yy / Consts::GRID_SIZE);

	Rx = (x - GridX * Consts::GRID_SIZE) / 16;
	Ry = (y - GridY * Consts::GRID_SIZE) / 16;

	Dx = 0.0f;
	Dy = 0.0f;
}

void Entity::Update(float deltaTime)
{

	Xx = (GridX + Rx) * Consts::GRID_SIZE;
	Yy = (GridY + Ry) * Consts::GRID_SIZE;

	UpdatePhysics(deltaTime);

}

void Entity::UpdatePhysics(float deltaTime)
{
	Rx += Dx * deltaTime;
	Ry += Dy * deltaTime;

	Dx *= FRICTION;
	Dy += GlobalParameters::GRAVITY * deltaTime;
	Dy *= std::pow(FRICTION, deltaTime);

	while (Rx > 1.0f)
	{
		bool hasCollision = false;
		for (int heightSegment = 0; heightSegment < m_size.y; ++heightSegment)
		{
			if (HasCollisionWithCell(GridX + m_xOffsets.second + 1, GridY - heightSegment))
			{
				hasCollision = true;
				break;
			}
		}

		if (hasCollision)
		{
			Rx = 0.99f;
			Dx = 0.0f;
		}
		else
		{
			--Rx;
			++GridX;
		}
	}

	while (Rx < 0.0f)
	{
		bool hasCollision = false;
		for (int heightSegment = 0; heightSegment < m_size.y; ++heightSegment)
		{
			if (HasCollisionWithCell(GridX + m_xOffsets.first - 1, GridY - heightSegment))
			{
				hasCollision = true;
				break;
			}
		}

		if (hasCollision)
		{
			Rx = 0.0f;
			Dx = 0.0f;
		}
		else
		{
			++Rx;
			--GridX;
		}
	}

	while (Ry > 1.0f)
	{
		bool hasCollision = false;
		for (int widthSegment = m_xOffsets.first; widthSegment <= m_xOffsets.second; ++widthSegment)
		{
			if (HasCollisionWithCell(GridX + widthSegment, GridY + 1))
			{
				hasCollision = true;
				break;
			}
		}

		if (hasCollision)
		{
			Ry = 0.99f;
			Dy = 0.0f;

			m_canJump = true;
		}
		else
		{
			--Ry;
			++GridY;
		}
	}

	while (Ry < 0.0f)
	{
		bool hasCollision = false;
		for (int widthSegment = m_xOffsets.first; widthSegment <= m_xOffsets.second; ++widthSegment)
		{
			if (HasCollisionWithCell(GridX + widthSegment, GridY - m_size.y - 1))
			{
				hasCollision = true;
				break;
			}
		}

		if (hasCollision)
		{
			Ry = 0.0f;
			Dy = 0.0f;
		}
		else
		{
			++Ry;
			--GridY;
		}
	}

}

bool Entity::HasCollisionWithCell(int cellX, int cellY)
{
	return m_game.IsWall(cellX, cellY);
}

void Entity::Draw(sf::RenderWindow& window)
{
	m_sprite->setPosition({ Xx, Yy });
	window.draw(*m_sprite);
}

void Entity::Jump()
{
	if (m_canJump)
	{
		Dy -= m_jumpForce;
		m_canJump = false;
	}
}

bool Entity::DrawImGui()
{
	return false;
}
