#pragma once

// STD
#include <cstdint>

// ECX
#include "ecx/ecx.h"

// Transform component
struct c_transform
{
    u32 owner;
    math::vec2 pos;
};

// Circle component
struct c_circle
{
    u32 owner;
    f32 radius;
};
