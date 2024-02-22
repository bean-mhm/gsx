#pragma once

#include "matrix.h"
#include "utils.h"
#include "../internal_common/all.h"
#include "../internal_str/all.h"

namespace gsx::math
{

    template<typename T>
    class base_vec2
    {
    public:
        T x = 0, y = 0;

        constexpr base_vec2() = default;

        constexpr base_vec2(T x, T y)
            : x(x), y(y)
        {}

        constexpr base_vec2(T s)
            : x(s), y(s)
        {}

        template<typename U>
        explicit constexpr base_vec2(const base_vec2<U>& v)
            : x((T)v.x), y((T)v.y)
        {}

        explicit constexpr base_vec2(const base_mat<T, 1, 2>& m)
            : x(m(0)), y(m(1))
        {}

        explicit constexpr base_vec2(const base_mat<T, 2, 1>& m)
            : x(m(0)), y(m(1))
        {}

        explicit constexpr operator base_mat<T, 1, 2>() const
        {
            return base_mat<T, 1, 2>({ x, y });
        }

        explicit constexpr operator base_mat<T, 2, 1>() const
        {
            return base_mat<T, 2, 1>({ x, y });
        }

        std::string to_string() const
        {
            return std::format(
                "[{}, {}]",
                str::from_number(x),
                str::from_number(y)
            );
        }

        friend std::ostream& operator<<(std::ostream& os, const base_vec2<T>& v)
        {
            os << v.to_string();
            return os;
        }

        constexpr base_vec2<T> operator+(const base_vec2<T>& v) const
        {
            return base_vec2<T>(x + v.x, y + v.y);
        }

        constexpr base_vec2<T>& operator+=(const base_vec2<T>& v)
        {
            x += v.x;
            y += v.y;
            return *this;
        }

        constexpr base_vec2<T> operator-(const base_vec2<T>& v) const
        {
            return base_vec2<T>(x - v.x, y - v.y);
        }

        constexpr base_vec2<T>& operator-=(const base_vec2<T>& v)
        {
            x -= v.x;
            y -= v.y;
            return *this;
        }

        constexpr base_vec2<T> operator*(T s) const
        {
            return base_vec2<T>(s * x, s * y);
        }

        constexpr base_vec2<T>& operator*=(T s)
        {
            x *= s;
            y *= s;
            return *this;
        }

        constexpr base_vec2<T> operator*(base_vec2<T> v) const
        {
            return base_vec2<T>(x * v.x, y * v.y);
        }

        constexpr base_vec2<T>& operator*=(base_vec2<T> v)
        {
            x *= v.x;
            y *= v.y;
            return *this;
        }

        constexpr base_vec2<T> operator/(T s) const
        {
            if constexpr (std::is_floating_point_v<T>)
            {
                T inv = (T)1 / s;
                return base_vec2<T>(x * inv, y * inv);
            }
            else
            {
                return base_vec2<T>(x / s, y / s);
            }
        }

        constexpr base_vec2<T>& operator/=(T s)
        {
            if constexpr (std::is_floating_point_v<T>)
            {
                T inv = (T)1 / s;
                x *= inv;
                y *= inv;
            }
            else
            {
                x /= s;
                y /= s;
            }
            return *this;
        }

        constexpr base_vec2<T> operator/(base_vec2<T> v) const
        {
            return base_vec2<T>(x / v.x, y / v.y);
        }

        constexpr base_vec2<T>& operator/=(base_vec2<T> v)
        {
            x /= v.x;
            y /= v.y;
            return *this;
        }

        template<typename = std::enable_if_t<std::is_integral_v<T>>>
        constexpr base_vec2<T> operator%(base_vec2<T> v) const
        {
            return base_vec2<T>(x % v.x, y % v.y);
        }

        template<typename = std::enable_if_t<std::is_integral_v<T>>>
        constexpr base_vec2<T>& operator%=(base_vec2<T> v)
        {
            x %= v.x;
            y %= v.y;
            return *this;
        }

        constexpr bool operator==(const base_vec2<T>& v) const
        {
            return x == v.x && y == v.y;
        }

        constexpr bool operator!=(const base_vec2<T>& v) const
        {
            return x != v.x || y != v.y;
        }

        constexpr base_vec2<T> operator-() const
        {
            return base_vec2<T>(-x, -y);
        }

        constexpr T operator[](i32 i) const
        {
            if (i == 0) return x;
            return y;
        }

        constexpr T& operator[](i32 i)
        {
            if (i == 0) return x;
            return y;
        }

        constexpr base_vec2<T> permute(i32 x, i32 y) const
        {
            return base_vec2<T>((*this)[x], (*this)[y]);
        }

        constexpr base_vec2<T> yx() const
        {
            return base_vec2<T>(y, x);
        }

        constexpr i32 n_components() const
        {
            return 2;
        }

    };

    template<typename T>
    constexpr base_vec2<T> operator+(T s, const base_vec2<T>& v)
    {
        return v + s;
    }

    template<typename T>
    constexpr base_vec2<T> operator-(T s, const base_vec2<T>& v)
    {
        return (-v) + s;
    }

    template<typename T>
    constexpr base_vec2<T> operator*(T s, const base_vec2<T>& v)
    {
        return v * s;
    }

    template<typename T>
    constexpr base_vec2<T> operator/(T s, const base_vec2<T>& v)
    {
        return base_vec2<T>(s / v.x, s / v.y);
    }

    template<std::floating_point T>
    constexpr base_vec2<T> radians(const base_vec2<T>& degrees)
    {
        return degrees * deg2rad;
    }

    template<std::floating_point T>
    constexpr base_vec2<T> degrees(const base_vec2<T>& radians)
    {
        return radians * rad2deg;
    }

    template<std::floating_point T>
    inline base_vec2<T> sin(const base_vec2<T>& v)
    {
        return base_vec2<T>(sin(v.x), sin(v.y));
    }

    template<std::floating_point T>
    inline base_vec2<T> cos(const base_vec2<T>& v)
    {
        return base_vec2<T>(cos(v.x), cos(v.y));
    }

    template<std::floating_point T>
    inline base_vec2<T> tan(const base_vec2<T>& v)
    {
        return base_vec2<T>(tan(v.x), tan(v.y));
    }

    template<std::floating_point T>
    inline base_vec2<T> asin(const base_vec2<T>& v)
    {
        return base_vec2<T>(asin(v.x), asin(v.y));
    }

    template<std::floating_point T>
    inline base_vec2<T> acos(const base_vec2<T>& v)
    {
        return base_vec2<T>(acos(v.x), acos(v.y));
    }

    template<std::floating_point T>
    inline base_vec2<T> atan(const base_vec2<T>& v)
    {
        return base_vec2<T>(atan(v.x), atan(v.y));
    }

    template<std::floating_point T>
    inline base_vec2<T> sinh(const base_vec2<T>& v)
    {
        return base_vec2<T>(sinh(v.x), sinh(v.y));
    }

    template<std::floating_point T>
    inline base_vec2<T> cosh(const base_vec2<T>& v)
    {
        return base_vec2<T>(cosh(v.x), cosh(v.y));
    }

    template<std::floating_point T>
    inline base_vec2<T> tanh(const base_vec2<T>& v)
    {
        return base_vec2<T>(tanh(v.x), tanh(v.y));
    }

    template<std::floating_point T>
    inline base_vec2<T> asinh(const base_vec2<T>& v)
    {
        return base_vec2<T>(asinh(v.x), asinh(v.y));
    }

    template<std::floating_point T>
    inline base_vec2<T> acosh(const base_vec2<T>& v)
    {
        return base_vec2<T>(acosh(v.x), acosh(v.y));
    }

    template<std::floating_point T>
    inline base_vec2<T> atanh(const base_vec2<T>& v)
    {
        return base_vec2<T>(atanh(v.x), atanh(v.y));
    }

    template<std::floating_point T>
    inline base_vec2<T> pow(const base_vec2<T>& v1, const base_vec2<T>& v2)
    {
        return base_vec2<T>(pow(v1.x, v2.x), pow(v1.y, v2.y));
    }

    template<std::floating_point T>
    inline base_vec2<T> pow(const base_vec2<T>& v1, T v2)
    {
        return base_vec2<T>(pow(v1.x, v2), pow(v1.y, v2));
    }

    template<std::floating_point T>
    inline base_vec2<T> exp(const base_vec2<T>& v)
    {
        return base_vec2<T>(exp(v.x), exp(v.y));
    }

    template<std::floating_point T>
    inline base_vec2<T> log(const base_vec2<T>& v)
    {
        return base_vec2<T>(log(v.x), log(v.y));
    }

    template<std::floating_point T>
    inline base_vec2<T> exp2(const base_vec2<T>& v)
    {
        return base_vec2<T>(exp2(v.x), exp2(v.y));
    }

    template<std::floating_point T>
    inline base_vec2<T> log2(const base_vec2<T>& v)
    {
        return base_vec2<T>(log2(v.x), log2(v.y));
    }

    template<std::floating_point T>
    inline base_vec2<T> sqrt(const base_vec2<T>& v)
    {
        return base_vec2<T>(sqrt(v.x), sqrt(v.y));
    }

    template<std::floating_point T>
    inline base_vec2<T> inversesqrt(const base_vec2<T>& v)
    {
        return base_vec2<T>(inversesqrt(v.x), inversesqrt(v.y));
    }

    template<typename T>
    inline base_vec2<T> abs(const base_vec2<T>& v)
    {
        return base_vec2<T>(abs(v.x), abs(v.y));
    }

    template<typename T>
    constexpr base_vec2<T> sign(const base_vec2<T>& v)
    {
        return base_vec2<T>(sign(v.x), sign(v.y));
    }

    template<std::floating_point T>
    inline base_vec2<T> floor(const base_vec2<T>& v)
    {
        return base_vec2<T>(floor(v.x), floor(v.y));
    }

    template<std::floating_point T>
    inline base_vec2<T> ceil(const base_vec2<T>& v)
    {
        return base_vec2<T>(ceil(v.x), ceil(v.y));
    }

    template<std::floating_point T>
    inline base_vec2<T> trunc(const base_vec2<T>& v)
    {
        return base_vec2<T>(trunc(v.x), trunc(v.y));
    }

    template<std::floating_point T>
    inline base_vec2<T> fract(const base_vec2<T>& v)
    {
        return base_vec2<T>(fract(v.x), fract(v.y));
    }

    template<std::floating_point T>
    inline base_vec2<T> mod(const base_vec2<T>& v1, const base_vec2<T>& v2)
    {
        return base_vec2<T>(mod(v1.x, v2.x), mod(v1.y, v2.y));
    }

    template<std::floating_point T>
    inline base_vec2<T> mod(const base_vec2<T>& v1, T v2)
    {
        return base_vec2<T>(mod(v1.x, v2), mod(v1.y, v2));
    }

    template<std::floating_point T>
    inline base_vec2<T> modf(const base_vec2<T>& v, base_vec2<T>& i)
    {
        return base_vec2<T>(modf(v.x, i.x), modf(v.y, i.y));
    }

    template<std::floating_point T>
    inline base_vec2<T> wrap(const base_vec2<T>& v, T start, T end)
    {
        return start + mod(v - start, end - start);
    }

    template<typename T>
    constexpr base_vec2<T> min(const base_vec2<T>& v1, const base_vec2<T>& v2)
    {
        return base_vec2<T>(
            min(v1.x, v2.x),
            min(v1.y, v2.y)
        );
    }

    template<typename T>
    constexpr base_vec2<T> max(const base_vec2<T>& v1, const base_vec2<T>& v2)
    {
        return base_vec2<T>(
            max(v1.x, v2.x),
            max(v1.y, v2.y)
        );
    }

    template<typename T>
    constexpr base_vec2<T> clamp(const base_vec2<T>& v, T min, T max)
    {
        return base_vec2<T>(clamp(v.x, min, max), clamp(v.y, min, max));
    }

    template<typename T>
    constexpr base_vec2<T> clamp(
        const base_vec2<T>& v,
        const base_vec2<T>& min,
        const base_vec2<T>& max
    )
    {
        return base_vec2<T>(
            clamp(v.x, min.x, max.x),
            clamp(v.y, min.y, max.y)
        );
    }

    template<std::floating_point T>
    constexpr base_vec2<T> clamp01(const base_vec2<T>& v)
    {
        return base_vec2<T>(clamp01(v.x), clamp01(v.y));
    }

    template<std::floating_point T>
    constexpr base_vec2<T> mix(
        const base_vec2<T>& v1,
        const base_vec2<T>& v2,
        T a
    )
    {
        return v1 + a * (v2 - v1);
    }

    template<std::floating_point T>
    constexpr base_vec2<T> remap(
        const base_vec2<T>& v,
        T a_start,
        T a_end,
        T b_start,
        T b_end
    )
    {
        return
            b_start + ((b_end - b_start) / (a_end - a_start)) * (v - a_start);
    }

    template<std::floating_point T>
    constexpr base_vec2<T> remap_clamp(
        const base_vec2<T>& v,
        T a_start,
        T a_end,
        T b_start,
        T b_end
    )
    {
        base_vec2<T> t = clamp01((v - a_start) / (a_end - a_start));
        return b_start + t * (b_end - b_start);
    }

    template<std::floating_point T>
    constexpr base_vec2<T> remap01(
        const base_vec2<T>& v,
        T a_start,
        T a_end
    )
    {
        return clamp01((v - a_start) / (a_end - a_start));
    }

    template<typename T>
    constexpr base_vec2<T> step(T edge, const base_vec2<T>& v)
    {
        return base_vec2<T>(step(edge, v.x), step(edge, v.y));
    }

    template<std::floating_point T>
    constexpr base_vec2<T> smoothstep(
        T edge0,
        T edge1,
        const base_vec2<T>& v
    )
    {
        return base_vec2<T>(
            smoothstep(edge0, edge1, v.x), smoothstep(edge0, edge1, v.y)
        );
    }

    template<typename T>
    constexpr T length_squared(const base_vec2<T>& v)
    {
        return v.x * v.x + v.y * v.y;
    }

    template<std::floating_point T>
    inline T length(const base_vec2<T>& v)
    {
        return sqrt(length_squared(v));
    }

    template<typename T>
    constexpr T distance_squared(
        const base_vec2<T>& v1,
        const base_vec2<T>& v2
    )
    {
        return length_squared(v1 - v2);
    }

    template<std::floating_point T>
    inline T distance(const base_vec2<T>& v1, const base_vec2<T>& v2)
    {
        return length(v1 - v2);
    }

    template<typename T>
    constexpr T dot(const base_vec2<T>& v1, const base_vec2<T>& v2)
    {
        return v1.x * v2.x + v1.y * v2.y;
    }

    template<std::floating_point T>
    inline base_vec2<T> normalize(const base_vec2<T>& v)
    {
        return v / length(v);
    }

    template<std::floating_point T>
    constexpr base_vec2<T> faceforward(
        const base_vec2<T>& n,
        const base_vec2<T>& i,
        const base_vec2<T>& nref
    )
    {
        if (dot(nref, i) < 0)
            return n;
        return -n;
    }

    template<std::floating_point T>
    constexpr base_vec2<T> reflect(const base_vec2<T>& i, const base_vec2<T>& n)
    {
        return i - 2 * dot(n, i) * n;
    }

    template<std::floating_point T>
    inline base_vec2<T> refract(
        const base_vec2<T>& i,
        const base_vec2<T>& n,
        T eta
    )
    {
        T dp = dot(n, i);
        T k = 1 - eta * eta * (1 - dp * dp);

        if (k < 0)
            return 0;

        return eta * i - (eta * dp + sqrt(k)) * n;
    }

    template<typename T>
    constexpr T min_component(const base_vec2<T>& v)
    {
        return min(v.x, v.y);
    }

    template<typename T>
    constexpr T max_component(const base_vec2<T>& v)
    {
        return max(v.x, v.y);
    }

    template<typename T>
    constexpr i32 min_component_index(const base_vec2<T>& v)
    {
        return (v.x < v.y) ? 0 : 1;
    }

    template<typename T>
    constexpr i32 max_component_index(const base_vec2<T>& v)
    {
        return (v.x > v.y) ? 0 : 1;
    }

    using vec2 = base_vec2<f32>;
    using dvec2 = base_vec2<f64>;
    using ivec2 = base_vec2<i32>;
    using uvec2 = base_vec2<u32>;

}
