#pragma once

#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>

#include "SFML/Graphics.hpp"
#include "SFML/System.hpp"
#include "SFML/Window.hpp"

#include "sys.hpp"

#include "Particle.hpp"
#include "ParticleManager.hpp"


class Entity;
class EnemyEntity;
class PlayerEntity;

class HotReloadShader;

class Game {
private:
	enum PlacableObject 
	{
		None = 0,
		Wall = 1,
		Enemy = 2,
		Player = 3,
	};

public:
	sf::RenderWindow*				m_window = nullptr;
	bool							m_closing = false;

	// Background
	sf::RectangleShape				m_background;
	HotReloadShader *				m_backgroundShader = nullptr;
	sf::Texture						m_backgroundTexture;

	
	std::vector<sf::Vector2i>		m_walls;
	std::vector<sf::RectangleShape> m_wallSprites;

private:
	PlayerEntity* m_player;
	std::vector<Entity*> m_enemies;

	// Edit Mode
	static constexpr char DELIMITER = ' ';
	bool m_editMode;
	PlacableObject m_selectedObject;


public:
	ParticleManager beforeParts;
	ParticleManager afterParts;

	Game(sf::RenderWindow * win);
	~Game();


	void CacheWalls();

	void ProcessInput(sf::Event ev);
	void PollInput(double dt);

	void Update(double dt);

	void Draw(sf::RenderWindow& win);

	bool IsWall(int cx, int cy);
	bool IsWall(sf::Vector2i position);

	bool CollidesWithEnemyAtPoint(float x, float y, Entity** o_hitEnemy);

	void DrawImGui();

private:

	void InitWalls();
	void InitPlayer();

	// Edit Mode
	void LoadLevel();
	void CreateEmptyLevel();
	void ReloadLevel();
	void SaveLevel();
	void ClearLevel();
	void EnterEditMode();
	void ExitEditMode();
	void ProcessMouseInput(sf::Mouse::Button pressedButton);
	void ProcessMouseInput_EditMode(sf::Mouse::Button pressedButton);
	EnemyEntity* CreateEnemyAtPosition(sf::Vector2i position);

	bool IsEnemyAtPosition(sf::Vector2i position);

	static bool TryParseVector2i(std::string& r_str, char delimiter, sf::Vector2i& o_result);
	static void DeleteIfExists(void* ptr);
};