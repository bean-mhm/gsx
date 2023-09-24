#pragma once

#include "utils.h"

#include "../str/utils.h"

namespace tef::math
{

    template <typename T>
    class vec2_base
    {
    public:
        T x = 0, y = 0;

        // Constructors

        constexpr vec2_base() = default;

        constexpr vec2_base(T x, T y)
            : x(x), y(y)
        {}

        constexpr vec2_base(T s)
            : x(s), y(s)
        {}

        // Type casting
        template <typename U>
        constexpr operator vec2_base<U>() const
        {
            return vec2_base<U>((U)x, (U)y);
        }

        // String
        constexpr std::string to_string() const
        {
            return std::format(
                "[{}, {}]",
                str::from_number(x),
                str::from_number(y)
            );
        }

        // Print
        friend std::ostream& operator<<(std::ostream& os, const vec2_base<T>& v)
        {
            os << v.to_string();
            return os;
        }

        // this + vector (may be implicitly converted from a scalar)
        constexpr vec2_base<T> operator+(const vec2_base<T>& v) const
        {
            return vec2_base<T>(x + v.x, y + v.y);
        }

        // this += vector (may be implicitly converted from a scalar)
        constexpr vec2_base<T>& operator+=(const vec2_base<T>& v)
        {
            x += v.x;
            y += v.y;
            return *this;
        }

        // this - vector (may be implicitly converted from a scalar)
        constexpr vec2_base<T> operator-(const vec2_base<T>& v) const
        {
            return vec2_base<T>(x - v.x, y - v.y);
        }

        // this -= vector (may be implicitly converted from a scalar)
        constexpr vec2_base<T>& operator-=(const vec2_base<T>& v)
        {
            x -= v.x;
            y -= v.y;
            return *this;
        }

        // this * scalar
        constexpr vec2_base<T> operator*(T s) const
        {
            return vec2_base<T>(s * x, s * y);
        }

        // this *= scalar
        constexpr vec2_base<T>& operator*=(T s)
        {
            x *= s;
            y *= s;
            return *this;
        }

        // this * vector (component-wise multiplication)
        constexpr vec2_base<T> operator*(vec2_base<T> v) const
        {
            return vec2_base<T>(x * v.x, y * v.y);
        }

        // this *= vector (component-wise multiplication)
        constexpr vec2_base<T>& operator*=(vec2_base<T> v)
        {
            x *= v.x;
            y *= v.y;
            return *this;
        }

        // this / scalar
        constexpr vec2_base<T> operator/(T s) const
        {
            float inv = 1.f / (float)s;
            return vec2_base<T>(x * inv, y * inv);
        }

        // this /= scalar
        constexpr vec2_base<T>& operator/=(T s)
        {
            float inv = 1.f / (float)s;
            x *= inv;
            y *= inv;
            return *this;
        }

        // this / vector (component-wise division)
        constexpr vec2_base<T> operator/(vec2_base<T> v) const
        {
            return vec2_base<T>(x / v.x, y / v.y);
        }

        // this /= vector (component-wise division)
        constexpr vec2_base<T>& operator/=(vec2_base<T> v)
        {
            x /= v.x;
            y /= v.y;
            return *this;
        }

        // this == vector
        constexpr bool operator==(const vec2_base<T>& v) const
        {
            return x == v.x && y == v.y;
        }

        // this != vector
        constexpr bool operator!=(const vec2_base<T>& v) const
        {
            return x != v.x || y != v.y;
        }

        // (-1) * this
        constexpr vec2_base<T> operator-() const
        {
            return vec2_base<T>(-x, -y);
        }

        // Access by index (copy)
        constexpr T operator[](int i) const
        {
            if (i == 0) return x;
            return y;
        }

        // Access by index (reference)
        constexpr T& operator[](int i)
        {
            if (i == 0) return x;
            return y;
        }

        // Permute by indices
        constexpr vec2_base<T> permute(int x, int y) const
        {
            return vec2_base<T>((*this)[x], (*this)[y]);
        }

        // Swapped components
        constexpr vec2_base<T> yx() const
        {
            return vec2_base<T>(y, x);
        }

    };

    // Scalar + vector
    template <typename T>
    constexpr vec2_base<T> operator+(T s, const vec2_base<T>& v)
    {
        return v + s;
    }

    // Scalar - vector
    template <typename T>
    constexpr vec2_base<T> operator-(T s, const vec2_base<T>& v)
    {
        return (-v) + s;
    }

    // Scalar * vector
    template <typename T>
    constexpr vec2_base<T> operator*(T s, const vec2_base<T>& v)
    {
        return v * s;
    }

    // Scalar / vector
    template <typename T>
    constexpr vec2_base<T> operator/(T s, const vec2_base<T>& v)
    {
        return vec2_base<T>(s / v.x, s / v.y);
    }

    template <typename T>
    constexpr vec2_base<T> radians(const vec2_base<T>& degrees)
    {
        return degrees * deg2rad;
    }

    template <typename T>
    constexpr vec2_base<T> degrees(const vec2_base<T>& radians)
    {
        return radians * rad2deg;
    }

    template <typename T>
    constexpr vec2_base<T> sin(const vec2_base<T>& v)
    {
        return vec2_base<T>(sin(v.x), sin(v.y));
    }

    template <typename T>
    constexpr vec2_base<T> cos(const vec2_base<T>& v)
    {
        return vec2_base<T>(cos(v.x), cos(v.y));
    }

    template <typename T>
    constexpr vec2_base<T> tan(const vec2_base<T>& v)
    {
        return vec2_base<T>(tan(v.x), tan(v.y));
    }

    template <typename T>
    constexpr vec2_base<T> asin(const vec2_base<T>& v)
    {
        return vec2_base<T>(asin(v.x), asin(v.y));
    }

    template <typename T>
    constexpr vec2_base<T> acos(const vec2_base<T>& v)
    {
        return vec2_base<T>(acos(v.x), acos(v.y));
    }

    template <typename T>
    constexpr vec2_base<T> atan(const vec2_base<T>& v)
    {
        return vec2_base<T>(atan(v.x), atan(v.y));
    }

    template <typename T>
    constexpr vec2_base<T> sinh(const vec2_base<T>& v)
    {
        return vec2_base<T>(sinh(v.x), sinh(v.y));
    }

    template <typename T>
    constexpr vec2_base<T> cosh(const vec2_base<T>& v)
    {
        return vec2_base<T>(cosh(v.x), cosh(v.y));
    }

    template <typename T>
    constexpr vec2_base<T> tanh(const vec2_base<T>& v)
    {
        return vec2_base<T>(tanh(v.x), tanh(v.y));
    }

    template <typename T>
    constexpr vec2_base<T> asinh(const vec2_base<T>& v)
    {
        return vec2_base<T>(asinh(v.x), asinh(v.y));
    }

    template <typename T>
    constexpr vec2_base<T> acosh(const vec2_base<T>& v)
    {
        return vec2_base<T>(acosh(v.x), acosh(v.y));
    }

    template <typename T>
    constexpr vec2_base<T> atanh(const vec2_base<T>& v)
    {
        return vec2_base<T>(atanh(v.x), atanh(v.y));
    }

    template <typename T>
    constexpr vec2_base<T> pow(const vec2_base<T>& v1, const vec2_base<T>& v2)
    {
        return vec2_base<T>(pow(v1.x, v2.x), pow(v1.y, v2.y));
    }

    template <typename T>
    constexpr vec2_base<T> exp(const vec2_base<T>& v)
    {
        return vec2_base<T>(exp(v.x), exp(v.y));
    }

    template <typename T>
    constexpr vec2_base<T> log(const vec2_base<T>& v)
    {
        return vec2_base<T>(log(v.x), log(v.y));
    }

    template <typename T>
    constexpr vec2_base<T> exp2(const vec2_base<T>& v)
    {
        return vec2_base<T>(exp2(v.x), exp2(v.y));
    }

    template <typename T>
    constexpr vec2_base<T> log2(const vec2_base<T>& v)
    {
        return vec2_base<T>(log2(v.x), log2(v.y));
    }

    template <typename T>
    constexpr vec2_base<T> sqrt(const vec2_base<T>& v)
    {
        return vec2_base<T>(sqrt(v.x), sqrt(v.y));
    }

    template <typename T>
    constexpr vec2_base<T> inversesqrt(const vec2_base<T>& v)
    {
        return vec2_base<T>(inversesqrt(v.x), inversesqrt(v.y));
    }

    template <typename T>
    constexpr vec2_base<T> abs(const vec2_base<T>& v)
    {
        return vec2_base<T>(abs(v.x), abs(v.y));
    }

    template <typename T>
    constexpr vec2_base<T> sign(const vec2_base<T>& v)
    {
        return vec2_base<T>(sign(v.x), sign(v.y));
    }

    template <typename T>
    constexpr vec2_base<T> floor(const vec2_base<T>& v)
    {
        return vec2_base<T>(floor(v.x), floor(v.y));
    }

    template <typename T>
    constexpr vec2_base<T> ceil(const vec2_base<T>& v)
    {
        return vec2_base<T>(ceil(v.x), ceil(v.y));
    }

    template <typename T>
    constexpr vec2_base<T> trunc(const vec2_base<T>& v)
    {
        return vec2_base<T>(trunc(v.x), trunc(v.y));
    }

    template <typename T>
    constexpr vec2_base<T> fract(const vec2_base<T>& v)
    {
        return vec2_base<T>(fract(v.x), fract(v.y));
    }

    template <typename T>
    constexpr vec2_base<T> mod(const vec2_base<T>& v1, const vec2_base<T>& v2)
    {
        return vec2_base<T>(mod(v1.x, v2.x), mod(v1.y, v2.y));
    }

    template <typename T>
    constexpr vec2_base<T> mod(const vec2_base<T>& v1, float v2)
    {
        return vec2_base<T>(mod(v1.x, v2), mod(v1.y, v2));
    }

    template <typename T>
    constexpr vec2_base<T> modf(const vec2_base<T>& v, vec2_base<T>& i)
    {
        return vec2_base<T>(modf(v.x, i.x), modf(v.y, i.y));
    }

    template <typename T>
    constexpr vec2_base<T> wrap(const vec2_base<T>& v, float start, float end)
    {
        return start + mod(v - start, end - start);
    }

    template <typename T>
    constexpr vec2_base<T> min(const vec2_base<T>& v1, const vec2_base<T>& v2)
    {
        return vec2_base<T>(
            min(v1.x, v2.x),
            min(v1.y, v2.y)
        );
    }

    template <typename T>
    constexpr vec2_base<T> max(const vec2_base<T>& v1, const vec2_base<T>& v2)
    {
        return vec2_base<T>(
            max(v1.x, v2.x),
            max(v1.y, v2.y)
        );
    }

    template <typename T>
    constexpr vec2_base<T> clamp(const vec2_base<T>& v, float min, float max)
    {
        return vec2_base<T>(clamp(v.x, min, max), clamp(v.y, min, max));
    }

    template <typename T>
    constexpr vec2_base<T> clamp01(const vec2_base<T>& v)
    {
        return vec2_base<T>(clamp01(v.x), clamp01(v.y));
    }

    template <typename T>
    constexpr vec2_base<T> mix(const vec2_base<T>& v1, const vec2_base<T>& v2, float a)
    {
        return v1 + a * (v2 - v1);
    }

    template <typename T>
    constexpr vec2_base<T> remap(
        const vec2_base<T>& v,
        float a_start,
        float a_end,
        float b_start,
        float b_end
    )
    {
        return b_start + ((b_end - b_start) / (a_end - a_start)) * (v - a_start);
    }

    template <typename T>
    constexpr vec2_base<T> remap_clamp(
        const vec2_base<T>& v,
        float a_start,
        float a_end,
        float b_start,
        float b_end
    )
    {
        vec2_base<T> t = clamp01((v - a_start) / (a_end - a_start));
        return b_start + t * (b_end - b_start);
    }

    template <typename T>
    constexpr vec2_base<T> remap01(const vec2_base<T>& v, float a_start, float a_end)
    {
        return clamp01((v - a_start) / (a_end - a_start));
    }

    template <typename T>
    constexpr vec2_base<T> step(float edge, const vec2_base<T>& v)
    {
        return vec2_base<T>(step(edge, v.x), step(edge, v.y));
    }

    template <typename T>
    constexpr vec2_base<T> smoothstep(float edge0, float edge1, const vec2_base<T>& v)
    {
        return vec2_base<T>(smoothstep(edge0, edge1, v.x), smoothstep(edge0, edge1, v.y));
    }

    template <typename T>
    constexpr float length_squared(const vec2_base<T>& v)
    {
        return v.x * v.x + v.y * v.y;
    }

    template <typename T>
    constexpr float length(const vec2_base<T>& v)
    {
        return sqrt(length_squared(v));
    }

    template <typename T>
    constexpr float distance_squared(const vec2_base<T>& v1, const vec2_base<T>& v2)
    {
        return length_squared(v1 - v2);
    }

    template <typename T>
    constexpr float distance(const vec2_base<T>& v1, const vec2_base<T>& v2)
    {
        return length(v1 - v2);
    }

    template <typename T>
    constexpr T dot(const vec2_base<T>& v1, const vec2_base<T>& v2)
    {
        return v1.x * v2.x + v1.y * v2.y;
    }

    template <typename T>
    constexpr vec2_base<T> normalize(const vec2_base<T>& v)
    {
        return v / length(v);
    }

    template <typename T>
    constexpr vec2_base<T> faceforward(
        const vec2_base<T>& N,
        const vec2_base<T>& I,
        const vec2_base<T>& Nref
    )
    {
        if (dot(Nref, I) < 0)
            return N;
        return -N;
    }

    template <typename T>
    constexpr vec2_base<T> reflect(const vec2_base<T>& I, const vec2_base<T>& N)
    {
        return I - 2 * dot(N, I) * N;
    }

    template <typename T>
    constexpr vec2_base<T> refract(const vec2_base<T>& I, const vec2_base<T>& N, float eta)
    {
        float dp = dot(N, I);
        float k = 1 - eta * eta * (1 - dp * dp);

        if (k < 0)
            return 0;

        return eta * I - (eta * dp + sqrt(k)) * N;
    }

    template <typename T>
    constexpr T min_component(const vec2_base<T>& v)
    {
        return min(v.x, v.y);
    }

    template <typename T>
    constexpr T max_component(const vec2_base<T>& v)
    {
        return max(v.x, v.y);
    }

    template <typename T>
    constexpr int min_component_index(const vec2_base<T>& v)
    {
        return (v.x < v.y) ? 0 : 1;
    }

    template <typename T>
    constexpr int max_component_index(const vec2_base<T>& v)
    {
        return (v.x > v.y) ? 0 : 1;
    }

    // Type definitions
    using vec2 = vec2_base<float>;
    using ivec2 = vec2_base<int32_t>;
    using uvec2 = vec2_base<uint32_t>;

}
