#pragma once
#include <SFML/Graphics.hpp>
#include "Timer.h"

class Camera
{
private:
	sf::View m_cameraView;
	sf::View m_unshakenView;
	Timer m_screenshakeTimer;
	Timer m_screenshakeFrequencyTimer;
	float m_currentShakeMagnitude;

public:
	Camera(sf::FloatRect startView);
	void ApplyCamera(sf::RenderWindow& r_window);
	void UpdateCamera(float deltaTime);
	void ScreenShake(float duration, float magnitude);
};

