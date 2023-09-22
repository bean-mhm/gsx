#pragma once

#include "vec2.h"
#include "utils.h"

#include "../str/utils.h"

namespace tef::math
{

    template <typename T>
    class vec3_base
    {
    public:
        T x = 0, y = 0, z = 0;

        // Constructors

        constexpr vec3_base() = default;

        constexpr vec3_base(T x, T y, T z)
            : x(x), y(y), z(z)
        {}

        constexpr vec3_base(T s)
            : x(s), y(s), z(s)
        {}

        constexpr vec3_base(vec2_base<T> xy, T z)
            : x(xy.x), y(xy.y), z(z)
        {}

        constexpr vec3_base(T x, vec2_base<T> yz)
            : x(x), y(yz.x), z(yz.y)
        {}

        // Type casting
        template <typename U>
        operator vec3_base<U>() const
        {
            return vec3_base<U>((U)x, (U)y, (U)z);
        }

        // String
        operator std::string() const
        {
            return
                "["
                + str::from_number(x)
                + ", "
                + str::from_number(y)
                + ", "
                + str::from_number(z)
                + "]";
        }

        // Print
        friend std::ostream& operator<<(std::ostream& os, const vec3_base<T>& v)
        {
            os << std::string(v);
            return os;
        }

        // this + vector (may be implicitly converted from a scalar)
        vec3_base<T> operator+(const vec3_base<T>& v) const
        {
            return vec3_base<T>(x + v.x, y + v.y, z + v.z);
        }

        // this += vector (may be implicitly converted from a scalar)
        vec3_base<T>& operator+=(const vec3_base<T>& v)
        {
            x += v.x;
            y += v.y;
            z += v.z;
            return *this;
        }

        // this - vector (may be implicitly converted from a scalar)
        vec3_base<T> operator-(const vec3_base<T>& v) const
        {
            return vec3_base<T>(x - v.x, y - v.y, z - v.z);
        }

        // this -= vector (may be implicitly converted from a scalar)
        vec3_base<T>& operator-=(const vec3_base<T>& v)
        {
            x -= v.x;
            y -= v.y;
            z -= v.z;
            return *this;
        }

        // this * scalar
        vec3_base<T> operator*(T s) const
        {
            return vec3_base<T>(s * x, s * y, s * z);
        }

        // this *= scalar
        vec3_base<T>& operator*=(T s)
        {
            x *= s;
            y *= s;
            z *= s;
            return *this;
        }

        // this * vector (component-wise multiplication)
        vec3_base<T> operator*(vec3_base<T> v) const
        {
            return vec3_base<T>(x * v.x, y * v.y, z * v.z);
        }

        // this *= vector (component-wise multiplication)
        vec3_base<T>& operator*=(vec3_base<T> v)
        {
            x *= v.x;
            y *= v.y;
            z *= v.z;
            return *this;
        }

        // this / scalar
        vec3_base<T> operator/(T s) const
        {
            float inv = 1.f / (float)s;
            return vec3_base<T>(x * inv, y * inv, z * inv);
        }

        // this /= scalar
        vec3_base<T>& operator/=(T s)
        {
            float inv = 1.f / (float)s;
            x *= inv;
            y *= inv;
            z *= inv;
            return *this;
        }

        // this / vector (component-wise division)
        vec3_base<T> operator/(vec3_base<T> v) const
        {
            return vec3_base<T>(x / v.x, y / v.y, z / v.z);
        }

        // this /= vector (component-wise division)
        vec3_base<T>& operator/=(vec3_base<T> v)
        {
            x /= v.x;
            y /= v.y;
            z /= v.z;
            return *this;
        }

        // this == vector
        bool operator==(const vec3_base<T>& v) const
        {
            return x == v.x && y == v.y && z == v.z;
        }

        // this != vector
        bool operator!=(const vec3_base<T>& v) const
        {
            return x != v.x || y != v.y || z != v.z;
        }

        // (-1) * this
        vec3_base<T> operator-() const
        {
            return vec3_base<T>(-x, -y, -z);
        }

        // Access by index (copy)
        T operator[](int i) const
        {
            if (i == 0) return x;
            if (i == 1) return y;
            return z;
        }

        // Access by index (reference)
        T& operator[](int i)
        {
            if (i == 0) return x;
            if (i == 1) return y;
            return z;
        }

        // Permute by indices

        vec2_base<T> permute(int x, int y) const
        {
            return vec2_base<T>((*this)[x], (*this)[y]);
        }

        vec3_base<T> permute(int x, int y, int z) const
        {
            return vec3_base<T>((*this)[x], (*this)[y], (*this)[z]);
        }

    };

    // Scalar + vector
    template <typename T>
    inline vec3_base<T> operator+(T s, const vec3_base<T>& v)
    {
        return v + s;
    }

    // Scalar - vector
    template <typename T>
    inline vec3_base<T> operator-(T s, const vec3_base<T>& v)
    {
        return (-v) + s;
    }

    // Scalar * vector
    template <typename T>
    inline vec3_base<T> operator*(T s, const vec3_base<T>& v)
    {
        return v * s;
    }

    // Scalar / vector
    template <typename T>
    inline vec3_base<T> operator/(T s, const vec3_base<T>& v)
    {
        return vec3_base<T>(s / v.x, s / v.y, s / v.z);
    }

    template <typename T>
    inline vec3_base<T> radians(const vec3_base<T>& degrees)
    {
        return degrees * deg2rad;
    }

    template <typename T>
    inline vec3_base<T> degrees(const vec3_base<T>& radians)
    {
        return radians * rad2deg;
    }

    template <typename T>
    inline vec3_base<T> sin(const vec3_base<T>& v)
    {
        return vec3_base<T>(sin(v.x), sin(v.y), sin(v.z));
    }

    template <typename T>
    inline vec3_base<T> cos(const vec3_base<T>& v)
    {
        return vec3_base<T>(cos(v.x), cos(v.y), cos(v.z));
    }

    template <typename T>
    inline vec3_base<T> tan(const vec3_base<T>& v)
    {
        return vec3_base<T>(tan(v.x), tan(v.y), tan(v.z));
    }

    template <typename T>
    inline vec3_base<T> asin(const vec3_base<T>& v)
    {
        return vec3_base<T>(asin(v.x), asin(v.y), asin(v.z));
    }

    template <typename T>
    inline vec3_base<T> acos(const vec3_base<T>& v)
    {
        return vec3_base<T>(acos(v.x), acos(v.y), acos(v.z));
    }

    template <typename T>
    inline vec3_base<T> atan(const vec3_base<T>& v)
    {
        return vec3_base<T>(atan(v.x), atan(v.y), atan(v.z));
    }

    template <typename T>
    inline vec3_base<T> sinh(const vec3_base<T>& v)
    {
        return vec3_base<T>(sinh(v.x), sinh(v.y), sinh(v.z));
    }

    template <typename T>
    inline vec3_base<T> cosh(const vec3_base<T>& v)
    {
        return vec3_base<T>(cosh(v.x), cosh(v.y), cosh(v.z));
    }

    template <typename T>
    inline vec3_base<T> tanh(const vec3_base<T>& v)
    {
        return vec3_base<T>(tanh(v.x), tanh(v.y), tanh(v.z));
    }

    template <typename T>
    inline vec3_base<T> asinh(const vec3_base<T>& v)
    {
        return vec3_base<T>(asinh(v.x), asinh(v.y), asinh(v.z));
    }

    template <typename T>
    inline vec3_base<T> acosh(const vec3_base<T>& v)
    {
        return vec3_base<T>(acosh(v.x), acosh(v.y), acosh(v.z));
    }

    template <typename T>
    inline vec3_base<T> atanh(const vec3_base<T>& v)
    {
        return vec3_base<T>(atanh(v.x), atanh(v.y), atanh(v.z));
    }

    template <typename T>
    inline vec3_base<T> pow(const vec3_base<T>& v1, const vec3_base<T>& v2)
    {
        return vec3_base<T>(pow(v1.x, v2.x), pow(v1.y, v2.y), pow(v1.z, v2.z));
    }

    template <typename T>
    inline vec3_base<T> exp(const vec3_base<T>& v)
    {
        return vec3_base<T>(exp(v.x), exp(v.y), exp(v.z));
    }

    template <typename T>
    inline vec3_base<T> log(const vec3_base<T>& v)
    {
        return vec3_base<T>(log(v.x), log(v.y), log(v.z));
    }

    template <typename T>
    inline vec3_base<T> exp2(const vec3_base<T>& v)
    {
        return vec3_base<T>(exp2(v.x), exp2(v.y), exp2(v.z));
    }

    template <typename T>
    inline vec3_base<T> log2(const vec3_base<T>& v)
    {
        return vec3_base<T>(log2(v.x), log2(v.y), log2(v.z));
    }

    template <typename T>
    inline vec3_base<T> sqrt(const vec3_base<T>& v)
    {
        return vec3_base<T>(sqrt(v.x), sqrt(v.y), sqrt(v.z));
    }

    template <typename T>
    inline vec3_base<T> inversesqrt(const vec3_base<T>& v)
    {
        return vec3_base<T>(inversesqrt(v.x), inversesqrt(v.y), inversesqrt(v.z));
    }

    template <typename T>
    inline vec3_base<T> abs(const vec3_base<T>& v)
    {
        return vec3_base<T>(abs(v.x), abs(v.y), abs(v.z));
    }

    template <typename T>
    inline vec3_base<T> sign(const vec3_base<T>& v)
    {
        return vec3_base<T>(sign(v.x), sign(v.y), sign(v.z));
    }

    template <typename T>
    inline vec3_base<T> floor(const vec3_base<T>& v)
    {
        return vec3_base<T>(floor(v.x), floor(v.y), floor(v.z));
    }

    template <typename T>
    inline vec3_base<T> ceil(const vec3_base<T>& v)
    {
        return vec3_base<T>(ceil(v.x), ceil(v.y), ceil(v.z));
    }

    template <typename T>
    inline vec3_base<T> trunc(const vec3_base<T>& v)
    {
        return vec3_base<T>(trunc(v.x), trunc(v.y), trunc(v.z));
    }

    template <typename T>
    inline vec3_base<T> fract(const vec3_base<T>& v)
    {
        return vec3_base<T>(fract(v.x), fract(v.y), fract(v.z));
    }

    template <typename T>
    inline vec3_base<T> mod(const vec3_base<T>& v1, const vec3_base<T>& v2)
    {
        return vec3_base<T>(mod(v1.x, v2.x), mod(v1.y, v2.y), mod(v1.z, v2.z));
    }

    template <typename T>
    inline vec3_base<T> mod(const vec3_base<T>& v1, float v2)
    {
        return vec3_base<T>(mod(v1.x, v2), mod(v1.y, v2), mod(v1.z, v2));
    }

    template <typename T>
    inline vec3_base<T> modf(const vec3_base<T>& v, vec3_base<T>& i)
    {
        return vec3_base<T>(modf(v.x, i.x), modf(v.y, i.y), modf(v.z, i.z));
    }

    template <typename T>
    inline vec3_base<T> wrap(const vec3_base<T>& v, float start, float end)
    {
        return start + mod(v - start, end - start);
    }

    template <typename T>
    inline vec3_base<T> min(const vec3_base<T>& v1, const vec3_base<T>& v2)
    {
        return vec3_base<T>(
            min(v1.x, v2.x),
            min(v1.y, v2.y),
            min(v1.z, v2.z)
        );
    }

    template <typename T>
    inline vec3_base<T> max(const vec3_base<T>& v1, const vec3_base<T>& v2)
    {
        return vec3_base<T>(
            max(v1.x, v2.x),
            max(v1.y, v2.y),
            max(v1.z, v2.z)
        );
    }

    template <typename T>
    inline vec3_base<T> clamp(const vec3_base<T>& v, float min, float max)
    {
        return vec3_base<T>(clamp(v.x, min, max), clamp(v.y, min, max), clamp(v.z, min, max));
    }

    template <typename T>
    inline vec3_base<T> clamp01(const vec3_base<T>& v)
    {
        return vec3_base<T>(clamp01(v.x), clamp01(v.y), clamp01(v.z));
    }

    template <typename T>
    inline vec3_base<T> mix(const vec3_base<T>& v1, const vec3_base<T>& v2, float a)
    {
        return v1 + a * (v2 - v1);
    }

    template <typename T>
    inline vec3_base<T> remap(
        const vec3_base<T>& v,
        float a_start,
        float a_end,
        float b_start,
        float b_end
    )
    {
        return b_start + ((b_end - b_start) / (a_end - a_start)) * (v - a_start);
    }

    template <typename T>
    inline vec3_base<T> remap_clamp(
        const vec3_base<T>& v,
        float a_start,
        float a_end,
        float b_start,
        float b_end
    )
    {
        vec3_base<T> t = clamp01((v - a_start) / (a_end - a_start));
        return b_start + t * (b_end - b_start);
    }

    template <typename T>
    inline vec3_base<T> remap01(const vec3_base<T>& v, float a_start, float a_end)
    {
        return clamp01((v - a_start) / (a_end - a_start));
    }

    template <typename T>
    inline vec3_base<T> step(float edge, const vec3_base<T>& v)
    {
        return vec3_base<T>(step(edge, v.x), step(edge, v.y), step(edge, v.z));
    }

    template <typename T>
    inline vec3_base<T> smoothstep(float edge0, float edge1, const vec3_base<T>& v)
    {
        return vec3_base<T>(
            smoothstep(edge0, edge1, v.x),
            smoothstep(edge0, edge1, v.y),
            smoothstep(edge0, edge1, v.z)
        );
    }

    template <typename T>
    inline float length_squared(const vec3_base<T>& v)
    {
        return v.x * v.x + v.y * v.y + v.z * v.z;
    }

    template <typename T>
    inline float length(const vec3_base<T>& v)
    {
        return sqrt(length_squared(v));
    }

    template <typename T>
    inline float distance_squared(const vec3_base<T>& v1, const vec3_base<T>& v2)
    {
        return length_squared(v1 - v2);
    }

    template <typename T>
    inline float distance(const vec3_base<T>& v1, const vec3_base<T>& v2)
    {
        return length(v1 - v2);
    }

    template <typename T>
    inline T dot(const vec3_base<T>& v1, const vec3_base<T>& v2)
    {
        return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
    }

    template <typename T>
    inline vec3_base<T> cross(const vec3_base<T>& v1, const vec3_base<T>& v2)
    {
        return vec3_base<T>(
            (v1.y * v2.z) - (v1.z * v2.y),
            (v1.z * v2.x) - (v1.x * v2.z),
            (v1.x * v2.y) - (v1.y * v2.x)
        );
    }

    template <typename T>
    inline vec3_base<T> normalize(const vec3_base<T>& v)
    {
        return v / length(v);
    }

    template <typename T>
    inline vec3_base<T> faceforward(
        const vec3_base<T>& N,
        const vec3_base<T>& I,
        const vec3_base<T>& Nref
    )
    {
        if (dot(Nref, I) < 0)
            return N;
        return -N;
    }

    template <typename T>
    inline vec3_base<T> reflect(const vec3_base<T>& I, const vec3_base<T>& N)
    {
        return I - 2 * dot(N, I) * N;
    }

    template <typename T>
    inline vec3_base<T> refract(const vec3_base<T>& I, const vec3_base<T>& N, float eta)
    {
        float dp = dot(N, I);
        float k = 1 - eta * eta * (1 - dp * dp);

        if (k < 0)
            return 0;

        return eta * I - (eta * dp + sqrt(k)) * N;
    }

    template <typename T>
    inline T min_component(const vec3_base<T>& v)
    {
        return min(v.x, min(v.y, v.z));
    }

    template <typename T>
    inline T max_component(const vec3_base<T>& v)
    {
        return max(v.x, max(v.y, v.z));
    }

    template <typename T>
    inline int min_component_index(const vec3_base<T>& v)
    {
        return (v.x < v.y)
            ? ((v.x < v.z) ? 0 : 2)
            : ((v.y < v.z) ? 1 : 2);
    }

    template <typename T>
    inline int max_component_index(const vec3_base<T>& v)
    {
        return (v.x > v.y)
            ? ((v.x > v.z) ? 0 : 2)
            : ((v.y > v.z) ? 1 : 2);
    }

    template <typename T>
    inline void coordinate_system(const vec3_base<T>& v1, vec3_base<T>& v2, vec3_base<T>& v3)
    {
        if (abs(v1.x) > abs(v1.y))
        {
            v2 = vec3_base<T>(-v1.z, 0, v1.x) / sqrt(v1.x * v1.x + v1.z * v1.z);
        }
        else
        {
            v2 = vec3_base<T>(0, v1.z, -v1.y) / sqrt(v1.y * v1.y + v1.z * v1.z);
        }
        v3 = cross(v1, v2);
    }

    // Type definitions
    typedef vec3_base<float> vec3;
    typedef vec3_base<int32_t> ivec3;
    typedef vec3_base<uint32_t> uvec3;

}
