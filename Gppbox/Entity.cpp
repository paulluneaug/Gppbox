#include <iostream>

#include <imgui.h>

#include "C.hpp"
#include "Entity.h"
#include "GlobalParameters.h"


Entity::Entity(Game& game, sf::Vector2i size, float health) :
	m_game(game),
	m_grounded(true),
	m_size(size),
	m_sprite(nullptr),
	m_name("Entity"),
	m_freeze(false),
	m_groundFriction(DEFAULT_GROUND_FRICTION),
	m_airFriction(DEFAULT_AIR_FRICTION),
	m_jumpInput(false),
	m_health(health)
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
	if (m_freeze || !IsAlive())
	{
		return;
	}

	UpdatePosition(deltaTime);

}

void Entity::UpdatePosition(float deltaTime)
{
	TryJump();

	Dx = std::clamp(Dx + Input.x * m_lateralSpeed, -m_maxSpeed, m_maxSpeed);


	float friction = m_grounded ? m_groundFriction : m_airFriction;

	Dx *= std::powf(friction, deltaTime * 60); // Based on 60 frames/seconds
	Dy += GlobalParameters::GRAVITY * deltaTime;
	Dy *= std::powf(m_airFriction, deltaTime);

	Rx += Dx * deltaTime;
	Ry += Dy * deltaTime;

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
	m_grounded = false;
}

void Entity::ResolvePhysics(float deltaTime)
{
	while (Rx > 1.0f)
	{
		bool hasCollision = false;

		if (CollidesRight(deltaTime))
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
		if (CollidesLeft(deltaTime))
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

bool Entity::CollidesLeft(float deltaTime)
{
	for (int heightSegment = 0; heightSegment < m_size.y; ++heightSegment)
	{
		if (HasCollisionWithCell(GridX + m_xOffsets.first - 1, GridY - heightSegment))
		{
			return true;
		}
	}
	return false;
}

bool Entity::CollidesRight(float deltaTime)
{
	for (int heightSegment = 0; heightSegment < m_size.y; ++heightSegment)
	{
		if (HasCollisionWithCell(GridX + m_xOffsets.second + 1, GridY - heightSegment))
		{
			return true;
		}
	}
	return false;
}

bool Entity::HasCollisionWithCell(int cellX, int cellY) const
{
	return m_game.IsWall(cellX, cellY);
}

void Entity::Draw(sf::RenderWindow& r_window)
{
	if (!IsAlive())
	{
		return;
	}
	m_sprite->setPosition({ Xx, Yy });
	r_window.draw(*m_sprite);
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
		ImGui::Text("Velocity : (%.2f ; %.2f)", Dx, Dy);


		ImGui::Separator();

		ImGui::Text("Controller Settings");
		ImGui::DragFloat("Jump Force", &m_jumpForce);
		ImGui::SliderFloat("Ground Friction", &m_groundFriction, 0.0f, 1.0f);
		ImGui::SliderFloat("Air Friction", &m_airFriction, 0.0f, 1.0f);

		ImGui::Spacing();

		ImGui::DragFloat("Lateral Speed", &m_lateralSpeed);
		ImGui::SliderFloat("Max Speed", &m_maxSpeed, 1.0f, 100.0f);

		ImGui::TreePop();
	}
	return false;
}

bool Entity::CollidesWithPoint(float x, float y) const
{
	float cornerX = Xx + m_xOffsets.first;
	float cornerY = Yy - m_size.y * Consts::GRID_SIZE;

	return cornerX <= x && x <= cornerX + m_size.x * Consts::GRID_SIZE
		&& cornerY <= y && y <= cornerY + m_size.y * Consts::GRID_SIZE;
}

void Entity::TakeDamage(float damage)
{
	m_health -= damage;
}

bool Entity::IsAlive() const
{
	return m_health > 0.0f;
}

Vector2f Entity::GetCenter() const
{
	return 
	{ 
		Xx + (m_xOffsets.first + m_size.x * Consts::GRID_SIZE / 2),
		Yy - (m_size.y * Consts::GRID_SIZE / 2)
	};
}
