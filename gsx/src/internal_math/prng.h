#pragma once

// STD
#include <cstdint>

// Internal
#include "vec2.h"
#include "vec3.h"
#include "../internal_common/all.h"

namespace gsx::math
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
        no_copy_construct_no_assignment(prng_t);

        // 32-bit signed integer from -2^31 to 2^31-1
        i32 next_i32();

        // 32-bit signed integer from min to max
        i32 next_i32(i32 min, i32 max);

        // 32-bit unsigned integer from 0 to 2^32-1
        u32 next_u32();

        // 32-bit unsigned integer from min to max
        u32 next_u32(u32 min, u32 max);

        // 32-bit float from 0 to 1
        f32 next_f32();

        // 32-bit float from min to max
        f32 next_f32(f32 min, f32 max);

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
        u32 state[2];

        void init(u32 seed0, u32 seed1);

    };

}
