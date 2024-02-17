#include "prng.h"

#include <random>

#include "utils.h"

namespace gsx::math
{

    inline u32 rot(u32 x, u32 k)
    {
        return (x << k) | (x >> (32u - k));
    }

    prng_t::prng_t()
    {
        std::random_device rd;
        std::uniform_int_distribution<u32> dist(0, UINT32_MAX);
        init(dist(rd), dist(rd));
    }

    prng_t::prng_t(uvec2 seed)
    {
        init(seed.x, seed.y);
    }

    prng_t::prng_t(vec2 seed)
    {
        seed += 7.6476101531702f;
        seed *= vec2(10.258331f, 31.833125f);
        init(
            *reinterpret_cast<u32*>(&seed.x),
            *reinterpret_cast<u32*>(&seed.y)
        );
    }

    prng_t::prng_t(vec3 seed)
    {
        seed += 3.49276101561702f;
        seed.x *= seed.z + 10.258331f;
        seed.y *= seed.z + 31.833125f;
        init(
            *reinterpret_cast<u32*>(&seed.x),
            *reinterpret_cast<u32*>(&seed.y)
        );
    }

    i32 prng_t::next_i32()
    {
        u32 s0 = state[0];
        u32 s1 = state[1];
        u32 result = rot(s0 * 0x9e3779bbu, 5u) * 5u;
        s1 ^= s0;
        state[0] = rot(s0, 26u) ^ s1 ^ (s1 << 9u);
        state[1] = rot(s1, 13u);
        return *reinterpret_cast<i32*>(&result);
    }

    i32 prng_t::next_i32(i32 min, i32 max)
    {
        return min + (next_u32() % ((u32)(max - min) + 1u));
    }

    u32 prng_t::next_u32()
    {
        u32 s0 = state[0];
        u32 s1 = state[1];
        u32 result = rot(s0 * 0x9e3779bbu, 5u) * 5u;
        s1 ^= s0;
        state[0] = rot(s0, 26u) ^ s1 ^ (s1 << 9u);
        state[1] = rot(s1, 13u);
        return result;
    }

    u32 prng_t::next_u32(u32 min, u32 max)
    {
        return min + (next_u32() % (max - min + 1u));
    }

    f32 prng_t::next_f32()
    {
        return (f32)next_u32() / (f32)(0xffffffffu);
    }

    f32 prng_t::next_f32(f32 min, f32 max)
    {
        return min + (next_f32() * (max - min));
    }

    vec2 prng_t::next_in_circle()
    {
        vec2 v;
        for (i32 i = 0; i < 100; i++)
        {
            v = vec2(
                2.f * next_f32() - 1.f,
                2.f * next_f32() - 1.f
            );
            if (length_squared(v) <= 1.f) return v;
        }
        return v;
    }

    vec3 prng_t::next_in_sphere()
    {
        vec3 v;
        for (i32 i = 0; i < 100; i++)
        {
            v = vec3(
                2.f * next_f32() - 1.f,
                2.f * next_f32() - 1.f,
                2.f * next_f32() - 1.f
            );
            if (length_squared(v) <= 1.f) return v;
        }
        return v;
    }

    vec2 prng_t::next_on_circle()
    {
        vec2 v;
        f32 lensqr;
        for (i32 i = 0; i < 100; i++)
        {
            v = vec2(
                2.f * next_f32() - 1.f,
                2.f * next_f32() - 1.f
            );
            lensqr = length_squared(v);
            if (lensqr <= 1.f && lensqr != 0.f) break;
        }
        return v / sqrt(lensqr);
    }

    vec3 prng_t::next_on_sphere()
    {
        vec3 v;
        f32 lensqr;
        for (i32 i = 0; i < 100; i++)
        {
            v = vec3(
                2.f * next_f32() - 1.f,
                2.f * next_f32() - 1.f,
                2.f * next_f32() - 1.f
            );
            lensqr = length_squared(v);
            if (lensqr <= 1.f && lensqr != 0.f) break;
        }
        return v / sqrt(lensqr);
    }

    vec2 prng_t::next_gauss()
    {
        f32 u1 = next_f32();
        f32 u2 = next_f32();
        f32 temp = sqrt(-2.f * log(u1));
        return vec2(temp * cos(tau * u2), temp * sin(tau * u2));
    }

    void prng_t::init(u32 seed0, u32 seed1)
    {
        seed0 += 193;
        seed1 += 7771;
        state[0] = (seed0 * 522133279u) ^ (seed1 * 93444155u);
        state[1] = (seed0 * 720880126u) ^ (seed1 * 1665791465u);
        next_u32();
    }

}
