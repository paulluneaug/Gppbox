#pragma once
#include <SFML/System/Vector2.hpp>

class Vector2fUtils
{
public:
	static void ClampMagnitude(sf::Vector2f& r_vec, float min, float max);
    static float SqrMagnitude(const sf::Vector2f& vec);
    static float Magnitude(const sf::Vector2f& vec);
    static float SqrDistance(const sf::Vector2f& from, const sf::Vector2f& to);
    static float Distance(const sf::Vector2f& from, const sf::Vector2f& to);
    static sf::Vector2f Normalized(const sf::Vector2f& vec);

    static float Dot(const sf::Vector2f& a, const sf::Vector2f& b);
    static float Angle(const sf::Vector2f& from, const sf::Vector2f& to);
};

