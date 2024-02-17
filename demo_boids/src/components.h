#pragma once

#include "gsx/gsx.h"

struct boid_t
{
    math::vec2 pos;
    math::vec2 vel;
};

struct attractor_t
{
    math::vec2 pos;
    f32 strength = 0;
};
