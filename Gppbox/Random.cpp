#include "Random.h"

uint32_t Random::s_seed = 0;

void Random::SetSeed(uint32_t newSeed)
{
    s_seed = newSeed;
}

float Random::RandomFloat01()
{
    return RandomFloat01(s_seed);
}

float Random::RandomFloat(float min, float max)
{
    return RandomFloat(s_seed, min, max);
}

int Random::RandomInt()
{
    return RandomInt(s_seed);
}

int Random::RandomInt(int min, int max)
{
    return RandomInt(s_seed, min, max);
}

uint32_t Random::RandomUInt()
{
    return RandomUInt(s_seed);
}

bool Random::RandomBool()
{
    return RandomBool(s_seed);
}

bool Random::RandomBoolProb(float probability)
{
    return RandomBoolProb(s_seed, probability);
}

uint32_t Random::Hash_Internal(uint32_t s)
{
    s ^= 2747636419u;
    s *= 2654435769u;
    s ^= s >> 16;
    s *= 2654435769u;
    s ^= s >> 16;
    s *= 2654435769u;
    return s;
}

uint32_t Random::Hash(uint32_t& r_seed)
{
    r_seed = Hash_Internal(r_seed);
    return r_seed;
}

float Random::RandomFloat01(uint32_t& r_seed)
{
    return Hash(r_seed) / 4294967295.0f; // 2^32-1
}

float Random::RandomFloat(uint32_t& r_seed, float min, float max)
{
    return Utils::RemapFrom01(RandomFloat01(r_seed), min, max);
}

int Random::RandomInt(uint32_t& r_seed)
{
    return Hash(r_seed);
}

int Random::RandomInt(uint32_t& r_seed, int min, int max)
{
    return int(Utils::RemapFrom01(RandomFloat01(r_seed), min, max));
}

uint32_t Random::RandomUInt(uint32_t& r_seed)
{
    return Hash(r_seed);
}

bool Random::RandomBool(uint32_t& r_seed)
{
    return RandomBoolProb(r_seed, 0.5f);
}

bool Random::RandomBoolProb(uint32_t& r_seed, float probability)
{
    return RandomFloat01(r_seed) < probability;
}
