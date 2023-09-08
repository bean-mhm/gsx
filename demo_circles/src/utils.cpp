#include "utils.h"

#include <cmath>

static constexpr float uv_scale = 4.;

vec2 screen_to_uv(ivec2 coord, ivec2 res)
{
    return vec2
    {
        uv_scale * 2.f * (coord.x + .5f - res.x * .5f) / std::min(res.x, res.y),
        uv_scale * 2.f * (coord.y + .5f - res.y * .5f) / std::min(res.x, res.y)
    };
}

float get_px2uv_ratio(ivec2 res)
{
    return uv_scale * 2.f / std::min(res.x, res.y);
}

float sd_circle(vec2 p, vec2 center, float radius)
{
    vec2 v = { center.x - p.x, center.y - p.y };
    return std::sqrtf(v.x * v.x + v.y * v.y) - radius;
}
