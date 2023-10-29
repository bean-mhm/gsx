#pragma once

// GSX
#include "gsx/gsx.h"

// Boid
struct boid_t
{
    math::vec2 pos;
    math::vec2 vel;
};

// Attractor
struct attractor_t
{
    math::vec2 pos;
    f32 strength = 0;
};
