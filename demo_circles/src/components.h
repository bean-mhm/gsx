#pragma once

// TEF
#include "tef/tef.h"

// GLM
#include "glm/glm.hpp"

// Transform component
struct c_transform : tef::base_component_t
{
    glm::vec2 pos;
};

// Circle component
struct c_circle : tef::base_component_t
{
    float radius;
};
