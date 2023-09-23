#pragma once

// STD
#include <cstdint>

// Internal
#include "vec2.h"
#include "vec3.h"
#include "utils.h"
#include "../utils.h"

namespace tef::math
{

    // Pseudo-Random Number Generator
    class prng_t
    {
    public:
        constexpr prng_t(uvec2 seed)
        {
            state[0] = seed.x;
            state[1] = seed.y;
            next_u32();
        }

        constexpr prng_t(vec2 seed)
        {
            seed += 7.6476101561702f;
            state[0] = *reinterpret_cast<uint32_t*>(&seed.x);
            state[1] = *reinterpret_cast<uint32_t*>(&seed.y);
            next_u32();
        }

        constexpr prng_t(vec3 seed)
        {
            seed.x *= seed.z + 10.258331f;
            seed.y *= seed.z + 31.833125f;
            seed += 3.49276101561702f;

            state[0] = *reinterpret_cast<uint32_t*>(&seed.x);
            state[1] = *reinterpret_cast<uint32_t*>(&seed.y);

            next_u32();
        }

        no_default_copy_move_constructor(prng_t);

        // 32-bit signed integer from -2^31 to 2^31-1
        constexpr int32_t next_i32()
        {
            uint32_t s0 = state[0];
            uint32_t s1 = state[1];
            uint32_t result = rot(s0 * 0x9e3779bbu, 5) * 5u;
            s1 ^= s0;
            state[0] = rot(s0, 26) ^ s1 ^ (s1 << 9);
            state[1] = rot(s1, 13);
            return *reinterpret_cast<int32_t*>(&result);
        }

        // 32-bit signed integer from min to max
        constexpr int32_t next_i32(int32_t min, int32_t max)
        {
            return min + (next_i32() % (max - min + 1));
        }

        // 32-bit unsigned integer from 0 to 2^32-1
        constexpr uint32_t next_u32()
        {
            uint32_t s0 = state[0];
            uint32_t s1 = state[1];
            uint32_t result = rot(s0 * 0x9e3779bbu, 5) * 5u;
            s1 ^= s0;
            state[0] = rot(s0, 26) ^ s1 ^ (s1 << 9);
            state[1] = rot(s1, 13);
            return result;
        }

        // 32-bit unsigned integer from min to max
        constexpr uint32_t next_u32(uint32_t min, uint32_t max)
        {
            return min + (next_u32() % (max - min + 1u));
        }

        // 32-bit float from 0 to 1
        constexpr float next_float()
        {
            return (float)next_u32() / (float)(0xffffffffu);
        }

        // 32-bit float from min to max
        constexpr float next_float(float min, float max)
        {
            return min + (next_float() * (max - min));
        }

        // 2D point inside the unit circle
        constexpr vec2 next_in_circle()
        {
            vec2 v = vec2(1);
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

        // 3D point inside the unit sphere
        constexpr vec3 next_in_sphere()
        {
            vec3 v = vec3(1);
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

        // 2D point on the unit circle, distanced 1 unit away from the origin
        constexpr vec2 next_on_circle()
        {
            vec2 v = vec2(1);
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

        // 3D point on the unit sphere, distanced 1 unit away from the origin
        constexpr vec3 next_on_sphere()
        {
            vec3 v = vec3(1);
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

        // Normally distributed 2D point centered around the origin (using the Box-Muller
        // Transform)
        // https://www.baeldung.com/cs/uniform-to-normal-distribution
        constexpr vec2 next_gauss()
        {
            float u1 = next_float();
            float u2 = next_float();
            float temp = sqrt(-2.f * log(u1));
            return vec2(temp * cos(tau * u2), temp * sin(tau * u2));
        }

    private:
        uint32_t state[2];

        constexpr uint32_t rot(uint32_t x, uint32_t k)
        {
            return (x << k) | (x >> (32 - k));
        }

    };

}
