#include "Utils.h"
#include <algorithm>
#include <cmath>

void Utils::ClampMagnitude(sf::Vector2f& r_vec, float min, float max)
{
	float sqrMagnitude = SqrMagnitude(r_vec);
	bool tooShort = sqrMagnitude < min * min;
	bool tooLong = sqrMagnitude > max * max;
	if (!tooShort && !tooLong)
	{
		return;
	}
	float wantedMagnitude = tooShort ? min : max;
	r_vec = Normalize(r_vec) * wantedMagnitude;
}

float Utils::SqrMagnitude(const sf::Vector2f& vec)
{
	return SqrMagnitude(vec.x, vec.y);
}

float Utils::SqrMagnitude(float x, float y)
{
	return x * x + y * y;
}

float Utils::Magnitude(const sf::Vector2f& vec)
{
	return Magnitude(vec.x, vec.y);
}

float Utils::Magnitude(float x, float y)
{
	return std::sqrt(SqrMagnitude(x, y));
}

float Utils::SqrDistance(const sf::Vector2f& from, const sf::Vector2f& to)
{
	return SqrDistance(from.x, from.y, to.x, to.y);
}

float Utils::SqrDistance(float xFrom, float yFrom, float xTo, float yTo)
{
	return SqrMagnitude(xTo - xFrom, yTo - yFrom);
}

float Utils::Distance(const sf::Vector2f& from, const sf::Vector2f& to)
{
	return Distance(from.x, from.y, to.x, to.y);
}

float Utils::Distance(float xFrom, float yFrom, float xTo, float yTo)
{
	return std::sqrt(SqrDistance(xFrom, yFrom, xTo, yTo));
}

sf::Vector2f Utils::Normalize(const sf::Vector2f& vec)
{
	float magnitude = Magnitude(vec);
	return	sf::Vector2f{ vec.x / magnitude, vec.y / magnitude };
}

float Utils::Dot(const sf::Vector2f& a, const sf::Vector2f& b)
{
	return a.x * b.x + a.y * b.y;
}

float Utils::Angle(const sf::Vector2f& from, const sf::Vector2f& to)
{
	float num = std::sqrt(SqrMagnitude(from) * SqrMagnitude(to));
	if (num < 1E-15f)
	{
		return 0.0f;
	}

	float num2 = std::clamp(Dot(from, to) / num, -1.0f, 1.0f);
	return std::acos(num2);// *57.29578f;
}

float Utils::SignedAngle(const sf::Vector2f& from, const sf::Vector2f& to)
{
	float angle = Angle(from, to);
	float sign = Sign(from.x * to.y - from.y * to.x);
	return angle * sign;
}

float Utils::Sign(float val)
{
	return val < 0.0f ? -1.0f : 1.0f;
}

int Utils::Sign(int val)
{
	return val < 0 ? -1 : 1;
}

float Utils::SmoothLerp(float a, float b, float deltaTime, float halfLife)
{
	return b + (a - b) * exp2f(-deltaTime / halfLife);
}

sf::Vector2f Utils::SmoothLerp(const sf::Vector2f& a, const sf::Vector2f& b, float deltaTime, float halfLife)
{
	return 
	{
		SmoothLerp(a.x, b.x, deltaTime, halfLife),
		SmoothLerp(a.y, b.y, deltaTime, halfLife)
	};
}
