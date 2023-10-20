#pragma once

// Internal
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

        // Constructors

        constexpr base_vec2() = default;

        constexpr base_vec2(T x, T y)
            : x(x), y(y)
        {}

        constexpr base_vec2(T s)
            : x(s), y(s)
        {}

        // Explicit type conversion
        template<typename U>
        explicit constexpr base_vec2(const base_vec2<U>& v)
            : x((T)v.x), y((T)v.y)
        {}

        // Construct from matrix

        explicit constexpr base_vec2(const mat1x2& m)
            : x((T)m(0)), y((T)m(1))
        {}

        explicit constexpr base_vec2(const mat2x1& m)
            : x((T)m(0)), y((T)m(1))
        {}

        // Cast to matrix

        explicit constexpr operator mat1x2() const
        {
            return mat1x2({ (f32)x, (f32)y });
        }

        explicit constexpr operator mat2x1() const
        {
            return mat2x1({ (f32)x, (f32)y });
        }

        // String
        std::string to_string() const
        {
            return std::format(
                "[{}, {}]",
                str::from_number(x),
                str::from_number(y)
            );
        }

        // Print
        friend std::ostream& operator<<(std::ostream& os, const base_vec2<T>& v)
        {
            os << v.to_string();
            return os;
        }

        // this + vector (may be implicitly converted from a scalar)
        constexpr base_vec2<T> operator+(const base_vec2<T>& v) const
        {
            return base_vec2<T>(x + v.x, y + v.y);
        }

        // this += vector (may be implicitly converted from a scalar)
        constexpr base_vec2<T>& operator+=(const base_vec2<T>& v)
        {
            x += v.x;
            y += v.y;
            return *this;
        }

        // this - vector (may be implicitly converted from a scalar)
        constexpr base_vec2<T> operator-(const base_vec2<T>& v) const
        {
            return base_vec2<T>(x - v.x, y - v.y);
        }

        // this -= vector (may be implicitly converted from a scalar)
        constexpr base_vec2<T>& operator-=(const base_vec2<T>& v)
        {
            x -= v.x;
            y -= v.y;
            return *this;
        }

        // this * scalar
        constexpr base_vec2<T> operator*(T s) const
        {
            return base_vec2<T>(s * x, s * y);
        }

        // this *= scalar
        constexpr base_vec2<T>& operator*=(T s)
        {
            x *= s;
            y *= s;
            return *this;
        }

        // this * vector (component-wise multiplication)
        constexpr base_vec2<T> operator*(base_vec2<T> v) const
        {
            return base_vec2<T>(x * v.x, y * v.y);
        }

        // this *= vector (component-wise multiplication)
        constexpr base_vec2<T>& operator*=(base_vec2<T> v)
        {
            x *= v.x;
            y *= v.y;
            return *this;
        }

        // this / scalar
        constexpr base_vec2<T> operator/(T s) const
        {
            f32 inv = 1.f / (f32)s;
            return base_vec2<T>(x * inv, y * inv);
        }

        // this /= scalar
        constexpr base_vec2<T>& operator/=(T s)
        {
            f32 inv = 1.f / (f32)s;
            x *= inv;
            y *= inv;
            return *this;
        }

        // this / vector (component-wise division)
        constexpr base_vec2<T> operator/(base_vec2<T> v) const
        {
            return base_vec2<T>(x / v.x, y / v.y);
        }

        // this /= vector (component-wise division)
        constexpr base_vec2<T>& operator/=(base_vec2<T> v)
        {
            x /= v.x;
            y /= v.y;
            return *this;
        }

        // this == vector
        constexpr bool operator==(const base_vec2<T>& v) const
        {
            return x == v.x && y == v.y;
        }

        // this != vector
        constexpr bool operator!=(const base_vec2<T>& v) const
        {
            return x != v.x || y != v.y;
        }

        // (-1) * this
        constexpr base_vec2<T> operator-() const
        {
            return base_vec2<T>(-x, -y);
        }

        // Access by index (copy)
        constexpr T operator[](i32 i) const
        {
            if (i == 0) return x;
            return y;
        }

        // Access by index (reference)
        constexpr T& operator[](i32 i)
        {
            if (i == 0) return x;
            return y;
        }

        // Permute by indices
        constexpr base_vec2<T> permute(i32 x, i32 y) const
        {
            return base_vec2<T>((*this)[x], (*this)[y]);
        }

        // Swapped components
        constexpr base_vec2<T> yx() const
        {
            return base_vec2<T>(y, x);
        }

        // Number of components
        constexpr i32 n_components() const
        {
            return 2;
        }

    };

    // Scalar + vector
    template<typename T>
    constexpr base_vec2<T> operator+(T s, const base_vec2<T>& v)
    {
        return v + s;
    }

    // Scalar - vector
    template<typename T>
    constexpr base_vec2<T> operator-(T s, const base_vec2<T>& v)
    {
        return (-v) + s;
    }

    // Scalar * vector
    template<typename T>
    constexpr base_vec2<T> operator*(T s, const base_vec2<T>& v)
    {
        return v * s;
    }

    // Scalar / vector
    template<typename T>
    constexpr base_vec2<T> operator/(T s, const base_vec2<T>& v)
    {
        return base_vec2<T>(s / v.x, s / v.y);
    }

    template<typename T>
    constexpr base_vec2<T> radians(const base_vec2<T>& degrees)
    {
        return degrees * deg2rad;
    }

    template<typename T>
    constexpr base_vec2<T> degrees(const base_vec2<T>& radians)
    {
        return radians * rad2deg;
    }

    template<typename T>
    inline base_vec2<T> sin(const base_vec2<T>& v)
    {
        return base_vec2<T>(sin(v.x), sin(v.y));
    }

    template<typename T>
    inline base_vec2<T> cos(const base_vec2<T>& v)
    {
        return base_vec2<T>(cos(v.x), cos(v.y));
    }

    template<typename T>
    inline base_vec2<T> tan(const base_vec2<T>& v)
    {
        return base_vec2<T>(tan(v.x), tan(v.y));
    }

    template<typename T>
    inline base_vec2<T> asin(const base_vec2<T>& v)
    {
        return base_vec2<T>(asin(v.x), asin(v.y));
    }

    template<typename T>
    inline base_vec2<T> acos(const base_vec2<T>& v)
    {
        return base_vec2<T>(acos(v.x), acos(v.y));
    }

    template<typename T>
    inline base_vec2<T> atan(const base_vec2<T>& v)
    {
        return base_vec2<T>(atan(v.x), atan(v.y));
    }

    template<typename T>
    inline base_vec2<T> sinh(const base_vec2<T>& v)
    {
        return base_vec2<T>(sinh(v.x), sinh(v.y));
    }

    template<typename T>
    inline base_vec2<T> cosh(const base_vec2<T>& v)
    {
        return base_vec2<T>(cosh(v.x), cosh(v.y));
    }

    template<typename T>
    inline base_vec2<T> tanh(const base_vec2<T>& v)
    {
        return base_vec2<T>(tanh(v.x), tanh(v.y));
    }

    template<typename T>
    inline base_vec2<T> asinh(const base_vec2<T>& v)
    {
        return base_vec2<T>(asinh(v.x), asinh(v.y));
    }

    template<typename T>
    inline base_vec2<T> acosh(const base_vec2<T>& v)
    {
        return base_vec2<T>(acosh(v.x), acosh(v.y));
    }

    template<typename T>
    inline base_vec2<T> atanh(const base_vec2<T>& v)
    {
        return base_vec2<T>(atanh(v.x), atanh(v.y));
    }

    template<typename T>
    inline base_vec2<T> pow(const base_vec2<T>& v1, const base_vec2<T>& v2)
    {
        return base_vec2<T>(pow(v1.x, v2.x), pow(v1.y, v2.y));
    }

    template<typename T>
    inline base_vec2<T> pow(const base_vec2<T>& v1, T v2)
    {
        return base_vec2<T>(pow(v1.x, v2), pow(v1.y, v2));
    }

    template<typename T>
    inline base_vec2<T> exp(const base_vec2<T>& v)
    {
        return base_vec2<T>(exp(v.x), exp(v.y));
    }

    template<typename T>
    inline base_vec2<T> log(const base_vec2<T>& v)
    {
        return base_vec2<T>(log(v.x), log(v.y));
    }

    template<typename T>
    inline base_vec2<T> exp2(const base_vec2<T>& v)
    {
        return base_vec2<T>(exp2(v.x), exp2(v.y));
    }

    template<typename T>
    inline base_vec2<T> log2(const base_vec2<T>& v)
    {
        return base_vec2<T>(log2(v.x), log2(v.y));
    }

    template<typename T>
    inline base_vec2<T> sqrt(const base_vec2<T>& v)
    {
        return base_vec2<T>(sqrt(v.x), sqrt(v.y));
    }

    template<typename T>
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

    template<typename T>
    inline base_vec2<T> floor(const base_vec2<T>& v)
    {
        return base_vec2<T>(floor(v.x), floor(v.y));
    }

    template<typename T>
    inline base_vec2<T> ceil(const base_vec2<T>& v)
    {
        return base_vec2<T>(ceil(v.x), ceil(v.y));
    }

    template<typename T>
    inline base_vec2<T> trunc(const base_vec2<T>& v)
    {
        return base_vec2<T>(trunc(v.x), trunc(v.y));
    }

    template<typename T>
    inline base_vec2<T> fract(const base_vec2<T>& v)
    {
        return base_vec2<T>(fract(v.x), fract(v.y));
    }

    template<typename T>
    inline base_vec2<T> mod(const base_vec2<T>& v1, const base_vec2<T>& v2)
    {
        return base_vec2<T>(mod(v1.x, v2.x), mod(v1.y, v2.y));
    }

    template<typename T>
    inline base_vec2<T> mod(const base_vec2<T>& v1, T v2)
    {
        return base_vec2<T>(mod(v1.x, v2), mod(v1.y, v2));
    }

    template<typename T>
    inline base_vec2<T> modf(const base_vec2<T>& v, base_vec2<T>& i)
    {
        return base_vec2<T>(modf(v.x, i.x), modf(v.y, i.y));
    }

    template<typename T>
    inline base_vec2<T> wrap(const base_vec2<T>& v, f32 start, f32 end)
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
    constexpr base_vec2<T> clamp(const base_vec2<T>& v, f32 min, f32 max)
    {
        return base_vec2<T>(clamp(v.x, min, max), clamp(v.y, min, max));
    }

    template<typename T>
    constexpr base_vec2<T> clamp01(const base_vec2<T>& v)
    {
        return base_vec2<T>(clamp01(v.x), clamp01(v.y));
    }

    template<typename T>
    constexpr base_vec2<T> mix(const base_vec2<T>& v1, const base_vec2<T>& v2, f32 a)
    {
        return v1 + a * (v2 - v1);
    }

    template<typename T>
    constexpr base_vec2<T> remap(
        const base_vec2<T>& v,
        f32 a_start,
        f32 a_end,
        f32 b_start,
        f32 b_end
    )
    {
        return b_start + ((b_end - b_start) / (a_end - a_start)) * (v - a_start);
    }

    template<typename T>
    constexpr base_vec2<T> remap_clamp(
        const base_vec2<T>& v,
        f32 a_start,
        f32 a_end,
        f32 b_start,
        f32 b_end
    )
    {
        base_vec2<T> t = clamp01((v - a_start) / (a_end - a_start));
        return b_start + t * (b_end - b_start);
    }

    template<typename T>
    constexpr base_vec2<T> remap01(const base_vec2<T>& v, f32 a_start, f32 a_end)
    {
        return clamp01((v - a_start) / (a_end - a_start));
    }

    template<typename T>
    constexpr base_vec2<T> step(f32 edge, const base_vec2<T>& v)
    {
        return base_vec2<T>(step(edge, v.x), step(edge, v.y));
    }

    template<typename T>
    constexpr base_vec2<T> smoothstep(f32 edge0, f32 edge1, const base_vec2<T>& v)
    {
        return base_vec2<T>(smoothstep(edge0, edge1, v.x), smoothstep(edge0, edge1, v.y));
    }

    template<typename T>
    constexpr f32 length_squared(const base_vec2<T>& v)
    {
        return v.x * v.x + v.y * v.y;
    }

    template<typename T>
    inline f32 length(const base_vec2<T>& v)
    {
        return sqrt(length_squared(v));
    }

    template<typename T>
    constexpr f32 distance_squared(const base_vec2<T>& v1, const base_vec2<T>& v2)
    {
        return length_squared(v1 - v2);
    }

    template<typename T>
    inline f32 distance(const base_vec2<T>& v1, const base_vec2<T>& v2)
    {
        return length(v1 - v2);
    }

    template<typename T>
    constexpr T dot(const base_vec2<T>& v1, const base_vec2<T>& v2)
    {
        return v1.x * v2.x + v1.y * v2.y;
    }

    template<typename T>
    inline base_vec2<T> normalize(const base_vec2<T>& v)
    {
        return v / length(v);
    }

    template<typename T>
    constexpr base_vec2<T> faceforward(
        const base_vec2<T>& N,
        const base_vec2<T>& I,
        const base_vec2<T>& Nref
    )
    {
        if (dot(Nref, I) < 0)
            return N;
        return -N;
    }

    template<typename T>
    constexpr base_vec2<T> reflect(const base_vec2<T>& I, const base_vec2<T>& N)
    {
        return I - 2 * dot(N, I) * N;
    }

    template<typename T>
    inline base_vec2<T> refract(const base_vec2<T>& I, const base_vec2<T>& N, f32 eta)
    {
        f32 dp = dot(N, I);
        f32 k = 1 - eta * eta * (1 - dp * dp);

        if (k < 0)
            return 0;

        return eta * I - (eta * dp + sqrt(k)) * N;
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

    // Type definitions
    using vec2 = base_vec2<f32>;
    using ivec2 = base_vec2<i32>;
    using uvec2 = base_vec2<u32>;

}
