#pragma once
#include <SFML/System/Vector2.hpp>
#include <vector>
#include <array>

class Utils
{
public:
    static constexpr float PI = 3.141592653589793f;
    static constexpr float DEG_2_RAD = PI / 180.0f;
    static constexpr float RAD_2_DEG = 180.0f / PI;

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

    /// <summary>
    /// Framerate independant lerp
    /// </summary>
    /// <remarks>https://bsky.app/profile/freya.bsky.social/post/3klfoezuzil2l</remarks>
    static float SmoothLerp(float a, float b, float deltaTime, float halfLife);
    static sf::Vector2f SmoothLerp(const sf::Vector2f& a, const sf::Vector2f& b, float deltaTime, float halfLife);

    static float RemapFrom01(float val, float targetMin, float targetMax);
    static float RemapTo01(float val, float initialMin, float initialMax);
    static float Remap(float val, float initialMin, float initialMax, float targetMin, float targetMax);


    static std::vector<std::array<float, 2>> Raycast(sf::Vector2f pos, sf::Vector2f dir, float maxDist, const sf::Vector2f& cellsSize = {1, 1});
};

