#pragma once

// STD
#include <cstdint>

// GSX
#include "gsx/gsx.h"

// Transform component
struct transform_t
{
    u32 owner;
    math::vec2 pos;
};

// Circle component
struct circle_t
{
    u32 owner;
    f32 radius;
};
