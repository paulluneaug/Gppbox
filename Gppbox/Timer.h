#pragma once

class Timer
{
private :

	float m_duration = 1.0f;
	bool m_repeat = false;


	float m_elapsedTime = 0.0f;
	bool m_isRunning = false;

public:
	Timer(float duration, bool repeat, float startTime = 0.0f);

	// Getters - Setters
	bool IsRunning() const;

	float GetDuration() const;
	void SetDuration(float duration);

	float GetProgress() const;
	float GetTimeLeft() const;

	void Start();
	void Stop();
	void Pause(bool pause);
	void Reset();

	bool Update(float deltaTime);
};