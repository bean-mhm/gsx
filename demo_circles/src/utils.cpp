#include "utils.h"

// STD
#include <cmath>
#include <cstdint>
#include <cstdlib>

static constexpr f32 uv_scale = 4.;

math::vec2 screen_to_uv(math::uvec2 coord, math::uvec2 res)
{
    return 2.f * uv_scale
        * (math::vec2(coord) + .5f - math::vec2(res) * .5f)
        / (f32)math::min(res.x, res.y);
}

f32 get_px2uv_ratio(math::uvec2 res)
{
    return uv_scale * 2.f / (f32)std::min(res.x, res.y);
}

f32 sd_circle(math::vec2 p, math::vec2 center, f32 radius)
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
