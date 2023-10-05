#pragma once

// STD
#include <cstdint>

// TEF
#include "tef/ecs.h"
#include "tef/math.h"

// Transform component
struct c_transform
{
    uint32_t owner;
    math::vec2 pos;
};

// Circle component
struct c_circle
{
    uint32_t owner;
    float radius;
};
