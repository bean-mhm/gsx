#include "utils.h"

#include <cmath>
#include <cstdint>
#include <cstdlib>

static constexpr float uv_scale = 4.;

glm::vec2 screen_to_uv(glm::uvec2 coord, glm::uvec2 res)
{
    return 2.f * uv_scale * (glm::vec2(coord) + .5f - glm::vec2(res) * .5f) / (float)std::min(res.x, res.y);
}

float get_px2uv_ratio(glm::uvec2 res)
{
    return uv_scale * 2.f / (float)std::min(res.x, res.y);
}

float sd_circle(glm::vec2 p, glm::vec2 center, float radius)
{
    return glm::distance(p, center) - radius;
}

void clear_console()
{
#if defined(_WIN32) || defined(_WIN64) || defined(WINDOWS)
    std::system("cls");
#else
    std::system("clear");
#endif
}
