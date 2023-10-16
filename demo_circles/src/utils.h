#pragma once

// ECX
#include "ecx/ecx.h"

math::vec2 screen_to_uv(math::uvec2 coord, math::uvec2 res);
f32 get_px2uv_ratio(math::uvec2 res);
f32 sd_circle(math::vec2 p, math::vec2 center, f32 radius);

void clear_console();
