#pragma once

#include <cstdint>

#include "gsx/gsx.h"

struct transform_t
{
    u32 owner;
    math::vec2 pos;
};

struct circle_t
{
    u32 owner;
    f32 radius;
};
