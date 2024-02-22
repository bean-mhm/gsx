#pragma once

#include "vec2.h"
#include "vec3.h"
#include "utils.h"
#include "../internal_common/all.h"
#include "../internal_str/all.h"

namespace gsx::math
{

    template<typename T>
    class base_vec4
    {
    public:
        T x = 0, y = 0, z = 0, w = 0;

        constexpr base_vec4() = default;

        constexpr base_vec4(T x, T y, T z, T w)
            : x(x), y(y), z(z), w(w)
        {}

        constexpr base_vec4(T s)
            : x(s), y(s), z(s), w(s)
        {}

        constexpr base_vec4(base_vec2<T> xy, T z, T w)
            : x(xy.x), y(xy.y), z(z), w(w)
        {}

        constexpr base_vec4(T x, base_vec2<T> yz, T w)
            : x(x), y(yz.x), z(yz.y), w(w)
        {}

        constexpr base_vec4(T x, T y, base_vec2<T> zw)
            : x(x), y(y), z(zw.x), w(zw.y)
        {}

        constexpr base_vec4(base_vec2<T> xy, base_vec2<T> zw)
            : x(xy.x), y(xy.y), z(zw.x), w(zw.y)
        {}

        constexpr base_vec4(base_vec3<T> xyz, T w)
            : x(xyz.x), y(xyz.y), z(xyz.z), w(w)
        {}

        constexpr base_vec4(T x, base_vec3<T> yzw)
            : x(x), y(yzw.x), z(yzw.y), w(yzw.z)
        {}

        template<typename U>
        explicit constexpr base_vec4(const base_vec4<U>& v)
            : x((T)v.x), y((T)v.y), z((T)v.z), w((T)v.w)
        {}

        explicit constexpr base_vec4(const base_mat<T, 1, 4>& m)
            : x(m(0)), y(m(1)), z(m(2)), w(m(3))
        {}

        explicit constexpr base_vec4(const base_mat<T, 4, 1>& m)
            : x(m(0)), y(m(1)), z(m(2)), w(m(3))
        {}

        explicit constexpr operator base_mat<T, 1, 4>() const
        {
            return base_mat<T, 1, 4>({ x, y, z, w });
        }

        explicit constexpr operator base_mat<T, 4, 1>() const
        {
            return base_mat<T, 4, 1>({ x, y, z, w });
        }

        std::string to_string() const
        {
            return std::format(
                "[{}, {}, {}, {}]",
                str::from_number(x),
                str::from_number(y),
                str::from_number(z),
                str::from_number(w)
            );
        }

        friend std::ostream& operator<<(std::ostream& os, const base_vec4<T>& v)
        {
            os << v.to_string();
            return os;
        }

        constexpr base_vec4<T> operator+(const base_vec4<T>& v) const
        {
            return base_vec4<T>(x + v.x, y + v.y, z + v.z, w + v.w);
        }

        constexpr base_vec4<T>& operator+=(const base_vec4<T>& v)
        {
            x += v.x;
            y += v.y;
            z += v.z;
            w += v.w;
            return *this;
        }

        constexpr base_vec4<T> operator-(const base_vec4<T>& v) const
        {
            return base_vec4<T>(x - v.x, y - v.y, z - v.z, w - v.w);
        }

        constexpr base_vec4<T>& operator-=(const base_vec4<T>& v)
        {
            x -= v.x;
            y -= v.y;
            z -= v.z;
            w -= v.w;
            return *this;
        }

        constexpr base_vec4<T> operator*(T s) const
        {
            return base_vec4<T>(s * x, s * y, s * z, s * w);
        }

        constexpr base_vec4<T>& operator*=(T s)
        {
            x *= s;
            y *= s;
            z *= s;
            w *= s;
            return *this;
        }

        constexpr base_vec4<T> operator*(base_vec4<T> v) const
        {
            return base_vec4<T>(x * v.x, y * v.y, z * v.z, w * v.w);
        }

        constexpr base_vec4<T>& operator*=(base_vec4<T> v)
        {
            x *= v.x;
            y *= v.y;
            z *= v.z;
            w *= v.w;
            return *this;
        }

        constexpr base_vec4<T> operator/(T s) const
        {
            if constexpr (std::is_floating_point_v<T>)
            {
                T inv = (T)1 / s;
                return base_vec4<T>(x * inv, y * inv, z * inv, w * inv);
            }
            else
            {
                return base_vec4<T>(x / s, y / s, z / s, w / s);
            }
        }

        constexpr base_vec4<T>& operator/=(T s)
        {
            if constexpr (std::is_floating_point_v<T>)
            {
                T inv = (T)1 / s;
                x *= inv;
                y *= inv;
                z *= inv;
                w *= inv;
            }
            else
            {
                x /= s;
                y /= s;
                z /= s;
                w /= s;
            }
            return *this;
        }

        constexpr base_vec4<T> operator/(base_vec4<T> v) const
        {
            return base_vec4<T>(x / v.x, y / v.y, z / v.z, w / v.w);
        }

        constexpr base_vec4<T>& operator/=(base_vec4<T> v)
        {
            x /= v.x;
            y /= v.y;
            z /= v.z;
            w /= v.w;
            return *this;
        }

        template<typename = std::enable_if_t<std::is_integral_v<T>>>
        constexpr base_vec4<T> operator%(base_vec4<T> v) const
        {
            return base_vec4<T>(x % v.x, y % v.y, z % v.z, w % v.w);
        }

        template<typename = std::enable_if_t<std::is_integral_v<T>>>
        constexpr base_vec4<T>& operator%=(base_vec4<T> v)
        {
            x %= v.x;
            y %= v.y;
            z %= v.z;
            w %= v.w;
            return *this;
        }

        constexpr bool operator==(const base_vec4<T>& v) const
        {
            return x == v.x && y == v.y && z == v.z && w == v.w;
        }

        constexpr bool operator!=(const base_vec4<T>& v) const
        {
            return x != v.x || y != v.y || z != v.z || w != v.w;
        }

        constexpr base_vec4<T> operator-() const
        {
            return base_vec4<T>(-x, -y, -z, -w);
        }

        constexpr T operator[](i32 i) const
        {
            if (i == 0) return x;
            if (i == 1) return y;
            if (i == 2) return z;
            return w;
        }

        constexpr T& operator[](i32 i)
        {
            if (i == 0) return x;
            if (i == 1) return y;
            if (i == 2) return z;
            return w;
        }

        constexpr base_vec2<T> permute(i32 x, i32 y) const
        {
            return base_vec2<T>((*this)[x], (*this)[y]);
        }

        constexpr base_vec3<T> permute(i32 x, i32 y, i32 z) const
        {
            return base_vec3<T>((*this)[x], (*this)[y], (*this)[z]);
        }

        constexpr base_vec4<T> permute(i32 x, i32 y, i32 z, i32 w) const
        {
            return base_vec4<T>((*this)[x], (*this)[y], (*this)[z], (*this)[w]);
        }

        constexpr i32 n_components() const
        {
            return 4;
        }

    };

    template<typename T>
    constexpr base_vec4<T> operator+(T s, const base_vec4<T>& v)
    {
        return v + s;
    }

    template<typename T>
    constexpr base_vec4<T> operator-(T s, const base_vec4<T>& v)
    {
        return (-v) + s;
    }

    template<typename T>
    constexpr base_vec4<T> operator*(T s, const base_vec4<T>& v)
    {
        return v * s;
    }

    template<typename T>
    constexpr base_vec4<T> operator/(T s, const base_vec4<T>& v)
    {
        return base_vec4<T>(s / v.x, s / v.y, s / v.z, s / v.w);
    }

    template<std::floating_point T>
    constexpr base_vec4<T> radians(const base_vec4<T>& degrees)
    {
        return degrees * deg2rad;
    }

    template<std::floating_point T>
    constexpr base_vec4<T> degrees(const base_vec4<T>& radians)
    {
        return radians * rad2deg;
    }

    template<std::floating_point T>
    inline base_vec4<T> sin(const base_vec4<T>& v)
    {
        return base_vec4<T>(sin(v.x), sin(v.y), sin(v.z), sin(v.w));
    }

    template<std::floating_point T>
    inline base_vec4<T> cos(const base_vec4<T>& v)
    {
        return base_vec4<T>(cos(v.x), cos(v.y), cos(v.z), cos(v.w));
    }

    template<std::floating_point T>
    inline base_vec4<T> tan(const base_vec4<T>& v)
    {
        return base_vec4<T>(tan(v.x), tan(v.y), tan(v.z), tan(v.w));
    }

    template<std::floating_point T>
    inline base_vec4<T> asin(const base_vec4<T>& v)
    {
        return base_vec4<T>(asin(v.x), asin(v.y), asin(v.z), asin(v.w));
    }

    template<std::floating_point T>
    inline base_vec4<T> acos(const base_vec4<T>& v)
    {
        return base_vec4<T>(acos(v.x), acos(v.y), acos(v.z), acos(v.w));
    }

    template<std::floating_point T>
    inline base_vec4<T> atan(const base_vec4<T>& v)
    {
        return base_vec4<T>(atan(v.x), atan(v.y), atan(v.z), atan(v.w));
    }

    template<std::floating_point T>
    inline base_vec4<T> sinh(const base_vec4<T>& v)
    {
        return base_vec4<T>(sinh(v.x), sinh(v.y), sinh(v.z), sinh(v.w));
    }

    template<std::floating_point T>
    inline base_vec4<T> cosh(const base_vec4<T>& v)
    {
        return base_vec4<T>(cosh(v.x), cosh(v.y), cosh(v.z), cosh(v.w));
    }

    template<std::floating_point T>
    inline base_vec4<T> tanh(const base_vec4<T>& v)
    {
        return base_vec4<T>(tanh(v.x), tanh(v.y), tanh(v.z), tanh(v.w));
    }

    template<std::floating_point T>
    inline base_vec4<T> asinh(const base_vec4<T>& v)
    {
        return base_vec4<T>(asinh(v.x), asinh(v.y), asinh(v.z), asinh(v.w));
    }

    template<std::floating_point T>
    inline base_vec4<T> acosh(const base_vec4<T>& v)
    {
        return base_vec4<T>(acosh(v.x), acosh(v.y), acosh(v.z), acosh(v.w));
    }

    template<std::floating_point T>
    inline base_vec4<T> atanh(const base_vec4<T>& v)
    {
        return base_vec4<T>(atanh(v.x), atanh(v.y), atanh(v.z), atanh(v.w));
    }

    template<std::floating_point T>
    inline base_vec4<T> pow(const base_vec4<T>& v1, const base_vec4<T>& v2)
    {
        return base_vec4<T>(
            pow(v1.x, v2.x), pow(v1.y, v2.y), pow(v1.z, v2.z), pow(v1.w, v2.w)
        );
    }

    template<std::floating_point T>
    inline base_vec4<T> pow(const base_vec4<T>& v1, T v2)
    {
        return base_vec4<T>(
            pow(v1.x, v2), pow(v1.y, v2), pow(v1.z, v2), pow(v1.w, v2)
        );
    }

    template<std::floating_point T>
    inline base_vec4<T> exp(const base_vec4<T>& v)
    {
        return base_vec4<T>(exp(v.x), exp(v.y), exp(v.z), exp(v.w));
    }

    template<std::floating_point T>
    inline base_vec4<T> log(const base_vec4<T>& v)
    {
        return base_vec4<T>(log(v.x), log(v.y), log(v.z), log(v.w));
    }

    template<std::floating_point T>
    inline base_vec4<T> exp2(const base_vec4<T>& v)
    {
        return base_vec4<T>(exp2(v.x), exp2(v.y), exp2(v.z), exp2(v.w));
    }

    template<std::floating_point T>
    inline base_vec4<T> log2(const base_vec4<T>& v)
    {
        return base_vec4<T>(log2(v.x), log2(v.y), log2(v.z), log2(v.w));
    }

    template<std::floating_point T>
    inline base_vec4<T> sqrt(const base_vec4<T>& v)
    {
        return base_vec4<T>(sqrt(v.x), sqrt(v.y), sqrt(v.z), sqrt(v.w));
    }

    template<std::floating_point T>
    inline base_vec4<T> inversesqrt(const base_vec4<T>& v)
    {
        return base_vec4<T>(
            inversesqrt(v.x),
            inversesqrt(v.y),
            inversesqrt(v.z),
            inversesqrt(v.w)
        );
    }

    template<typename T>
    inline base_vec4<T> abs(const base_vec4<T>& v)
    {
        return base_vec4<T>(abs(v.x), abs(v.y), abs(v.z), abs(v.w));
    }

    template<typename T>
    constexpr base_vec4<T> sign(const base_vec4<T>& v)
    {
        return base_vec4<T>(sign(v.x), sign(v.y), sign(v.z), sign(v.w));
    }

    template<std::floating_point T>
    inline base_vec4<T> floor(const base_vec4<T>& v)
    {
        return base_vec4<T>(floor(v.x), floor(v.y), floor(v.z), floor(v.w));
    }

    template<std::floating_point T>
    inline base_vec4<T> ceil(const base_vec4<T>& v)
    {
        return base_vec4<T>(ceil(v.x), ceil(v.y), ceil(v.z), ceil(v.w));
    }

    template<std::floating_point T>
    inline base_vec4<T> trunc(const base_vec4<T>& v)
    {
        return base_vec4<T>(trunc(v.x), trunc(v.y), trunc(v.z), trunc(v.w));
    }

    template<std::floating_point T>
    inline base_vec4<T> fract(const base_vec4<T>& v)
    {
        return base_vec4<T>(fract(v.x), fract(v.y), fract(v.z), fract(v.w));
    }

    template<std::floating_point T>
    inline base_vec4<T> mod(const base_vec4<T>& v1, const base_vec4<T>& v2)
    {
        return base_vec4<T>(
            mod(v1.x, v2.x), mod(v1.y, v2.y), mod(v1.z, v2.z), mod(v1.w, v2.w)
        );
    }

    template<std::floating_point T>
    inline base_vec4<T> mod(const base_vec4<T>& v1, T v2)
    {
        return base_vec4<T>(
            mod(v1.x, v2), mod(v1.y, v2), mod(v1.z, v2), mod(v1.w, v2)
        );
    }

    template<std::floating_point T>
    inline base_vec4<T> modf(const base_vec4<T>& v, base_vec4<T>& i)
    {
        return base_vec4<T>(
            modf(v.x, i.x), modf(v.y, i.y), modf(v.z, i.z), modf(v.w, i.w)
        );
    }

    template<std::floating_point T>
    inline base_vec4<T> wrap(const base_vec4<T>& v, T start, T end)
    {
        return start + mod(v - start, end - start);
    }

    template<typename T>
    constexpr base_vec4<T> min(const base_vec4<T>& v1, const base_vec4<T>& v2)
    {
        return base_vec4<T>(
            min(v1.x, v2.x),
            min(v1.y, v2.y),
            min(v1.z, v2.z),
            min(v1.w, v2.w)
        );
    }

    template<typename T>
    constexpr base_vec4<T> max(const base_vec4<T>& v1, const base_vec4<T>& v2)
    {
        return base_vec4<T>(
            max(v1.x, v2.x),
            max(v1.y, v2.y),
            max(v1.z, v2.z),
            max(v1.w, v2.w)
        );
    }

    template<typename T>
    constexpr base_vec4<T> clamp(const base_vec4<T>& v, T min, T max)
    {
        return base_vec4<T>(
            clamp(v.x, min, max),
            clamp(v.y, min, max),
            clamp(v.z, min, max),
            clamp(v.w, min, max)
        );
    }

    template<typename T>
    constexpr base_vec4<T> clamp(
        const base_vec4<T>& v,
        const base_vec4<T>& min,
        const base_vec4<T>& max
    )
    {
        return base_vec4<T>(
            clamp(v.x, min.x, max.x),
            clamp(v.y, min.y, max.y),
            clamp(v.z, min.z, max.z),
            clamp(v.w, min.w, max.w)
        );
    }

    template<std::floating_point T>
    constexpr base_vec4<T> clamp01(const base_vec4<T>& v)
    {
        return base_vec4<T>(
            clamp01(v.x), clamp01(v.y), clamp01(v.z), clamp01(v.w)
        );
    }

    template<std::floating_point T>
    constexpr base_vec4<T> mix(
        const base_vec4<T>& v1,
        const base_vec4<T>& v2,
        T a
    )
    {
        return v1 + a * (v2 - v1);
    }

    template<std::floating_point T>
    constexpr base_vec4<T> remap(
        const base_vec4<T>& v,
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
    constexpr base_vec4<T> remap_clamp(
        const base_vec4<T>& v,
        T a_start,
        T a_end,
        T b_start,
        T b_end
    )
    {
        base_vec4<T> t = clamp01((v - a_start) / (a_end - a_start));
        return b_start + t * (b_end - b_start);
    }

    template<std::floating_point T>
    constexpr base_vec4<T> remap01(
        const base_vec4<T>& v,
        T a_start,
        T a_end
    )
    {
        return clamp01((v - a_start) / (a_end - a_start));
    }

    template<typename T>
    constexpr base_vec4<T> step(T edge, const base_vec4<T>& v)
    {
        return base_vec4<T>(
            step(edge, v.x), step(edge, v.y), step(edge, v.z), step(edge, v.w)
        );
    }

    template<std::floating_point T>
    constexpr base_vec4<T> smoothstep(
        T edge0,
        T edge1,
        const base_vec4<T>& v
    )
    {
        return base_vec4<T>(
            smoothstep(edge0, edge1, v.x),
            smoothstep(edge0, edge1, v.y),
            smoothstep(edge0, edge1, v.z),
            smoothstep(edge0, edge1, v.w)
        );
    }

    template<typename T>
    constexpr T length_squared(const base_vec4<T>& v)
    {
        return v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w;
    }

    template<std::floating_point T>
    inline T length(const base_vec4<T>& v)
    {
        return sqrt(length_squared(v));
    }

    template<typename T>
    constexpr T distance_squared(
        const base_vec4<T>& v1,
        const base_vec4<T>& v2
    )
    {
        return length_squared(v1 - v2);
    }

    template<std::floating_point T>
    inline T distance(const base_vec4<T>& v1, const base_vec4<T>& v2)
    {
        return length(v1 - v2);
    }

    template<typename T>
    constexpr T dot(const base_vec4<T>& v1, const base_vec4<T>& v2)
    {
        return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w;
    }

    template<std::floating_point T>
    inline base_vec4<T> normalize(const base_vec4<T>& v)
    {
        return v / length(v);
    }

    template<std::floating_point T>
    constexpr base_vec4<T> faceforward(
        const base_vec4<T>& n,
        const base_vec4<T>& i,
        const base_vec4<T>& nref
    )
    {
        if (dot(nref, i) < 0)
            return n;
        return -n;
    }

    template<std::floating_point T>
    constexpr base_vec4<T> reflect(const base_vec4<T>& i, const base_vec4<T>& n)
    {
        return i - 2 * dot(n, i) * n;
    }

    template<std::floating_point T>
    inline base_vec4<T> refract(
        const base_vec4<T>& i,
        const base_vec4<T>& n,
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
    constexpr T min_component(const base_vec4<T>& v)
    {
        return min(v.x, min(v.y, min(v.z, v.w)));
    }

    template<typename T>
    constexpr T max_component(const base_vec4<T>& v)
    {
        return max(v.x, max(v.y, max(v.z, v.w)));
    }

    template<typename T>
    constexpr i32 min_component_index(const base_vec4<T>& v)
    {
        return (v.x < v.y)
            ? ((v.x < v.z)
                ? (v.x < v.w ? 0 : 3)
                : (v.z < v.w ? 2 : 3))
            : ((v.y < v.z)
                ? (v.y < v.w ? 1 : 3)
                : (v.z < v.w ? 2 : 3));
    }

    template<typename T>
    constexpr i32 max_component_index(const base_vec4<T>& v)
    {
        return (v.x > v.y)
            ? ((v.x > v.z)
                ? (v.x > v.w ? 0 : 3)
                : (v.z > v.w ? 2 : 3))
            : ((v.y > v.z)
                ? (v.y > v.w ? 1 : 3)
                : (v.z > v.w ? 2 : 3));
    }

    using vec4 = base_vec4<f32>;
    using dvec4 = base_vec4<f64>;
    using ivec4 = base_vec4<i32>;
    using uvec4 = base_vec4<u32>;

}
