
#include <array>
#include <imgui.h>
#include <vector>

#include "C.hpp"
#include "Game.hpp"

#include "HotReloadShader.hpp"
#include "Entity.h"
#include "GlobalParameters.h"


static int cols = 1280 / Consts::GRID_SIZE;
static int lastLine = 720 / Consts::GRID_SIZE - 1;

Game::Game(sf::RenderWindow* win) {
	this->m_window = win;
	m_background = sf::RectangleShape(Vector2f((float)win->getSize().x, (float)win->getSize().y));

	bool isOk = m_backgroundTexture.loadFromFile("res/bg_stars.png");
	if (!isOk) {
		printf("ERR : LOAD FAILED\n");
	}
	m_background.setTexture(&m_backgroundTexture);
	m_background.setSize(sf::Vector2f(1280, 720));

	m_backgroundShader = new HotReloadShader("res/bg.vert", "res/bg.frag");
	InitWalls();
	InitEntities();
}

Game::~Game()
{
	if (m_backgroundShader) 
	{
		delete m_backgroundShader;
	}
	for (Entity* entity : m_entities) 
	{
		if (entity) 
		{
			delete entity;
		}
	}

}

void Game::InitWalls()
{
	for (int i = 0; i < 1280 / Consts::GRID_SIZE; ++i)
		m_walls.push_back(Vector2i(i, lastLine));

	m_walls.push_back(Vector2i(0, lastLine - 1));
	m_walls.push_back(Vector2i(0, lastLine - 2));
	m_walls.push_back(Vector2i(0, lastLine - 3));

	m_walls.push_back(Vector2i(cols - 1, lastLine - 1));
	m_walls.push_back(Vector2i(cols - 1, lastLine - 2));
	m_walls.push_back(Vector2i(cols - 1, lastLine - 3));

	m_walls.push_back(Vector2i(cols >> 2, lastLine - 2));
	m_walls.push_back(Vector2i(cols >> 2, lastLine - 3));
	m_walls.push_back(Vector2i(cols >> 2, lastLine - 4));
	m_walls.push_back(Vector2i((cols >> 2) + 1, lastLine - 4));
	CacheWalls();
}

void Game::InitEntities()
{
	m_player = new Entity(*this, { 2, 1 });
	m_player->SetCoordinates(300.0f, 300.0f);
	m_entities.push_back(m_player);
}

void Game::CacheWalls()
{
	m_wallSprites.clear();
	for (Vector2i& w : m_walls) {
		sf::RectangleShape rect(Vector2f(16, 16));
		rect.setPosition((float)w.x * Consts::GRID_SIZE, (float)w.y * Consts::GRID_SIZE);
		rect.setFillColor(sf::Color(0x07ff07ff));
		m_wallSprites.push_back(rect);
	}
}

void Game::ProcessInput(sf::Event ev) {
	if (ev.type == sf::Event::Closed) {
		m_window->close();
		m_closing = true;
		return;
	}
	if (ev.type == sf::Event::KeyReleased) {


	}
}


static double g_time = 0.0;
static double g_tickTimer = 0.0;


void Game::PollInput(double dt) {

	float lateralSpeed = 8.0;
	float maxSpeed = 40.0;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q)) {
		m_player->Dx = std::clamp(m_player->Dx - lateralSpeed, -maxSpeed, maxSpeed);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
		m_player->Dx = std::clamp(m_player->Dx + lateralSpeed, -maxSpeed, maxSpeed);

	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::T)) {

	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) {
		if (!m_wasSpacePressed) {
			OnSpacePressed();
			m_wasSpacePressed = true;
		}
	}
	else {
		m_wasSpacePressed = false;
	}

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


	for (Entity* entity : m_entities) {
		entity->Update(dt);
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

	for (Entity* entity : m_entities) {
		entity->Draw(win);
	}


	afterParts.draw(win);
}

void Game::OnSpacePressed() {
	m_player->Jump();
}


bool Game::IsWall(int cx, int cy)
{
	for (Vector2i& w : m_walls) {
		if (w.x == cx && w.y == cy)
			return true;
	}
	return false;
}

void Game::DrawImGui()
{
	if (ImGui::CollapsingHeader("Global Paramaters")) 
	{
		ImGui::DragFloat("Gravity", &GlobalParameters::GRAVITY, 0.5f, 0.0f);
	}

	if (ImGui::CollapsingHeader("Entities"))
	{
		for (Entity* entity : m_entities) 
		{
			entity->DrawImGui();
		}
	}
}

