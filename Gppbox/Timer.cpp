#include "Timer.h"

#include <algorithm>

Timer::Timer(float duration, bool repeat, float startTime) :
	m_duration(std::max(0.0f, duration)),
	m_repeat(repeat),
	m_elapsedTime(startTime)
{
}

bool Timer::IsRunning() const
{
	return m_isRunning;
}

float Timer::GetDuration() const
{
	return m_duration;
}

void Timer::SetDuration(float duration)
{
	m_duration = duration;
	Reset();
}

float Timer::GetProgress() const
{
	return m_elapsedTime / m_duration;
}

float Timer::GetTimeLeft() const
{
	return m_duration - m_elapsedTime;
}

void Timer::Start()
{
	Pause(false);
}

void Timer::Stop()
{
	Pause(true);
	Reset();
}

void Timer::Pause(bool pause)
{
	m_isRunning = !pause;
}

void Timer::Reset()
{
	m_elapsedTime = 0.0f;
}

bool Timer::Update(float deltaTime)
{
	if (!m_isRunning)
	{
		return false;
	}

	m_elapsedTime += deltaTime;
	if (m_elapsedTime < m_duration)
	{
		return false;
	}

	if (m_repeat)
	{
		m_elapsedTime -= m_duration;
	}
	else
	{
		Stop();
	}
	return true;
}
