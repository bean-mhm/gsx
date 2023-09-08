#pragma once

struct vec2
{
    float x = 0;
    float y = 0;
};

struct ivec2
{
    int x = 0;
    int y = 0;
};

vec2 screen_to_uv(ivec2 coord, ivec2 res);
float get_px2uv_ratio(ivec2 res);

// Signed distance of a circle
float sd_circle(vec2 p, vec2 center, float radius);
