#pragma once

#include <limits>
#include <algorithm>
#include <cmath>
#include <cstdint>

namespace tef::math
{

    static constexpr float infinity = std::numeric_limits<float>::infinity();
    static constexpr float epsilon = 1e-6f;
    static constexpr float sqrt2 = 1.414213562373095048801688724f;
    static constexpr float sqrt3 = 1.732050807568877293527446341f;
    static constexpr float e = 2.718281828459045235360287471f;
    static constexpr float pi = 3.141592653589793238462643383f;
    static constexpr float tau = 6.283185307179586476925286766f;
    static constexpr float pi_over_2 = 1.570796326794896619231321691f;
    static constexpr float deg2rad = 0.017453292519943295769236907f;
    static constexpr float rad2deg = 57.29577951308232087679815481f;

    bool solve_quadratic(float a, float b, float c, float& t0, float& t1);
    bool solve_linear_2x2(const float a[2][2], const float b[2], float& x0, float& x1);

    inline float radians(float degrees)
    {
        return degrees * deg2rad;
    }

    inline float degrees(float radians)
    {
        return radians * rad2deg;
    }

    inline float sin(float angle)
    {
        return std::sin(angle);
    }

    inline float cos(float angle)
    {
        return std::cos(angle);
    }

    inline float tan(float angle)
    {
        return std::tan(angle);
    }

    inline float asin(float x)
    {
        return std::asin(x);
    }

    inline float acos(float x)
    {
        return std::acos(x);
    }

    inline float atan(float y, float x)
    {
        return std::atan2(y, x);
    }

    inline float atan(float y_over_x)
    {
        return std::atan(y_over_x);
    }

    inline float sinh(float x)
    {
        return std::sinh(x);
    }

    inline float cosh(float x)
    {
        return std::cosh(x);
    }

    inline float tanh(float x)
    {
        return std::tanh(x);
    }

    inline float asinh(float x)
    {
        return std::asinh(x);
    }

    inline float acosh(float x)
    {
        return std::acosh(x);
    }

    inline float atanh(float x)
    {
        return std::atanh(x);
    }

    inline float pow(float x, float y)
    {
        return std::pow(x, y);
    }

    inline float exp(float x)
    {
        return std::exp(x);
    }

    inline float log(float x)
    {
        return std::log(x);
    }

    inline float exp2(float x)
    {
        return std::exp2(x);
    }

    inline float log2(float x)
    {
        return std::log2(x);
    }

    inline float sqrt(float x)
    {
        return std::sqrt(x);
    }

    inline float inversesqrt(float x)
    {
        return 1.f / std::sqrt(x);
    }

    inline float abs(float x)
    {
        return std::abs(x);
    }

    inline float sign(float x)
    {
        if (x > 0) return 1.f;
        if (x == 0) return 0.f;
        if (x < 0) return -1.f;
    }

    inline float floor(float x)
    {
        return std::floor(x);
    }

    inline float ceil(float x)
    {
        return std::ceil(x);
    }

    inline float trunc(float x)
    {
        return std::trunc(x);
    }

    inline float fract(float x)
    {
        return x - std::floor(x);
    }

    inline float mod(float x, float y)
    {
        return std::fmod(x, y);
    }

    inline float modf(float x, float& i)
    {
        return std::modf(x, &i);
    }

    inline float wrap(float x, float start, float end)
    {
        return start + std::fmod(x - start, end - start);
    }

    inline float min(float x, float y)
    {
        return std::min(x, y);
    }

    inline float max(float x, float y)
    {
        return std::max(x, y);
    }

    inline float clamp(float x, float min, float max)
    {
        return std::clamp(x, min, max);
    }

    inline float clamp01(float x)
    {
        return std::clamp(x, 0.f, 1.f);
    }

    inline float mix(float x, float y, float a)
    {
        return x + a * (y - x);
    }

    inline float remap(float x, float a_start, float a_end, float b_start, float b_end)
    {
        return b_start + ((b_end - b_start) / (a_end - a_start)) * (x - a_start);
    }

    inline float remap_clamp(float x, float a_start, float a_end, float b_start, float b_end)
    {
        float t = clamp01((x - a_start) / (a_end - a_start));
        return b_start + t * (b_end - b_start);
    }

    inline float remap01(float x, float a_start, float a_end)
    {
        return clamp01((x - a_start) / (a_end - a_start));
    }

    inline float step(float edge, float x)
    {
        if (x < edge) return 0.f;
        return 1.f;
    }

    inline float smoothstep(float edge0, float edge1, float x)
    {
        float t = clamp01((x - edge0) / (edge1 - edge0));
        return t * t * (3.f - 2.f * t);
    }

    inline bool isnan(float x)
    {
        return std::isnan(x);
    }

    inline bool isinf(float x)
    {
        return std::isinf(x);
    }

    template <typename T>
    inline uint64_t upper_power_of_2(T v)
    {
        return (uint64_t)std::pow(2., std::ceil(std::log2((double)v)));
    }

    inline uint32_t i32_to_u32(int v)
    {
        return std::max(v, 0);
    }

    inline int u32_to_i32(uint32_t v)
    {
        return std::min(v, (uint32_t)INT_MAX);
    }

}
