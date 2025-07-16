#pragma once
#include <cstdint>
#include "Utils.h"
class Random
{
private:
    static uint32_t s_seed;

public:
    static void SetSeed(uint32_t newSeed);

    static float RandomFloat01();

    static float RandomFloat(float min, float max);

    static int RandomInt();

    static int RandomInt(int min, int max);

    static uint32_t RandomUInt();

    static bool RandomBool();

    static bool RandomBoolProb(float probability);

private :

    static uint32_t Hash_Internal(uint32_t s);

    static uint32_t Hash(uint32_t& r_seed);

    static float RandomFloat01(uint32_t& r_seed);

    static float RandomFloat(uint32_t& r_seed, float min, float max);

    static int RandomInt(uint32_t& r_seed);

    static int RandomInt(uint32_t& r_seed, int min, int max);

    static uint32_t RandomUInt(uint32_t& r_seed);

    static bool RandomBool(uint32_t& r_seed);

    static bool RandomBoolProb(uint32_t& r_seed, float probability);

};

