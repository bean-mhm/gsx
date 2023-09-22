#pragma once

#include "vec2.h"
#include "vec3.h"
#include "utils.h"

#include "../str/utils.h"

namespace tef::math
{

    template <typename T>
    class vec4_base
    {
    public:
        T x = 0, y = 0, z = 0, w = 0;

        // Constructors

        constexpr vec4_base() = default;

        constexpr vec4_base(T x, T y, T z, T w)
            : x(x), y(y), z(z), w(w)
        {}

        constexpr vec4_base(T s)
            : x(s), y(s), z(s), w(s)
        {}

        constexpr vec4_base(vec2_base<T> xy, T z, T w)
            : x(xy.x), y(xy.y), z(z), w(w)
        {}

        constexpr vec4_base(T x, vec2_base<T> yz, T w)
            : x(x), y(yz.x), z(yz.y), w(w)
        {}

        constexpr vec4_base(T x, T y, vec2_base<T> zw)
            : x(x), y(y), z(zw.x), w(zw.y)
        {}

        constexpr vec4_base(vec2_base<T> xy, vec2_base<T> zw)
            : x(xy.x), y(xy.y), z(zw.x), w(zw.y)
        {}

        constexpr vec4_base(vec3_base<T> xyz, T w)
            : x(xyz.x), y(xyz.y), z(xyz.z), w(w)
        {}

        constexpr vec4_base(T x, vec3_base<T> yzw)
            : x(x), y(yzw.x), z(yzw.y), w(yzw.z)
        {}

        // Type casting
        template <typename U>
        constexpr operator vec4_base<U>() const
        {
            return vec4_base<U>((U)x, (U)y, (U)z, (U)w);
        }

        // String
        constexpr std::string to_string() const
        {
            return std::format(
                "[{}, {}, {}, {}]",
                str::from_number(x),
                str::from_number(y),
                str::from_number(z),
                str::from_number(w)
            );
        }

        // Print
        friend std::ostream& operator<<(std::ostream& os, const vec2_base<T>& v)
        {
            os << v.to_string();
            return os;
        }

        // this + vector (may be implicitly converted from a scalar)
        constexpr vec4_base<T> operator+(const vec4_base<T>& v) const
        {
            return vec4_base<T>(x + v.x, y + v.y, z + v.z, w + v.w);
        }

        // this += vector (may be implicitly converted from a scalar)
        constexpr vec4_base<T>& operator+=(const vec4_base<T>& v)
        {
            x += v.x;
            y += v.y;
            z += v.z;
            w += v.w;
            return *this;
        }

        // this - vector (may be implicitly converted from a scalar)
        constexpr vec4_base<T> operator-(const vec4_base<T>& v) const
        {
            return vec4_base<T>(x - v.x, y - v.y, z - v.z, w - v.w);
        }

        // this -= vector (may be implicitly converted from a scalar)
        constexpr vec4_base<T>& operator-=(const vec4_base<T>& v)
        {
            x -= v.x;
            y -= v.y;
            z -= v.z;
            w -= v.w;
            return *this;
        }

        // this * scalar
        constexpr vec4_base<T> operator*(T s) const
        {
            return vec4_base<T>(s * x, s * y, s * z, s * w);
        }

        // this *= scalar
        constexpr vec4_base<T>& operator*=(T s)
        {
            x *= s;
            y *= s;
            z *= s;
            w *= s;
            return *this;
        }

        // this * vector (component-wise multiplication)
        constexpr vec4_base<T> operator*(vec4_base<T> v) const
        {
            return vec4_base<T>(x * v.x, y * v.y, z * v.z, w * v.w);
        }

        // this *= vector (component-wise multiplication)
        constexpr vec4_base<T>& operator*=(vec4_base<T> v)
        {
            x *= v.x;
            y *= v.y;
            z *= v.z;
            w *= v.w;
            return *this;
        }

        // this / scalar
        constexpr vec4_base<T> operator/(T s) const
        {
            float inv = 1.f / (float)s;
            return vec4_base<T>(x * inv, y * inv, z * inv, w * inv);
        }

        // this /= scalar
        constexpr vec4_base<T>& operator/=(T s)
        {
            float inv = 1.f / (float)s;
            x *= inv;
            y *= inv;
            z *= inv;
            w *= inv;
            return *this;
        }

        // this / vector (component-wise division)
        constexpr vec4_base<T> operator/(vec4_base<T> v) const
        {
            return vec4_base<T>(x / v.x, y / v.y, z / v.z, w / v.w);
        }

        // this /= vector (component-wise division)
        constexpr vec4_base<T>& operator/=(vec4_base<T> v)
        {
            x /= v.x;
            y /= v.y;
            z /= v.z;
            w /= v.w;
            return *this;
        }

        // this == vector
        constexpr bool operator==(const vec4_base<T>& v) const
        {
            return x == v.x && y == v.y && z == v.z && w == v.w;
        }

        // this != vector
        constexpr bool operator!=(const vec4_base<T>& v) const
        {
            return x != v.x || y != v.y || z != v.z || w != v.w;
        }

        // (-1) * this
        constexpr vec4_base<T> operator-() const
        {
            return vec4_base<T>(-x, -y, -z, -w);
        }

        // Access by index (copy)
        constexpr T operator[](int i) const
        {
            if (i == 0) return x;
            if (i == 1) return y;
            if (i == 2) return z;
            return w;
        }

        // Access by index (reference)
        constexpr T& operator[](int i)
        {
            if (i == 0) return x;
            if (i == 1) return y;
            if (i == 2) return z;
            return w;
        }

        // Permute by indices

        constexpr vec2_base<T> permute(int x, int y) const
        {
            return vec2_base<T>((*this)[x], (*this)[y]);
        }

        constexpr vec3_base<T> permute(int x, int y, int z) const
        {
            return vec3_base<T>((*this)[x], (*this)[y], (*this)[z]);
        }

        constexpr vec4_base<T> permute(int x, int y, int z, int w) const
        {
            return vec4_base<T>((*this)[x], (*this)[y], (*this)[z], (*this)[w]);
        }

    };

    // Scalar + vector
    template <typename T>
    constexpr vec4_base<T> operator+(T s, const vec4_base<T>& v)
    {
        return v + s;
    }

    // Scalar - vector
    template <typename T>
    constexpr vec4_base<T> operator-(T s, const vec4_base<T>& v)
    {
        return (-v) + s;
    }

    // Scalar * vector
    template <typename T>
    constexpr vec4_base<T> operator*(T s, const vec4_base<T>& v)
    {
        return v * s;
    }

    // Scalar / vector
    template <typename T>
    constexpr vec4_base<T> operator/(T s, const vec4_base<T>& v)
    {
        return vec4_base<T>(s / v.x, s / v.y, s / v.z, s / v.w);
    }

    template <typename T>
    constexpr vec4_base<T> radians(const vec4_base<T>& degrees)
    {
        return degrees * deg2rad;
    }

    template <typename T>
    constexpr vec4_base<T> degrees(const vec4_base<T>& radians)
    {
        return radians * rad2deg;
    }

    template <typename T>
    constexpr vec4_base<T> sin(const vec4_base<T>& v)
    {
        return vec4_base<T>(sin(v.x), sin(v.y), sin(v.z), sin(v.w));
    }

    template <typename T>
    constexpr vec4_base<T> cos(const vec4_base<T>& v)
    {
        return vec4_base<T>(cos(v.x), cos(v.y), cos(v.z), cos(v.w));
    }

    template <typename T>
    constexpr vec4_base<T> tan(const vec4_base<T>& v)
    {
        return vec4_base<T>(tan(v.x), tan(v.y), tan(v.z), tan(v.w));
    }

    template <typename T>
    constexpr vec4_base<T> asin(const vec4_base<T>& v)
    {
        return vec4_base<T>(asin(v.x), asin(v.y), asin(v.z), asin(v.w));
    }

    template <typename T>
    constexpr vec4_base<T> acos(const vec4_base<T>& v)
    {
        return vec4_base<T>(acos(v.x), acos(v.y), acos(v.z), acos(v.w));
    }

    template <typename T>
    constexpr vec4_base<T> atan(const vec4_base<T>& v)
    {
        return vec4_base<T>(atan(v.x), atan(v.y), atan(v.z), atan(v.w));
    }

    template <typename T>
    constexpr vec4_base<T> sinh(const vec4_base<T>& v)
    {
        return vec4_base<T>(sinh(v.x), sinh(v.y), sinh(v.z), sinh(v.w));
    }

    template <typename T>
    constexpr vec4_base<T> cosh(const vec4_base<T>& v)
    {
        return vec4_base<T>(cosh(v.x), cosh(v.y), cosh(v.z), cosh(v.w));
    }

    template <typename T>
    constexpr vec4_base<T> tanh(const vec4_base<T>& v)
    {
        return vec4_base<T>(tanh(v.x), tanh(v.y), tanh(v.z), tanh(v.w));
    }

    template <typename T>
    constexpr vec4_base<T> asinh(const vec4_base<T>& v)
    {
        return vec4_base<T>(asinh(v.x), asinh(v.y), asinh(v.z), asinh(v.w));
    }

    template <typename T>
    constexpr vec4_base<T> acosh(const vec4_base<T>& v)
    {
        return vec4_base<T>(acosh(v.x), acosh(v.y), acosh(v.z), acosh(v.w));
    }

    template <typename T>
    constexpr vec4_base<T> atanh(const vec4_base<T>& v)
    {
        return vec4_base<T>(atanh(v.x), atanh(v.y), atanh(v.z), atanh(v.w));
    }

    template <typename T>
    constexpr vec4_base<T> pow(const vec4_base<T>& v1, const vec4_base<T>& v2)
    {
        return vec4_base<T>(pow(v1.x, v2.x), pow(v1.y, v2.y), pow(v1.z, v2.z), pow(v1.w, v2.w));
    }

    template <typename T>
    constexpr vec4_base<T> exp(const vec4_base<T>& v)
    {
        return vec4_base<T>(exp(v.x), exp(v.y), exp(v.z), exp(v.w));
    }

    template <typename T>
    constexpr vec4_base<T> log(const vec4_base<T>& v)
    {
        return vec4_base<T>(log(v.x), log(v.y), log(v.z), log(v.w));
    }

    template <typename T>
    constexpr vec4_base<T> exp2(const vec4_base<T>& v)
    {
        return vec4_base<T>(exp2(v.x), exp2(v.y), exp2(v.z), exp2(v.w));
    }

    template <typename T>
    constexpr vec4_base<T> log2(const vec4_base<T>& v)
    {
        return vec4_base<T>(log2(v.x), log2(v.y), log2(v.z), log2(v.w));
    }

    template <typename T>
    constexpr vec4_base<T> sqrt(const vec4_base<T>& v)
    {
        return vec4_base<T>(sqrt(v.x), sqrt(v.y), sqrt(v.z), sqrt(v.w));
    }

    template <typename T>
    constexpr vec4_base<T> inversesqrt(const vec4_base<T>& v)
    {
        return vec4_base<T>(
            inversesqrt(v.x),
            inversesqrt(v.y),
            inversesqrt(v.z),
            inversesqrt(v.w)
        );
    }

    template <typename T>
    constexpr vec4_base<T> abs(const vec4_base<T>& v)
    {
        return vec4_base<T>(abs(v.x), abs(v.y), abs(v.z), abs(v.w));
    }

    template <typename T>
    constexpr vec4_base<T> sign(const vec4_base<T>& v)
    {
        return vec4_base<T>(sign(v.x), sign(v.y), sign(v.z), sign(v.w));
    }

    template <typename T>
    constexpr vec4_base<T> floor(const vec4_base<T>& v)
    {
        return vec4_base<T>(floor(v.x), floor(v.y), floor(v.z), floor(v.w));
    }

    template <typename T>
    constexpr vec4_base<T> ceil(const vec4_base<T>& v)
    {
        return vec4_base<T>(ceil(v.x), ceil(v.y), ceil(v.z), ceil(v.w));
    }

    template <typename T>
    constexpr vec4_base<T> trunc(const vec4_base<T>& v)
    {
        return vec4_base<T>(trunc(v.x), trunc(v.y), trunc(v.z), trunc(v.w));
    }

    template <typename T>
    constexpr vec4_base<T> fract(const vec4_base<T>& v)
    {
        return vec4_base<T>(fract(v.x), fract(v.y), fract(v.z), fract(v.w));
    }

    template <typename T>
    constexpr vec4_base<T> mod(const vec4_base<T>& v1, const vec4_base<T>& v2)
    {
        return vec4_base<T>(mod(v1.x, v2.x), mod(v1.y, v2.y), mod(v1.z, v2.z), mod(v1.w, v2.w));
    }

    template <typename T>
    constexpr vec4_base<T> mod(const vec4_base<T>& v1, float v2)
    {
        return vec4_base<T>(mod(v1.x, v2), mod(v1.y, v2), mod(v1.z, v2), mod(v1.w, v2));
    }

    template <typename T>
    constexpr vec4_base<T> modf(const vec4_base<T>& v, vec4_base<T>& i)
    {
        return vec4_base<T>(modf(v.x, i.x), modf(v.y, i.y), modf(v.z, i.z), modf(v.w, i.w));
    }

    template <typename T>
    constexpr vec4_base<T> wrap(const vec4_base<T>& v, float start, float end)
    {
        return start + mod(v - start, end - start);
    }

    template <typename T>
    constexpr vec4_base<T> min(const vec4_base<T>& v1, const vec4_base<T>& v2)
    {
        return vec4_base<T>(
            min(v1.x, v2.x),
            min(v1.y, v2.y),
            min(v1.z, v2.z),
            min(v1.w, v2.w)
        );
    }

    template <typename T>
    constexpr vec4_base<T> max(const vec4_base<T>& v1, const vec4_base<T>& v2)
    {
        return vec4_base<T>(
            max(v1.x, v2.x),
            max(v1.y, v2.y),
            max(v1.z, v2.z),
            max(v1.w, v2.w)
        );
    }

    template <typename T>
    constexpr vec4_base<T> clamp(const vec4_base<T>& v, float min, float max)
    {
        return vec4_base<T>(
            clamp(v.x, min, max),
            clamp(v.y, min, max),
            clamp(v.z, min, max),
            clamp(v.w, min, max)
        );
    }

    template <typename T>
    constexpr vec4_base<T> clamp01(const vec4_base<T>& v)
    {
        return vec4_base<T>(clamp01(v.x), clamp01(v.y), clamp01(v.z), clamp01(v.w));
    }

    template <typename T>
    constexpr vec4_base<T> mix(const vec4_base<T>& v1, const vec4_base<T>& v2, float a)
    {
        return v1 + a * (v2 - v1);
    }

    template <typename T>
    constexpr vec4_base<T> remap(
        const vec4_base<T>& v,
        float a_start,
        float a_end,
        float b_start,
        float b_end
    )
    {
        return b_start + ((b_end - b_start) / (a_end - a_start)) * (v - a_start);
    }

    template <typename T>
    constexpr vec4_base<T> remap_clamp(
        const vec4_base<T>& v,
        float a_start,
        float a_end,
        float b_start,
        float b_end
    )
    {
        vec4_base<T> t = clamp01((v - a_start) / (a_end - a_start));
        return b_start + t * (b_end - b_start);
    }

    template <typename T>
    constexpr vec4_base<T> remap01(const vec4_base<T>& v, float a_start, float a_end)
    {
        return clamp01((v - a_start) / (a_end - a_start));
    }

    template <typename T>
    constexpr vec4_base<T> step(float edge, const vec4_base<T>& v)
    {
        return vec4_base<T>(step(edge, v.x), step(edge, v.y), step(edge, v.z), step(edge, v.w));
    }

    template <typename T>
    constexpr vec4_base<T> smoothstep(float edge0, float edge1, const vec4_base<T>& v)
    {
        return vec4_base<T>(
            smoothstep(edge0, edge1, v.x),
            smoothstep(edge0, edge1, v.y),
            smoothstep(edge0, edge1, v.z),
            smoothstep(edge0, edge1, v.w)
        );
    }

    template <typename T>
    constexpr float length_squared(const vec4_base<T>& v)
    {
        return v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w;
    }

    template <typename T>
    constexpr float length(const vec4_base<T>& v)
    {
        return sqrt(length_squared(v));
    }

    template <typename T>
    constexpr float distance_squared(const vec4_base<T>& v1, const vec4_base<T>& v2)
    {
        return length_squared(v1 - v2);
    }

    template <typename T>
    constexpr float distance(const vec4_base<T>& v1, const vec4_base<T>& v2)
    {
        return length(v1 - v2);
    }

    template <typename T>
    constexpr T dot(const vec4_base<T>& v1, const vec4_base<T>& v2)
    {
        return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w;
    }

    template <typename T>
    constexpr vec4_base<T> normalize(const vec4_base<T>& v)
    {
        return v / length(v);
    }

    template <typename T>
    constexpr vec4_base<T> faceforward(
        const vec4_base<T>& N,
        const vec4_base<T>& I,
        const vec4_base<T>& Nref
    )
    {
        if (dot(Nref, I) < 0)
            return N;
        return -N;
    }

    template <typename T>
    constexpr vec4_base<T> reflect(const vec4_base<T>& I, const vec4_base<T>& N)
    {
        return I - 2 * dot(N, I) * N;
    }

    template <typename T>
    constexpr vec4_base<T> refract(const vec4_base<T>& I, const vec4_base<T>& N, float eta)
    {
        float dp = dot(N, I);
        float k = 1 - eta * eta * (1 - dp * dp);

        if (k < 0)
            return 0;

        return eta * I - (eta * dp + sqrt(k)) * N;
    }

    template <typename T>
    constexpr T min_component(const vec4_base<T>& v)
    {
        return min(v.x, min(v.y, min(v.z, v.w)));
    }

    template <typename T>
    constexpr T max_component(const vec4_base<T>& v)
    {
        return max(v.x, max(v.y, max(v.z, v.w)));
    }

    template <typename T>
    constexpr int min_component_index(const vec4_base<T>& v)
    {
        return (v.x < v.y)
            ? ((v.x < v.z)
                ? (v.x < v.w ? 0 : 3)
                : (v.z < v.w ? 2 : 3))
            : ((v.y < v.z)
                ? (v.y < v.w ? 1 : 3)
                : (v.z < v.w ? 2 : 3));
    }

    template <typename T>
    constexpr int max_component_index(const vec4_base<T>& v)
    {
        return (v.x > v.y)
            ? ((v.x > v.z)
                ? (v.x > v.w ? 0 : 3)
                : (v.z > v.w ? 2 : 3))
            : ((v.y > v.z)
                ? (v.y > v.w ? 1 : 3)
                : (v.z > v.w ? 2 : 3));
    }

    // Type definitions
    typedef vec4_base<float> vec4;
    typedef vec4_base<int32_t> ivec4;
    typedef vec4_base<uint32_t> uvec4;

}
