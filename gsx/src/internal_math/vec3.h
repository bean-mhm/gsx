#pragma once

#include "vec2.h"
#include "utils.h"
#include "../internal_common/all.h"
#include "../internal_str/all.h"

namespace gsx::math
{

    template<typename T>
    class base_vec3
    {
    public:
        T x = 0, y = 0, z = 0;

        constexpr base_vec3() = default;

        constexpr base_vec3(T x, T y, T z)
            : x(x), y(y), z(z)
        {}

        constexpr base_vec3(T s)
            : x(s), y(s), z(s)
        {}

        constexpr base_vec3(base_vec2<T> xy, T z)
            : x(xy.x), y(xy.y), z(z)
        {}

        constexpr base_vec3(T x, base_vec2<T> yz)
            : x(x), y(yz.x), z(yz.y)
        {}

        template<typename U>
        explicit constexpr base_vec3(const base_vec3<U>& v)
            : x((T)v.x), y((T)v.y), z((T)v.z)
        {}

        explicit constexpr base_vec3(const base_mat<T, 1, 3>& m)
            : x(m(0)), y(m(1)), z(m(2))
        {}

        explicit constexpr base_vec3(const base_mat<T, 3, 1>& m)
            : x(m(0)), y(m(1)), z(m(2))
        {}

        explicit constexpr operator base_mat<T, 1, 3>() const
        {
            return base_mat<T, 1, 3>({ x, y, z });
        }

        explicit constexpr operator base_mat<T, 3, 1>() const
        {
            return base_mat<T, 3, 1>({ x, y, z });
        }

        std::string to_string() const
        {
            return std::format(
                "[{}, {}, {}]",
                str::from_number(x),
                str::from_number(y),
                str::from_number(z)
            );
        }

        friend std::ostream& operator<<(std::ostream& os, const base_vec3<T>& v)
        {
            os << v.to_string();
            return os;
        }

        constexpr base_vec3<T> operator+(const base_vec3<T>& v) const
        {
            return base_vec3<T>(x + v.x, y + v.y, z + v.z);
        }

        constexpr base_vec3<T>& operator+=(const base_vec3<T>& v)
        {
            x += v.x;
            y += v.y;
            z += v.z;
            return *this;
        }

        constexpr base_vec3<T> operator-(const base_vec3<T>& v) const
        {
            return base_vec3<T>(x - v.x, y - v.y, z - v.z);
        }

        constexpr base_vec3<T>& operator-=(const base_vec3<T>& v)
        {
            x -= v.x;
            y -= v.y;
            z -= v.z;
            return *this;
        }

        constexpr base_vec3<T> operator*(T s) const
        {
            return base_vec3<T>(s * x, s * y, s * z);
        }

        constexpr base_vec3<T>& operator*=(T s)
        {
            x *= s;
            y *= s;
            z *= s;
            return *this;
        }

        constexpr base_vec3<T> operator*(base_vec3<T> v) const
        {
            return base_vec3<T>(x * v.x, y * v.y, z * v.z);
        }

        constexpr base_vec3<T>& operator*=(base_vec3<T> v)
        {
            x *= v.x;
            y *= v.y;
            z *= v.z;
            return *this;
        }

        constexpr base_vec3<T> operator/(T s) const
        {
            if constexpr (std::is_floating_point_v<T>)
            {
                T inv = (T)1 / s;
                return base_vec3<T>(x * inv, y * inv, z * inv);
            }
            else
            {
                return base_vec3<T>(x / s, y / s, z / s);
            }
        }

        constexpr base_vec3<T>& operator/=(T s)
        {
            if constexpr (std::is_floating_point_v<T>)
            {
                T inv = (T)1 / s;
                x *= inv;
                y *= inv;
                z *= inv;
            }
            else
            {
                x /= s;
                y /= s;
                z /= s;
            }
            return *this;
        }

        constexpr base_vec3<T> operator/(base_vec3<T> v) const
        {
            return base_vec3<T>(x / v.x, y / v.y, z / v.z);
        }

        constexpr base_vec3<T>& operator/=(base_vec3<T> v)
        {
            x /= v.x;
            y /= v.y;
            z /= v.z;
            return *this;
        }

        template<typename = std::enable_if_t<std::is_integral_v<T>>>
        constexpr base_vec3<T> operator%(base_vec3<T> v) const
        {
            return base_vec3<T>(x % v.x, y % v.y, z % v.z);
        }

        template<typename = std::enable_if_t<std::is_integral_v<T>>>
        constexpr base_vec3<T>& operator%=(base_vec3<T> v)
        {
            x %= v.x;
            y %= v.y;
            z %= v.z;
            return *this;
        }

        constexpr bool operator==(const base_vec3<T>& v) const
        {
            return x == v.x && y == v.y && z == v.z;
        }

        constexpr bool operator!=(const base_vec3<T>& v) const
        {
            return x != v.x || y != v.y || z != v.z;
        }

        constexpr base_vec3<T> operator-() const
        {
            return base_vec3<T>(-x, -y, -z);
        }

        constexpr T operator[](i32 i) const
        {
            if (i == 0) return x;
            if (i == 1) return y;
            return z;
        }

        constexpr T& operator[](i32 i)
        {
            if (i == 0) return x;
            if (i == 1) return y;
            return z;
        }

        constexpr base_vec2<T> permute(i32 x, i32 y) const
        {
            return base_vec2<T>((*this)[x], (*this)[y]);
        }

        constexpr base_vec3<T> permute(i32 x, i32 y, i32 z) const
        {
            return base_vec3<T>((*this)[x], (*this)[y], (*this)[z]);
        }

        constexpr i32 n_components() const
        {
            return 3;
        }

    };

    template<typename T>
    constexpr base_vec3<T> operator+(T s, const base_vec3<T>& v)
    {
        return v + s;
    }

    template<typename T>
    constexpr base_vec3<T> operator-(T s, const base_vec3<T>& v)
    {
        return (-v) + s;
    }

    template<typename T>
    constexpr base_vec3<T> operator*(T s, const base_vec3<T>& v)
    {
        return v * s;
    }

    template<typename T>
    constexpr base_vec3<T> operator/(T s, const base_vec3<T>& v)
    {
        return base_vec3<T>(s / v.x, s / v.y, s / v.z);
    }

    template<std::floating_point T>
    constexpr base_vec3<T> radians(const base_vec3<T>& degrees)
    {
        return degrees * deg2rad;
    }

    template<std::floating_point T>
    constexpr base_vec3<T> degrees(const base_vec3<T>& radians)
    {
        return radians * rad2deg;
    }

    template<std::floating_point T>
    inline base_vec3<T> sin(const base_vec3<T>& v)
    {
        return base_vec3<T>(sin(v.x), sin(v.y), sin(v.z));
    }

    template<std::floating_point T>
    inline base_vec3<T> cos(const base_vec3<T>& v)
    {
        return base_vec3<T>(cos(v.x), cos(v.y), cos(v.z));
    }

    template<std::floating_point T>
    inline base_vec3<T> tan(const base_vec3<T>& v)
    {
        return base_vec3<T>(tan(v.x), tan(v.y), tan(v.z));
    }

    template<std::floating_point T>
    inline base_vec3<T> asin(const base_vec3<T>& v)
    {
        return base_vec3<T>(asin(v.x), asin(v.y), asin(v.z));
    }

    template<std::floating_point T>
    inline base_vec3<T> acos(const base_vec3<T>& v)
    {
        return base_vec3<T>(acos(v.x), acos(v.y), acos(v.z));
    }

    template<std::floating_point T>
    inline base_vec3<T> atan(const base_vec3<T>& v)
    {
        return base_vec3<T>(atan(v.x), atan(v.y), atan(v.z));
    }

    template<std::floating_point T>
    inline base_vec3<T> sinh(const base_vec3<T>& v)
    {
        return base_vec3<T>(sinh(v.x), sinh(v.y), sinh(v.z));
    }

    template<std::floating_point T>
    inline base_vec3<T> cosh(const base_vec3<T>& v)
    {
        return base_vec3<T>(cosh(v.x), cosh(v.y), cosh(v.z));
    }

    template<std::floating_point T>
    inline base_vec3<T> tanh(const base_vec3<T>& v)
    {
        return base_vec3<T>(tanh(v.x), tanh(v.y), tanh(v.z));
    }

    template<std::floating_point T>
    inline base_vec3<T> asinh(const base_vec3<T>& v)
    {
        return base_vec3<T>(asinh(v.x), asinh(v.y), asinh(v.z));
    }

    template<std::floating_point T>
    inline base_vec3<T> acosh(const base_vec3<T>& v)
    {
        return base_vec3<T>(acosh(v.x), acosh(v.y), acosh(v.z));
    }

    template<std::floating_point T>
    inline base_vec3<T> atanh(const base_vec3<T>& v)
    {
        return base_vec3<T>(atanh(v.x), atanh(v.y), atanh(v.z));
    }

    template<std::floating_point T>
    inline base_vec3<T> pow(const base_vec3<T>& v1, const base_vec3<T>& v2)
    {
        return base_vec3<T>(pow(v1.x, v2.x), pow(v1.y, v2.y), pow(v1.z, v2.z));
    }

    template<std::floating_point T>
    inline base_vec3<T> pow(const base_vec3<T>& v1, T v2)
    {
        return base_vec3<T>(pow(v1.x, v2), pow(v1.y, v2), pow(v1.z, v2));
    }

    template<std::floating_point T>
    inline base_vec3<T> exp(const base_vec3<T>& v)
    {
        return base_vec3<T>(exp(v.x), exp(v.y), exp(v.z));
    }

    template<std::floating_point T>
    inline base_vec3<T> log(const base_vec3<T>& v)
    {
        return base_vec3<T>(log(v.x), log(v.y), log(v.z));
    }

    template<std::floating_point T>
    inline base_vec3<T> exp2(const base_vec3<T>& v)
    {
        return base_vec3<T>(exp2(v.x), exp2(v.y), exp2(v.z));
    }

    template<std::floating_point T>
    inline base_vec3<T> log2(const base_vec3<T>& v)
    {
        return base_vec3<T>(log2(v.x), log2(v.y), log2(v.z));
    }

    template<std::floating_point T>
    inline base_vec3<T> sqrt(const base_vec3<T>& v)
    {
        return base_vec3<T>(sqrt(v.x), sqrt(v.y), sqrt(v.z));
    }

    template<std::floating_point T>
    inline base_vec3<T> inversesqrt(const base_vec3<T>& v)
    {
        return base_vec3<T>(
            inversesqrt(v.x), inversesqrt(v.y), inversesqrt(v.z)
        );
    }

    template<typename T>
    inline base_vec3<T> abs(const base_vec3<T>& v)
    {
        return base_vec3<T>(abs(v.x), abs(v.y), abs(v.z));
    }

    template<typename T>
    constexpr base_vec3<T> sign(const base_vec3<T>& v)
    {
        return base_vec3<T>(sign(v.x), sign(v.y), sign(v.z));
    }

    template<std::floating_point T>
    inline base_vec3<T> floor(const base_vec3<T>& v)
    {
        return base_vec3<T>(floor(v.x), floor(v.y), floor(v.z));
    }

    template<std::floating_point T>
    inline base_vec3<T> ceil(const base_vec3<T>& v)
    {
        return base_vec3<T>(ceil(v.x), ceil(v.y), ceil(v.z));
    }

    template<std::floating_point T>
    inline base_vec3<T> trunc(const base_vec3<T>& v)
    {
        return base_vec3<T>(trunc(v.x), trunc(v.y), trunc(v.z));
    }

    template<std::floating_point T>
    inline base_vec3<T> fract(const base_vec3<T>& v)
    {
        return base_vec3<T>(fract(v.x), fract(v.y), fract(v.z));
    }

    template<std::floating_point T>
    inline base_vec3<T> mod(const base_vec3<T>& v1, const base_vec3<T>& v2)
    {
        return base_vec3<T>(mod(v1.x, v2.x), mod(v1.y, v2.y), mod(v1.z, v2.z));
    }

    template<std::floating_point T>
    inline base_vec3<T> mod(const base_vec3<T>& v1, T v2)
    {
        return base_vec3<T>(mod(v1.x, v2), mod(v1.y, v2), mod(v1.z, v2));
    }

    template<std::floating_point T>
    inline base_vec3<T> modf(const base_vec3<T>& v, base_vec3<T>& i)
    {
        return base_vec3<T>(modf(v.x, i.x), modf(v.y, i.y), modf(v.z, i.z));
    }

    template<std::floating_point T>
    inline base_vec3<T> wrap(const base_vec3<T>& v, T start, T end)
    {
        return start + mod(v - start, end - start);
    }

    template<typename T>
    constexpr base_vec3<T> min(const base_vec3<T>& v1, const base_vec3<T>& v2)
    {
        return base_vec3<T>(
            min(v1.x, v2.x),
            min(v1.y, v2.y),
            min(v1.z, v2.z)
        );
    }

    template<typename T>
    constexpr base_vec3<T> max(const base_vec3<T>& v1, const base_vec3<T>& v2)
    {
        return base_vec3<T>(
            max(v1.x, v2.x),
            max(v1.y, v2.y),
            max(v1.z, v2.z)
        );
    }

    template<typename T>
    constexpr base_vec3<T> clamp(const base_vec3<T>& v, T min, T max)
    {
        return base_vec3<T>(
            clamp(v.x, min, max), clamp(v.y, min, max), clamp(v.z, min, max)
        );
    }

    template<typename T>
    constexpr base_vec3<T> clamp(
        const base_vec3<T>& v,
        const base_vec3<T>& min,
        const base_vec3<T>& max
    )
    {
        return base_vec3<T>(
            clamp(v.x, min.x, max.x),
            clamp(v.y, min.y, max.y),
            clamp(v.z, min.z, max.z)
        );
    }

    template<std::floating_point T>
    constexpr base_vec3<T> clamp01(const base_vec3<T>& v)
    {
        return base_vec3<T>(clamp01(v.x), clamp01(v.y), clamp01(v.z));
    }

    template<std::floating_point T>
    constexpr base_vec3<T> mix(
        const base_vec3<T>& v1,
        const base_vec3<T>& v2,
        T a
    )
    {
        return v1 + a * (v2 - v1);
    }

    template<std::floating_point T>
    constexpr base_vec3<T> remap(
        const base_vec3<T>& v,
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
    constexpr base_vec3<T> remap_clamp(
        const base_vec3<T>& v,
        T a_start,
        T a_end,
        T b_start,
        T b_end
    )
    {
        base_vec3<T> t = clamp01((v - a_start) / (a_end - a_start));
        return b_start + t * (b_end - b_start);
    }

    template<std::floating_point T>
    constexpr base_vec3<T> remap01(
        const base_vec3<T>& v,
        T a_start,
        T a_end
    )
    {
        return clamp01((v - a_start) / (a_end - a_start));
    }

    template<typename T>
    constexpr base_vec3<T> step(T edge, const base_vec3<T>& v)
    {
        return base_vec3<T>(step(edge, v.x), step(edge, v.y), step(edge, v.z));
    }

    template<std::floating_point T>
    constexpr base_vec3<T> smoothstep(
        T edge0,
        T edge1,
        const base_vec3<T>& v
    )
    {
        return base_vec3<T>(
            smoothstep(edge0, edge1, v.x),
            smoothstep(edge0, edge1, v.y),
            smoothstep(edge0, edge1, v.z)
        );
    }

    template<typename T>
    constexpr T length_squared(const base_vec3<T>& v)
    {
        return v.x * v.x + v.y * v.y + v.z * v.z;
    }

    template<std::floating_point T>
    inline T length(const base_vec3<T>& v)
    {
        return sqrt(length_squared(v));
    }

    template<typename T>
    constexpr T distance_squared(
        const base_vec3<T>& v1,
        const base_vec3<T>& v2
    )
    {
        return length_squared(v1 - v2);
    }

    template<std::floating_point T>
    inline T distance(const base_vec3<T>& v1, const base_vec3<T>& v2)
    {
        return length(v1 - v2);
    }

    template<typename T>
    constexpr T dot(const base_vec3<T>& v1, const base_vec3<T>& v2)
    {
        return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
    }

    template<typename T>
    constexpr base_vec3<T> cross(const base_vec3<T>& v1, const base_vec3<T>& v2)
    {
        return base_vec3<T>(
            (v1.y * v2.z) - (v1.z * v2.y),
            (v1.z * v2.x) - (v1.x * v2.z),
            (v1.x * v2.y) - (v1.y * v2.x)
        );
    }

    template<std::floating_point T>
    inline base_vec3<T> normalize(const base_vec3<T>& v)
    {
        return v / length(v);
    }

    template<std::floating_point T>
    constexpr base_vec3<T> faceforward(
        const base_vec3<T>& n,
        const base_vec3<T>& i,
        const base_vec3<T>& nref
    )
    {
        if (dot(nref, i) < 0)
            return n;
        return -n;
    }

    template<std::floating_point T>
    constexpr base_vec3<T> reflect(const base_vec3<T>& i, const base_vec3<T>& n)
    {
        return i - 2 * dot(n, i) * n;
    }

    template<std::floating_point T>
    inline base_vec3<T> refract(
        const base_vec3<T>& i,
        const base_vec3<T>& n,
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
    constexpr T min_component(const base_vec3<T>& v)
    {
        return min(v.x, min(v.y, v.z));
    }

    template<typename T>
    constexpr T max_component(const base_vec3<T>& v)
    {
        return max(v.x, max(v.y, v.z));
    }

    template<typename T>
    constexpr i32 min_component_index(const base_vec3<T>& v)
    {
        return (v.x < v.y)
            ? ((v.x < v.z) ? 0 : 2)
            : ((v.y < v.z) ? 1 : 2);
    }

    template<typename T>
    constexpr i32 max_component_index(const base_vec3<T>& v)
    {
        return (v.x > v.y)
            ? ((v.x > v.z) ? 0 : 2)
            : ((v.y > v.z) ? 1 : 2);
    }

    template<std::floating_point T>
    inline void coordinate_system(
        const base_vec3<T>& v1,
        base_vec3<T>& v2,
        base_vec3<T>& v3
    )
    {
        if (abs(v1.x) > abs(v1.y))
        {
            v2 = base_vec3<T>(-v1.z, 0, v1.x) / sqrt(v1.x * v1.x + v1.z * v1.z);
        }
        else
        {
            v2 = base_vec3<T>(0, v1.z, -v1.y) / sqrt(v1.y * v1.y + v1.z * v1.z);
        }
        v3 = cross(v1, v2);
    }

    using vec3 = base_vec3<f32>;
    using dvec3 = base_vec3<f64>;
    using ivec3 = base_vec3<i32>;
    using uvec3 = base_vec3<u32>;

}
