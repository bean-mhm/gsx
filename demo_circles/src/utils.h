#pragma once

// TEF
#include "tef/math.h"

math::vec2 screen_to_uv(math::uvec2 coord, math::uvec2 res);
float get_px2uv_ratio(math::uvec2 res);
float sd_circle(math::vec2 p, math::vec2 center, float radius);

void clear_console();
