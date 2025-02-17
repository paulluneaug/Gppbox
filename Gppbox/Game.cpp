
#include <array>
#include <imgui.h>
#include <vector>

#include "C.hpp"
#include "Game.hpp"

#include "HotReloadShader.hpp"

#include "Enemy.h"
#include "Entity.h"
#include "Player.h"
#include "Utils.h"

#include "GlobalParameters.h"
#include "Random.h"


static int cols = 1280 / Consts::GRID_SIZE;
static int lastLine = 720 / Consts::GRID_SIZE - 1;

Game::Game(sf::RenderWindow* win) :
	m_editMode(false),
	m_window(win)
{
	m_background = sf::RectangleShape(Vector2f((float)win->getSize().x, (float)win->getSize().y));

	Random::SetSeed(0);

	bool isOk = m_backgroundTexture.loadFromFile("res/bg_stars.png");
	if (!isOk) {
		printf("ERR : LOAD FAILED\n");
	}
	m_background.setTexture(&m_backgroundTexture);
	m_background.setSize(sf::Vector2f(1280, 720));

	m_backgroundShader = new HotReloadShader("res/bg.vert", "res/bg.frag");
	InitPlayer();
	LoadLevel();
}

Game::~Game()
{
	DeleteIfExists(m_backgroundShader);
	DeleteIfExists(m_player);
	ClearLevel();
}

void Game::InitPlayer()
{
	m_player = new PlayerEntity(*this);
	m_player->SetName("Player");
	m_player->SetGridCoordinates(30, 10);
}

void Game::CacheWalls()
{
	m_wallSprites.clear();
	for (Vector2i& w : m_walls)
	{
		sf::RectangleShape rect(Vector2f(Consts::GRID_SIZE, Consts::GRID_SIZE));
		rect.setPosition((float)w.x * Consts::GRID_SIZE, (float)w.y * Consts::GRID_SIZE);
		rect.setFillColor(sf::Color(0x07ff07ff));
		m_wallSprites.push_back(rect);
	}
}

void Game::ProcessInput(sf::Event ev) {
	if (ev.type == sf::Event::Closed)
	{
		m_window->close();
		m_closing = true;
		return;
	}
	if (ev.type == sf::Event::KeyReleased)
	{


	}
}


static double g_time = 0.0;
static double g_tickTimer = 0.0;


void Game::PollInput(double dt)
{
	bool leftPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q);
	bool rightPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D);
	m_player->Input.x = (leftPressed ? -1 : 0) + (rightPressed ? 1 : 0);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) 
	{
		m_player->SelectNextWeapon();
	}

	bool rightButtonPressed = sf::Mouse::isButtonPressed(sf::Mouse::Right);
	bool leftButtonPressed = sf::Mouse::isButtonPressed(sf::Mouse::Left);

	m_player->SetShootState(leftButtonPressed);

	if (rightButtonPressed)
	{
		ProcessMouseInput(sf::Mouse::Right);
	}
	if (leftButtonPressed)
	{
		ProcessMouseInput(sf::Mouse::Left);
	}

	bool spacePressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space);
	m_player->SetJumpInput(spacePressed);

}

static sf::VertexArray va;
static RenderStates vaRs;
static std::vector<sf::RectangleShape> rects;

int blendModeIndex(sf::BlendMode bm) {
	if (bm == sf::BlendAlpha) return 0;
	if (bm == sf::BlendAdd) return 1;
	if (bm == sf::BlendNone) return 2;
	if (bm == sf::BlendMultiply) return 3;
	return 4;
};

void Game::Update(double dt) {
	PollInput(dt);

	g_time += dt;
	if (m_backgroundShader) m_backgroundShader->update(dt);

	beforeParts.update(dt);
	afterParts.update(dt);

	if (!m_editMode)
	{
		m_player->Update(dt);
		for (Entity* entity : m_enemies)
		{
			entity->Update(dt);
		}
	}
}

void Game::Draw(sf::RenderWindow& win) {
	if (m_closing) return;

	sf::RenderStates states = sf::RenderStates::Default;
	sf::Shader* sh = &m_backgroundShader->sh;
	states.blendMode = sf::BlendAdd;
	states.shader = sh;
	states.texture = &m_backgroundTexture;
	sh->setUniform("texture", m_backgroundTexture);
	//sh->setUniform("time", g_time);
	win.draw(m_background, states);

	beforeParts.draw(win);

	for (sf::RectangleShape& r : m_wallSprites)
		win.draw(r);

	for (sf::RectangleShape& r : rects)
		win.draw(r);

	m_player->Draw(win);
	for (Entity* entity : m_enemies) {
		entity->Draw(win);
	}


	afterParts.draw(win);

	float radius = 0.3f * Consts::GRID_SIZE;
	sf::CircleShape dot = sf::CircleShape(radius);
	dot.setOrigin({ radius , radius });
	dot.setFillColor(Color::Red);

	dot.setPosition(m_raycastOrigin);
	win.draw(dot);

	sf::Vector2f direction =
	{
		cos(m_raycastAngle),
		sin(m_raycastAngle)
	};

	sf::Vector2f raycastEnd =
	{
		m_raycastOrigin.x + direction.x * m_raycastDistance * Consts::GRID_SIZE,
		m_raycastOrigin.y + direction.y * m_raycastDistance * Consts::GRID_SIZE
	};
	dot.setPosition(raycastEnd);
	win.draw(dot);

	radius = 0.2f * Consts::GRID_SIZE;
	dot.setRadius(radius);
	dot.setOrigin({ radius , radius });
	dot.setFillColor(Color::Cyan);

	for (std::array<float, 2>& hitPoint : Utils::Raycast(m_raycastOrigin, direction, m_raycastDistance, { Consts::GRID_SIZE,Consts::GRID_SIZE }))
	{
		dot.setPosition({ hitPoint[0] * Consts::GRID_SIZE, hitPoint[1] * Consts::GRID_SIZE });
		win.draw(dot);
	}
}


bool Game::IsWall(int cx, int cy)
{
	for (Vector2i& w : m_walls) {
		if (w.x == cx && w.y == cy)
			return true;
	}
	return false;
}

bool Game::IsWall(sf::Vector2i position)
{
	return IsWall(position.x, position.y);
}

bool Game::CollidesWithEnemyAtPoint(float x, float y, Entity** o_hitEnemy)
{
	for (Entity* enemy : m_enemies)
	{	
		if (!enemy->IsAlive()) 
		{
			continue;
		}

		if (enemy->CollidesWithPoint(x, y)) 
		{
			*o_hitEnemy = enemy;
			return true;
		}
	}
	return false;
}

Entity* Game::GetClosestEnemy(float x, float y)
{
	float closestDistance = FLT_MAX;
	Entity* closestEnemy = nullptr;

	for (Entity* enemy : m_enemies) 
	{
		if (!enemy->IsAlive()) 
		{
			continue;
		}
		float distance = Utils::SqrDistance(enemy->Xx, enemy->Yy, x, y);
		if (closestDistance > distance) 
		{
			closestDistance = distance;
			closestEnemy = enemy;
		}
	}

	return closestEnemy;
}

void Game::DrawImGui()
{
	if (ImGui::CollapsingHeader("Raycast")) 
	{
		float origin[] = { m_raycastOrigin.x, m_raycastOrigin.y };
		if (ImGui::DragFloat2("Origin", origin)) 
		{
			m_raycastOrigin.x = origin[0];
			m_raycastOrigin.y = origin[1];
		}

		ImGui::SliderAngle("Angle", &m_raycastAngle);
		ImGui::DragFloat("Dist", &m_raycastDistance, 1.0f, 0.0f);
	}

	if (ImGui::CollapsingHeader("Edit Mode"))
	{
		if (!m_editMode)
		{
			if (ImGui::Button("Enter Edit Mode"))
			{
				EnterEditMode();
			}
		}
		else
		{
			if (ImGui::Button("Exit Edit Mode"))
			{
				ExitEditMode();
			}
			ImGui::SameLine();
			if (ImGui::Button("Save Level"))
			{
				SaveLevel();
			}

			const char* placableObjectsNames[] = { "None", "Wall", "Enemy", "Player" };
			const char* selectedObjectName = placableObjectsNames[m_selectedObject];

			if (ImGui::BeginCombo("Selected Object", selectedObjectName))
			{
				for (int n = 0; n < IM_ARRAYSIZE(placableObjectsNames); n++)
				{
					const bool isSelected = (m_selectedObject == n);
					if (ImGui::Selectable(placableObjectsNames[n], isSelected))
					{
						m_selectedObject = PlacableObject(n);
					}

					if (isSelected)
					{
						ImGui::SetItemDefaultFocus();
					}
				}
				ImGui::EndCombo();
			}
		}
	}

	if (ImGui::CollapsingHeader("Global Paramaters"))
	{
		ImGui::DragFloat("Gravity", &GlobalParameters::GRAVITY, 0.5f, 0.0f);
	}

	if (ImGui::CollapsingHeader("Entities"))
	{
		m_player->DrawImGui();
		for (Entity* entity : m_enemies)
		{
			entity->DrawImGui();
		}
	}
}

#pragma region Level Editor

void Game::LoadLevel()
{
	enum ReadState
	{
		Walls,
		Player,
		Enemies,
		EndFile,
	};

	std::ifstream fileStream("SavedLevel.txt");

	if (fileStream.fail())
	{
		CreateEmptyLevel();
		return;
	}

	std::string readLine;
	ReadState readState = ReadState::Walls;
	sf::Vector2i parsedVector;

	// Read level file
	while (std::getline(fileStream, readLine))
	{
		switch (readState)
		{
		case ReadState::Walls:
			if (readLine.empty())
			{
				readState = ReadState::Player;
				break;
			}

			if (TryParseVector2i(readLine, DELIMITER, parsedVector))
			{
				m_walls.push_back(parsedVector);
			}
			else
			{
				printf("[ERR] Failed to parse the vector %s", readLine.c_str());
			}

			break;

		case ReadState::Player:
			if (readLine.empty())
			{
				readState = ReadState::Enemies;
				break;
			}

			if (TryParseVector2i(readLine, DELIMITER, parsedVector))
			{
				m_player->SetGridCoordinates(parsedVector.x, parsedVector.y);
			}
			else
			{
				printf("[ERR] Failed to parse the vector %s", readLine.c_str());
			}
			break;

		case ReadState::Enemies:
			if (readLine.empty())
			{
				readState = ReadState::EndFile;
				break;
			}

			if (TryParseVector2i(readLine, DELIMITER, parsedVector))
			{
				m_enemies.push_back(CreateEnemyAtPosition(parsedVector));
			}
			else
			{
				printf("[ERR] Failed to parse the vector %s", readLine.c_str());
			}
			break;

		case ReadState::EndFile:
			break;
		}
	}

	CacheWalls();
}

void Game::CreateEmptyLevel()
{
	SaveLevel();
}

void Game::ReloadLevel()
{
	ClearLevel();
	LoadLevel();
}

void Game::SaveLevel()
{
	std::ofstream fileStream("SavedLevel.txt");

	// Walls
	for (sf::Vector2i wall : m_walls)
	{
		fileStream << wall.x << DELIMITER << wall.y << "\n";
	}
	fileStream << "\n";

	// Player
	fileStream << m_player->GridX << DELIMITER << m_player->GridY << "\n";
	fileStream << "\n";

	// Enemies
	for (Entity* enemy : m_enemies)
	{
		fileStream << enemy->GridX << DELIMITER << enemy->GridY << "\n";
	}
	fileStream << "\n";
}

void Game::ClearLevel()
{
	for (Entity* entity : m_enemies)
	{
		DeleteIfExists(entity);
	}
	m_enemies.clear();

	m_walls.clear();
}

void Game::EnterEditMode()
{
	if (m_editMode)
	{
		return;
	}
	m_editMode = true;

	// Reload the level
	ReloadLevel();
}

void Game::ExitEditMode()
{
	if (!m_editMode)
	{
		return;
	}
	m_editMode = false;
}

void Game::ProcessMouseInput(sf::Mouse::Button pressedButton)
{
	if (ImGui::IsWindowHovered() || ImGui::IsWindowFocused())
	{
		return;
	}

	if (m_editMode)
	{
		ProcessMouseInput_EditMode(pressedButton);
	}

}

void Game::ProcessMouseInput_EditMode(sf::Mouse::Button pressedButton)
{
	bool destroyObject = false;

	switch (pressedButton)
	{
	case sf::Mouse::Button::Left:
		destroyObject = false;
		break;

	case sf::Mouse::Button::Right:
		destroyObject = true;
		break;

	default:
		printf("[ERR] Unsupported mouse button");
		return;
	}

	sf::Vector2i mousePosition = sf::Mouse::getPosition(*m_window);
	sf::Vector2i mouseGridPosition =
	{
		mousePosition.x / Consts::GRID_SIZE,
		mousePosition.y / Consts::GRID_SIZE,
	};

	sf::Vector2i enemyPosition = mouseGridPosition + sf::Vector2i(0, 1);

	switch (m_selectedObject)
	{
	case PlacableObject::None:
		break;

	case PlacableObject::Wall:
		if (destroyObject)
		{
			auto newEnd = std::remove(m_walls.begin(), m_walls.end(), mouseGridPosition);
			if (newEnd != m_walls.end())
			{
				m_walls.erase(newEnd, m_walls.end());
				CacheWalls();
			}
		}
		else
		{
			if (!IsWall(mouseGridPosition))
			{
				m_walls.push_back(mouseGridPosition);
				CacheWalls();
			}
		}
		break;

	case PlacableObject::Enemy:
		if (destroyObject)
		{
			auto newEnd = std::remove_if(
				m_enemies.begin(),
				m_enemies.end(),
				[enemyPosition](const Entity* e)
				{
					return e->GridX == enemyPosition.x && e->GridY == enemyPosition.y;
				}
			);

			auto currentEnd = m_enemies.end();
			if (newEnd != currentEnd)
			{
				for (auto iterator = newEnd + 1; iterator < currentEnd; ++iterator)
				{
					DeleteIfExists(*iterator);
				}

				m_enemies.erase(newEnd, currentEnd);
			}
		}
		else
		{
			if (!IsEnemyAtPosition(enemyPosition))
			{
				m_enemies.push_back(CreateEnemyAtPosition(enemyPosition));
			}
		}
		break;

	case PlacableObject::Player:
		// You can't destroy the player
		if (!destroyObject)
		{
			m_player->SetGridCoordinates(mouseGridPosition.x, mouseGridPosition.y);
		}
		break;
	}
}

EnemyEntity* Game::CreateEnemyAtPosition(sf::Vector2i position)
{
	EnemyEntity* newEnemy = new EnemyEntity(*this);

	std::ostringstream stringStream;
	stringStream << "Enemy_" << m_enemies.size();

	newEnemy->SetName(stringStream.str());
	newEnemy->SetGridCoordinates(position.x, position.y);
	return newEnemy;
}

bool Game::IsEnemyAtPosition(sf::Vector2i position)
{
	for (Entity* enemy : m_enemies)
	{
		if (enemy->GridX == position.x && enemy->GridY == position.y)
		{
			return true;
		}
	}
	return false;
}

#pragma endregion

bool Game::TryParseVector2i(std::string& r_str, char delimiter, sf::Vector2i& o_result)
{
	std::stringstream stream(r_str);

	std::string tempString;

	int x, y;
	if (!std::getline(stream, tempString, delimiter))
	{
		return false;
	}

	x = std::stoi(tempString);

	if (!std::getline(stream, tempString, delimiter))
	{
		return false;
	}
	y = std::stoi(tempString);

	o_result = sf::Vector2i{ x, y };
	return true;
}

void Game::DeleteIfExists(void* ptr)
{
	if (ptr != nullptr)
	{
		delete ptr;
	}
}

