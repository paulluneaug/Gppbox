#include "Utils.h"
#include <cmath>
#include <algorithm>

void Vector2fUtils::ClampMagnitude(sf::Vector2f& r_vec, float min, float max)
{
	float sqrMagnitude = SqrMagnitude(r_vec);
	bool tooShort = sqrMagnitude < min * min;
	bool tooLong = sqrMagnitude > max * max;
	if (!tooShort && !tooLong)
	{
		return;
	}
	float wantedMagnitude = tooShort ? min : max;
	r_vec = Normalized(r_vec) * wantedMagnitude;
}

float Vector2fUtils::SqrMagnitude(const sf::Vector2f& vec)
{
	return vec.x * vec.x + vec.y * vec.y;
}

float Vector2fUtils::Magnitude(const sf::Vector2f& vec)
{
	return std::sqrt(SqrMagnitude(vec));
}

float Vector2fUtils::SqrDistance(const sf::Vector2f& from, const sf::Vector2f& to)
{
	return SqrMagnitude(to - from);
}

float Vector2fUtils::Distance(const sf::Vector2f& from, const sf::Vector2f& to)
{
	return std::sqrt(SqrDistance(from, to));
}

sf::Vector2f Vector2fUtils::Normalized(const sf::Vector2f& vec)
{
	float magnitude = Magnitude(vec);
	return	sf::Vector2f{ vec.x / magnitude, vec.y / magnitude };
}

float Vector2fUtils::Dot(const sf::Vector2f& a, const sf::Vector2f& b)
{
	return a.x * b.x + a.y * b.y;
}

float Vector2fUtils::Angle(const sf::Vector2f& from, const sf::Vector2f& to)
{
	float num = std::sqrt(SqrMagnitude(from) * SqrMagnitude(to));
	if (num < 1E-15f)
	{
		return 0.0f;
	}

	float num2 = std::clamp(Dot(from, to) / num, -1.0f, 1.0f);
	return std::acos(num2) * 57.29578f;
}
