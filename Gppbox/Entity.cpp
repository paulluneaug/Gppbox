#include <iostream>

#include <imgui.h>

#include "C.hpp"
#include "Entity.h"
#include "GlobalParameters.h"


Entity::Entity(Game& game, sf::Vector2i size) :
	m_game(game),
	m_grounded(true),
	m_size(size),
	m_sprite(nullptr),
	m_name("Entity"),
	m_freeze(false),
	m_groundFriction(DEFAULT_GROUND_FRICTION),
	m_airFriction(DEFAULT_AIR_FRICTION),
	m_jumpInput(false)
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

	m_xOffsets = (size.x & 1) == 0 ?
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

void Entity::SetGridCoordinates(int x, int y)
{
	SetCoordinates(x * Consts::GRID_SIZE, y * Consts::GRID_SIZE);
}

void Entity::SetName(const std::string& name)
{
	m_name = name;
}

void Entity::SetJumpInput(bool state)
{
	m_jumpInput = state;
}

void Entity::Update(float deltaTime)
{
	if (m_freeze) 
	{
		return;
	}

	UpdatePosition(deltaTime);

}

void Entity::UpdatePosition(float deltaTime)
{
	TryJump();

	Rx += Dx * deltaTime;
	Ry += Dy * deltaTime;

	float friction = m_grounded ? m_groundFriction : m_airFriction;

	Dx *= friction;
	Dy += GlobalParameters::GRAVITY * deltaTime;
	Dy *= std::powf(m_airFriction, deltaTime);

	ResolvePhysics(deltaTime);

	Xx = (GridX + Rx) * Consts::GRID_SIZE;
	Yy = (GridY + Ry) * Consts::GRID_SIZE;
}

void Entity::TryJump()
{
	if (!m_jumpInput || !m_grounded) 
	{
		return;
	}

	Dy -= m_jumpForce;
}

void Entity::ResolvePhysics(float deltaTime)
{
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

			m_grounded = true;
		}
		else
		{
			--Ry;
			++GridY;
			m_grounded = false;
		}
	}

	while (Ry < 0.0f)
	{
		m_grounded = false;
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

bool Entity::DrawImGui()
{
	if (ImGui::TreeNode(m_name.c_str()))
	{
		ImGui::Text("Transform");

		// Freeze
		ImGui::Checkbox("Freeze", &m_freeze);

		// Position
		float position[2] = { Xx, Yy };
		if (ImGui::DragFloat2("Position", position))
		{
			SetCoordinates(position[0], position[1]);
		}

		ImGui::Text("Grid coordinates : (%i ; %i)", GridX, GridY);
		ImGui::Text("Coordinates in cell : (%.2f ; %.2f)", Rx, Ry);


		ImGui::Separator();

		ImGui::Text("Controller Settings");
		ImGui::DragFloat("Jump Force", &m_jumpForce);
		ImGui::DragFloat("Ground Friction", &m_groundFriction);
		ImGui::DragFloat("Air Friction", &m_airFriction);

		ImGui::TreePop();
	}
	return false;
}
