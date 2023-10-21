#pragma once

// GSX
#include "gsx/gsx.h"

// Boid
struct c_boid
{
    math::vec2 pos;
    math::vec2 vel;
};

// Attractor
struct c_attractor
{
    math::vec2 pos;
    f32 strength = 0;
};
