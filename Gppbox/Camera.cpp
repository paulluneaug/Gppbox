#include "Camera.h"
#include "Random.h"

Camera::Camera(sf::FloatRect startView) :
	m_cameraView(sf::View{startView}),
	m_unshakenView(m_cameraView),
	m_screenshakeTimer(Timer{ 0.0f, false }),
	m_screenshakeFrequencyTimer(Timer(0.02f, true)),
	m_currentShakeMagnitude(0.0f)
{
	m_screenshakeFrequencyTimer.Start();
}

void Camera::ApplyCamera(sf::RenderWindow& r_window)
{
	r_window.setView(m_cameraView);
}

void Camera::UpdateCamera(float deltaTime)
{
	if (m_screenshakeTimer.IsRunning()) 
	{
		if (m_screenshakeTimer.Update(deltaTime)) 
		{
			m_screenshakeTimer.Stop();
			m_cameraView.setCenter(m_unshakenView.getCenter());
			return;
		}

		if (m_screenshakeFrequencyTimer.Update(deltaTime)) 
		{
			sf::Vector2f offset =
			{
				Random::RandomFloat(-m_currentShakeMagnitude, m_currentShakeMagnitude),
				Random::RandomFloat(-m_currentShakeMagnitude, m_currentShakeMagnitude)
			};
			m_cameraView.setCenter(m_unshakenView.getCenter() + offset);
		}
	}
}

void Camera::ScreenShake(float duration, float magnitude)
{
	float timeLeft = m_screenshakeTimer.IsRunning() ? m_screenshakeTimer.GetTimeLeft() : 0.0f;
	if (timeLeft < duration) 
	{
		m_screenshakeTimer.SetDuration(duration);
		m_screenshakeTimer.Start();

		m_currentShakeMagnitude = magnitude;
	}
}
