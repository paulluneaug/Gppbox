#pragma once
#include <SFML/System/Vector2.hpp>

class Utils
{
public:
	static void ClampMagnitude(sf::Vector2f& r_vec, float min, float max);

    static float SqrMagnitude(const sf::Vector2f& vec);
    static float SqrMagnitude(float x, float y);
    static float Magnitude(const sf::Vector2f& vec);
    static float Magnitude(float x, float y);

    static float SqrDistance(const sf::Vector2f& from, const sf::Vector2f& to);
    static float SqrDistance(float xFrom, float yFrom, float xTo, float yTo);
    static float Distance(const sf::Vector2f& from, const sf::Vector2f& to);
    static float Distance(float xFrom, float yFrom, float xTo, float yTo);
    
    static sf::Vector2f Normalize(const sf::Vector2f& vec);

    static float Dot(const sf::Vector2f& a, const sf::Vector2f& b);
    static float Angle(const sf::Vector2f& from, const sf::Vector2f& to);
    static float SignedAngle(const sf::Vector2f& from, const sf::Vector2f& to);

    static float Sign(float val);
    static int Sign(int val);
};

