#pragma once

#include <vector>

#include "SFML/Graphics.hpp"
#include "SFML/System.hpp"
#include "SFML/Window.hpp"

#include "sys.hpp"

#include "Particle.hpp"
#include "ParticleManager.hpp"


using namespace sf;

class Entity;
class HotReloadShader;
class Game {
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
	Entity* m_player;
	std::vector<Entity*> m_entities;

public:
	ParticleManager beforeParts;
	ParticleManager afterParts;


	bool m_wasSpacePressed = false;

	Game(sf::RenderWindow * win);
	~Game();


	void CacheWalls();

	void ProcessInput(sf::Event ev);
	void PollInput(double dt);
	void OnSpacePressed();

	void Update(double dt);

	void Draw(sf::RenderWindow& win);

	bool IsWall(int cx, int cy);
	void DrawImGui();

private:

	void InitWalls();
	void InitEntities();
};