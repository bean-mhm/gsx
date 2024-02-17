#pragma once

#include <limits>
#include <algorithm>
#include <cmath>
#include <cstdint>

#include "../internal_common/all.h"

namespace gsx::math
{

    static constexpr f32 infinity = std::numeric_limits<f32>::infinity();
    static constexpr f32 epsilon = 1e-5f;
    static constexpr f32 sqrt2 = 1.414213562373095048801688724f;
    static constexpr f32 sqrt3 = 1.732050807568877293527446341f;
    static constexpr f32 e = 2.718281828459045235360287471f;
    static constexpr f32 pi = 3.141592653589793238462643383f;
    static constexpr f32 tau = 6.283185307179586476925286766f;
    static constexpr f32 pi_over_2 = 1.570796326794896619231321691f;
    static constexpr f32 deg2rad = 0.017453292519943295769236907f;
    static constexpr f32 rad2deg = 57.29577951308232087679815481f;

    constexpr f32 radians(f32 degrees)
    {
        return degrees * deg2rad;
    }

    constexpr f32 degrees(f32 radians)
    {
        return radians * rad2deg;
    }

    inline f32 sin(f32 angle)
    {
        return std::sin(angle);
    }

    inline f32 cos(f32 angle)
    {
        return std::cos(angle);
    }

    inline f32 tan(f32 angle)
    {
        return std::tan(angle);
    }

    inline f32 asin(f32 x)
    {
        return std::asin(x);
    }

    inline f32 acos(f32 x)
    {
        return std::acos(x);
    }

    inline f32 atan(f32 y, f32 x)
    {
        return std::atan2(y, x);
    }

    inline f32 atan(f32 y_over_x)
    {
        return std::atan(y_over_x);
    }

    inline f32 sinh(f32 x)
    {
        return std::sinh(x);
    }

    inline f32 cosh(f32 x)
    {
        return std::cosh(x);
    }

    inline f32 tanh(f32 x)
    {
        return std::tanh(x);
    }

    inline f32 asinh(f32 x)
    {
        return std::asinh(x);
    }

    inline f32 acosh(f32 x)
    {
        return std::acosh(x);
    }

    inline f32 atanh(f32 x)
    {
        return std::atanh(x);
    }

    template<typename T>
    inline T pow(T x, T y)
    {
        return std::pow(x, y);
    }

    inline f32 exp(f32 x)
    {
        return std::exp(x);
    }

    inline f32 log(f32 x)
    {
        return std::log(x);
    }

    inline f32 exp2(f32 x)
    {
        return std::exp2(x);
    }

    inline f32 log2(f32 x)
    {
        return std::log2(x);
    }

    inline f32 squared(f32 x)
    {
        return x * x;
    }

    inline f32 sqrt(f32 x)
    {
        return std::sqrt(x);
    }

    inline f32 inversesqrt(f32 x)
    {
        return 1.f / std::sqrt(x);
    }

    template<typename T>
    inline T abs(T x)
    {
        return std::abs(x);
    }

    template<typename T>
    constexpr T sign(T x)
    {
        if (x > 0) return 1;
        if (x == 0) return 0;
        if (x < 0) return -1;
    }

    template<typename T>
    inline T floor(T x)
    {
        return std::floor(x);
    }

    template<typename T>
    inline T ceil(T x)
    {
        return std::ceil(x);
    }

    template<typename T>
    inline T trunc(T x)
    {
        return std::trunc(x);
    }

    template<typename T>
    inline T fract(T x)
    {
        return x - std::floor(x);
    }

    template<typename T>
    inline T mod(T x, T y)
    {
        return std::fmod(x, y);
    }

    inline f32 modf(f32 x, f32& i)
    {
        return std::modf(x, &i);
    }

    template<typename T>
    inline T wrap(T x, T start, T end)
    {
        return start + std::fmod(x - start, end - start);
    }

    template<typename T>
    constexpr T min(T x, T y)
    {
        return std::min(x, y);
    }

    template<typename T>
    constexpr T max(T x, T y)
    {
        return std::max(x, y);
    }

    template<typename T>
    constexpr T clamp(T x, T min, T max)
    {
        return std::clamp(x, min, max);
    }

    constexpr f32 clamp01(f32 x)
    {
        return std::clamp(x, 0.f, 1.f);
    }

    constexpr f32 mix(f32 x, f32 y, f32 a)
    {
        return x + a * (y - x);
    }

    constexpr f32 remap(f32 x, f32 a_start, f32 a_end, f32 b_start, f32 b_end)
    {
        return
            b_start + ((b_end - b_start) / (a_end - a_start)) * (x - a_start);
    }

    constexpr f32 remap_clamp(
        f32 x,
        f32 a_start,
        f32 a_end,
        f32 b_start,
        f32 b_end
    )
    {
        f32 t = clamp01((x - a_start) / (a_end - a_start));
        return b_start + t * (b_end - b_start);
    }

    constexpr f32 remap01(f32 x, f32 a_start, f32 a_end)
    {
        return clamp01((x - a_start) / (a_end - a_start));
    }

    template<typename T>
    constexpr T step(T edge, T x)
    {
        if (x < edge) return 0;
        return 1;
    }

    constexpr f32 smoothstep(f32 edge0, f32 edge1, f32 x)
    {
        f32 t = clamp01((x - edge0) / (edge1 - edge0));
        return t * t * (3.f - 2.f * t);
    }

    inline bool isnan(f32 x)
    {
        return std::isnan(x);
    }

    inline bool isinf(f32 x)
    {
        return std::isinf(x);
    }

    template<typename T>
    inline u64 upper_power_of_2(T v)
    {
        return (u64)std::pow(2., std::ceil(std::log2((f64)v)));
    }

    constexpr u32 i32_to_u32(i32 v)
    {
        return std::max(v, 0);
    }

    constexpr i32 u32_to_i32(u32 v)
    {
        return std::min(v, (u32)INT_MAX);
    }

    inline bool solve_quadratic(f32 a, f32 b, f32 c, f32& t0, f32& t1)
    {
        f32 discrim = b * b - 4 * a * c;
        if (discrim < 0)
            return false;
        f32 root_discrim = sqrt(discrim);
        f32 q;
        if (b < 0)
        {
            q = -.5f * (b - root_discrim);
        }
        else
        {
            q = -.5f * (b + root_discrim);
        }
        t0 = q / a;
        t1 = c / q;
        if (t0 > t1)
            std::swap(t0, t1);
        return true;
    }

    inline bool solve_linear_2x2(
        const f32 a[2][2],
        const f32 b[2],
        f32& x0,
        f32& x1
    )
    {
        f32 det = a[0][0] * a[1][1] - a[0][1] * a[1][0];
        if (abs(det) < 1e-10f)
            return false;
        x0 = (a[1][1] * b[0] - a[0][1] * b[1]) / det;
        x1 = (a[0][0] * b[1] - a[1][0] * b[0]) / det;
        if (isnan(x0) || isnan(x1))
            return false;
        return true;
    }

}
