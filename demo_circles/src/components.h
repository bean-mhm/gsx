#pragma once

// TEF
#include "tef/tef.h"

// Transform component
struct c_transform : tef::base_component_t
{
    math::vec2 pos;
};

// Circle component
struct c_circle : tef::base_component_t
{
    float radius;
};
