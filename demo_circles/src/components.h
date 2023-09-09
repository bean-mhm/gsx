#pragma once

#include "tef.h"

#include "utils.h"

// Transform component
struct c_transform : tef::component_t
{
    glm::vec2 pos;
};

// Circle component
struct c_circle : tef::component_t
{
    float radius;
};
