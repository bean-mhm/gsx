#include "prng.h"

// STD
#include <random>

// Internal
#include "utils.h"

namespace tef::math
{

    inline uint32_t rot(uint32_t x, uint32_t k)
    {
        return (x << k) | (x >> (32u - k));
    }

    prng_t::prng_t()
    {
        std::random_device rd;
        std::uniform_int_distribution<uint32_t> dist(0, UINT32_MAX);
        init(dist(rd), dist(rd));
    }

    prng_t::prng_t(uvec2 seed)
    {
        init(seed.x, seed.y);
    }

    prng_t::prng_t(vec2 seed)
    {
        seed *= vec2(10.258331f, 31.833125f);
        seed += 7.6476101531702f;
        init(
            *reinterpret_cast<uint32_t*>(&seed.x),
            *reinterpret_cast<uint32_t*>(&seed.y)
        );
    }

    prng_t::prng_t(vec3 seed)
    {
        seed.x *= seed.z + 10.258331f;
        seed.y *= seed.z + 31.833125f;
        seed += 3.49276101561702f;
        init(
            *reinterpret_cast<uint32_t*>(&seed.x),
            *reinterpret_cast<uint32_t*>(&seed.y)
        );
    }

    int32_t prng_t::next_i32()
    {
        uint32_t s0 = state[0];
        uint32_t s1 = state[1];
        uint32_t result = rot(s0 * 0x9e3779bbu, 5u) * 5u;
        s1 ^= s0;
        state[0] = rot(s0, 26u) ^ s1 ^ (s1 << 9u);
        state[1] = rot(s1, 13u);
        return *reinterpret_cast<int32_t*>(&result);
    }

    int32_t prng_t::next_i32(int32_t min, int32_t max)
    {
        return min + (next_u32() % ((uint32_t)(max - min) + 1u));
    }

    uint32_t prng_t::next_u32()
    {
        uint32_t s0 = state[0];
        uint32_t s1 = state[1];
        uint32_t result = rot(s0 * 0x9e3779bbu, 5u) * 5u;
        s1 ^= s0;
        state[0] = rot(s0, 26u) ^ s1 ^ (s1 << 9u);
        state[1] = rot(s1, 13u);
        return result;
    }

    uint32_t prng_t::next_u32(uint32_t min, uint32_t max)
    {
        return min + (next_u32() % (max - min + 1u));
    }

    float prng_t::next_float()
    {
        return (float)next_u32() / (float)(0xffffffffu);
    }

    float prng_t::next_float(float min, float max)
    {
        return min + (next_float() * (max - min));
    }

    vec2 prng_t::next_in_circle()
    {
        vec2 v;
        for (int i = 0; i < 100; i++)
        {
            v = vec2(
                2.f * next_float() - 1.f,
                2.f * next_float() - 1.f
            );
            if (length_squared(v) <= 1.f) return v;
        }
        return v;
    }

    vec3 prng_t::next_in_sphere()
    {
        vec3 v;
        for (int i = 0; i < 100; i++)
        {
            v = vec3(
                2.f * next_float() - 1.f,
                2.f * next_float() - 1.f,
                2.f * next_float() - 1.f
            );
            if (length_squared(v) <= 1.f) return v;
        }
        return v;
    }

    vec2 prng_t::next_on_circle()
    {
        vec2 v;
        float lensqr;
        for (int i = 0; i < 100; i++)
        {
            v = vec2(
                2.f * next_float() - 1.f,
                2.f * next_float() - 1.f
            );
            lensqr = length_squared(v);
            if (lensqr <= 1.f && lensqr != 0.f) break;
        }
        return v / sqrt(lensqr);
    }

    vec3 prng_t::next_on_sphere()
    {
        vec3 v;
        float lensqr;
        for (int i = 0; i < 100; i++)
        {
            v = vec3(
                2.f * next_float() - 1.f,
                2.f * next_float() - 1.f,
                2.f * next_float() - 1.f
            );
            lensqr = length_squared(v);
            if (lensqr <= 1.f && lensqr != 0.f) break;
        }
        return v / sqrt(lensqr);
    }

    vec2 prng_t::next_gauss()
    {
        float u1 = next_float();
        float u2 = next_float();
        float temp = sqrt(-2.f * log(u1));
        return vec2(temp * cos(tau * u2), temp * sin(tau * u2));
    }

    void prng_t::init(uint32_t seed0, uint32_t seed1)
    {
        seed0 += 193;
        seed1 += 7771;
        state[0] = (seed0 * 522133279u) ^ (seed1 * 93444155u);
        state[1] = (seed0 * 720880126u) ^ (seed1 * 1665791465u);
        next_u32();
    }

}
