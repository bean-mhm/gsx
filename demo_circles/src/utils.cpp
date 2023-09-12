#include "utils.h"

#include <time.h>
#include <cmath>
#include <cstdint>

// https://stackoverflow.com/a/3418285/18049911
void str_replace_in_place(std::string& s, const std::string& from, const std::string& to)
{
    if (from.empty())
        return;

    uint64_t start_pos = 0;
    while ((start_pos = s.find(from, start_pos)) != std::string::npos)
    {
        s.replace(start_pos, from.length(), to);
        start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
    }
}

std::string str_replace(const std::string& s, const std::string& from, const std::string& to)
{
    std::string s_copy = s;
    str_replace_in_place(s_copy, from, to);
    return s_copy;
}

std::string str_from_time()
{
    time_t raw_time;
    tm time_info;
    char buffer[128];

    time(&raw_time);
    localtime_s(&time_info, &raw_time);
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &time_info);

    return std::string(buffer);
}

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
