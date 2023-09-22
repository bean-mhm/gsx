#include "utils.h"

// STD
#include <cmath>
#include <cstdint>
#include <cstdlib>

static constexpr float uv_scale = 4.;

math::vec2 screen_to_uv(math::uvec2 coord, math::uvec2 res)
{
    return 2.f * uv_scale
        * (math::vec2(coord) + .5f - math::vec2(res) * .5f)
        / (float)math::min(res.x, res.y);
}

float get_px2uv_ratio(math::uvec2 res)
{
    return uv_scale * 2.f / (float)std::min(res.x, res.y);
}

float sd_circle(math::vec2 p, math::vec2 center, float radius)
{
    return math::distance(p, center) - radius;
}

void clear_console()
{
#if defined(_WIN32) || defined(_WIN64) || defined(WINDOWS)
    std::system("cls");
#else
    std::system("clear");
#endif
}
