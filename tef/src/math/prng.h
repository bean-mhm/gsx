#pragma once

// STD
#include <cstdint>

// Internal
#include "vec2.h"
#include "vec3.h"
#include "../misc.h"

namespace tef::math
{

    // Pseudo-Random Number Generator
    class prng_t
    {
    public:
        // Constructors
        prng_t();
        prng_t(uvec2 seed);
        prng_t(vec2 seed);
        prng_t(vec3 seed);
        no_copy_move_constructor(prng_t);

        // 32-bit signed integer from -2^31 to 2^31-1
        int32_t next_i32();

        // 32-bit signed integer from min to max
        int32_t next_i32(int32_t min, int32_t max);

        // 32-bit unsigned integer from 0 to 2^32-1
        uint32_t next_u32();

        // 32-bit unsigned integer from min to max
        uint32_t next_u32(uint32_t min, uint32_t max);

        // 32-bit float from 0 to 1
        float next_float();

        // 32-bit float from min to max
        float next_float(float min, float max);

        // 2D point inside the unit circle
        vec2 next_in_circle();

        // 3D point inside the unit sphere
        vec3 next_in_sphere();

        // 2D point on the unit circle, distanced 1 unit away from the origin
        vec2 next_on_circle();

        // 3D point on the unit sphere, distanced 1 unit away from the origin
        vec3 next_on_sphere();

        // Normally distributed 2D point centered around the origin (using the Box-Muller
        // Transform)
        // https://www.baeldung.com/cs/uniform-to-normal-distribution
        vec2 next_gauss();

    private:
        uint32_t state[2];

        void init(uint32_t seed0, uint32_t seed1);

    };

}
