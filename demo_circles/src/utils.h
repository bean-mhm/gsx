#pragma once

#include "glm/glm.hpp"

glm::vec2 screen_to_uv(glm::uvec2 coord, glm::uvec2 res);
float get_px2uv_ratio(glm::uvec2 res);

// Signed distance of a circle
float sd_circle(glm::vec2 p, glm::vec2 center, float radius);
