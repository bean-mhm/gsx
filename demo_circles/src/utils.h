#pragma once

#include <string>

#include "glm/glm.hpp"

void str_replace_in_place(std::string& s, const std::string& from, const std::string& to);
std::string str_replace(const std::string& s, const std::string& from, const std::string& to);
std::string str_from_time();

glm::vec2 screen_to_uv(glm::uvec2 coord, glm::uvec2 res);
float get_px2uv_ratio(glm::uvec2 res);

// Signed distance of a circle
float sd_circle(glm::vec2 p, glm::vec2 center, float radius);
