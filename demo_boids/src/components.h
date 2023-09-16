#pragma once

// TEF
#include "tef/tef.h"

// GLM
#include "glm/glm.hpp"

// Boid component
struct c_boid : tef::base_component_t
{
    glm::vec2 pos;
    glm::vec2 vel;
};
