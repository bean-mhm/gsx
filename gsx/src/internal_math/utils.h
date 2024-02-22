#pragma once

#include <limits>
#include <algorithm>
#include <concepts>
#include <cmath>
#include <cstdint>

#include "../internal_common/all.h"

namespace gsx::math
{

    template<std::floating_point T>
    static constexpr T infinity = std::numeric_limits<T>::infinity();
    template<std::floating_point T>
    static constexpr T epsilon = 1e-5;
    template<std::floating_point T>
    static constexpr T sqrt2 = 1.414213562373095048801688724;
    template<std::floating_point T>
    static constexpr T sqrt3 = 1.732050807568877293527446341;
    template<std::floating_point T>
    static constexpr T e = 2.718281828459045235360287471;
    template<std::floating_point T>
    static constexpr T pi = 3.141592653589793238462643383;
    template<std::floating_point T>
    static constexpr T tau = 6.283185307179586476925286766;
    template<std::floating_point T>
    static constexpr T pi_over_2 = 1.570796326794896619231321691;
    template<std::floating_point T>
    static constexpr T deg2rad = 0.017453292519943295769236907;
    template<std::floating_point T>
    static constexpr T rad2deg = 57.29577951308232087679815481;

    template<std::floating_point T>
    constexpr T radians(T degrees)
    {
        return degrees * deg2rad<T>;
    }

    template<std::floating_point T>
    constexpr T degrees(T radians)
    {
        return radians * rad2deg<T>;
    }

    template<std::floating_point T>
    inline T sin(T angle)
    {
        return std::sin(angle);
    }

    template<std::floating_point T>
    inline T cos(T angle)
    {
        return std::cos(angle);
    }

    template<std::floating_point T>
    inline T tan(T angle)
    {
        return std::tan(angle);
    }

    template<std::floating_point T>
    inline T asin(T x)
    {
        return std::asin(x);
    }

    template<std::floating_point T>
    inline T acos(T x)
    {
        return std::acos(x);
    }

    template<std::floating_point T>
    inline T atan(T y, T x)
    {
        return std::atan2(y, x);
    }

    template<std::floating_point T>
    inline T atan(T y_over_x)
    {
        return std::atan(y_over_x);
    }

    template<std::floating_point T>
    inline T sinh(T x)
    {
        return std::sinh(x);
    }

    template<std::floating_point T>
    inline T cosh(T x)
    {
        return std::cosh(x);
    }

    template<std::floating_point T>
    inline T tanh(T x)
    {
        return std::tanh(x);
    }

    template<std::floating_point T>
    inline T asinh(T x)
    {
        return std::asinh(x);
    }

    template<std::floating_point T>
    inline T acosh(T x)
    {
        return std::acosh(x);
    }

    template<std::floating_point T>
    inline T atanh(T x)
    {
        return std::atanh(x);
    }

    template<std::floating_point T>
    inline T pow(T x, T y)
    {
        return std::pow(x, y);
    }

    template<std::floating_point T>
    inline T exp(T x)
    {
        return std::exp(x);
    }

    template<std::floating_point T>
    inline T log(T x)
    {
        return std::log(x);
    }

    template<std::floating_point T>
    inline T exp2(T x)
    {
        return std::exp2(x);
    }

    template<std::floating_point T>
    inline T log2(T x)
    {
        return std::log2(x);
    }

    template<std::floating_point T>
    inline T squared(T x)
    {
        return x * x;
    }

    template<std::floating_point T>
    inline T sqrt(T x)
    {
        return std::sqrt(x);
    }

    template<std::floating_point T>
    inline T inversesqrt(T x)
    {
        return (T)1 / std::sqrt(x);
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

    template<std::floating_point T>
    inline T floor(T x)
    {
        return std::floor(x);
    }

    template<std::floating_point T>
    inline T ceil(T x)
    {
        return std::ceil(x);
    }

    template<std::floating_point T>
    inline T trunc(T x)
    {
        return std::trunc(x);
    }

    template<std::floating_point T>
    inline T fract(T x)
    {
        return x - std::floor(x);
    }

    template<std::floating_point T>
    inline T mod(T x, T y)
    {
        return std::fmod(x, y);
    }

    template<std::floating_point T>
    inline T modf(T x, T& i)
    {
        return std::modf(x, &i);
    }

    template<std::floating_point T>
    inline T wrap(T x, T start, T end)
    {
        return start + std::fmod(x - start, end - start);
    }

    template<std::integral T>
    inline T wrap(T x, T start, T end)
    {
        return start + ((x - start) % (end - start));
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

    template<std::floating_point T>
    constexpr T clamp01(T x)
    {
        return std::clamp(x, (T)0, (T)1);
    }

    template<std::floating_point T>
    constexpr T mix(T x, T y, T a)
    {
        return x + a * (y - x);
    }

    template<std::floating_point T>
    constexpr T remap(T x, T a_start, T a_end, T b_start, T b_end)
    {
        return
            b_start + ((b_end - b_start) / (a_end - a_start)) * (x - a_start);
    }

    template<std::floating_point T>
    constexpr T remap_clamp(
        T x,
        T a_start,
        T a_end,
        T b_start,
        T b_end
    )
    {
        T t = clamp01((x - a_start) / (a_end - a_start));
        return b_start + t * (b_end - b_start);
    }

    template<std::floating_point T>
    constexpr T remap01(T x, T a_start, T a_end)
    {
        return clamp01((x - a_start) / (a_end - a_start));
    }

    template<typename T>
    constexpr T step(T edge, T x)
    {
        if (x < edge) return 0;
        return 1;
    }

    template<std::floating_point T>
    constexpr T smoothstep(T edge0, T edge1, T x)
    {
        T t = clamp01((x - edge0) / (edge1 - edge0));
        return t * t * ((T)3 - (T)2 * t);
    }

    template<std::floating_point T>
    inline bool isnan(T x)
    {
        return std::isnan(x);
    }

    template<std::floating_point T>
    inline bool isinf(T x)
    {
        return std::isinf(x);
    }

    template<typename T>
    inline u64 upper_power_of_2(T v)
    {
        return ((u64)1) << (u64)std::ceil(std::log2((f64)v));
    }

    template<typename T>
    inline u64 lower_power_of_2(T v)
    {
        return ((u64)1) << (u64)std::floor(std::log2((f64)v));
    }

    constexpr u32 i32_to_u32(i32 v)
    {
        return std::max(v, 0);
    }

    constexpr i32 u32_to_i32(u32 v)
    {
        return std::min(v, (u32)std::numeric_limits<i32>::max());
    }

    template<std::floating_point T>
    inline bool solve_quadratic(T a, T b, T c, T& t0, T& t1)
    {
        T discrim = b * b - (T)4. * a * c;
        if (discrim < 0)
            return false;
        T root_discrim = sqrt(discrim);
        T q;
        if (b < 0)
        {
            q = (T)(-.5) * (b - root_discrim);
        }
        else
        {
            q = (T)(-.5) * (b + root_discrim);
        }
        t0 = q / a;
        t1 = c / q;
        if (t0 > t1)
            std::swap(t0, t1);
        return true;
    }

    template<std::floating_point T>
    inline bool solve_linear_2x2(
        const T a[2][2],
        const T b[2],
        T& x0,
        T& x1
    )
    {
        T det = a[0][0] * a[1][1] - a[0][1] * a[1][0];
        if (abs(det) < (T)1e-10)
            return false;
        x0 = (a[1][1] * b[0] - a[0][1] * b[1]) / det;
        x1 = (a[0][0] * b[1] - a[1][0] * b[0]) / det;
        if (isnan(x0) || isnan(x1))
            return false;
        return true;
    }

}
