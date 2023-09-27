#pragma once

// Internal
#include "vec4.h"
#include "matrix.h"
#include "transform.h"
#include "utils.h"
#include "../str/utils.h"

namespace tef::math
{

    class quaternion_t
    {
    public:
        vec4 v;

        // Constructors

        constexpr quaternion_t()
            : v(0, 0, 0, 1)
        {}

        constexpr quaternion_t(const vec4& v)
            : v(v)
        {}

        quaternion_t(const mat3& m);
        quaternion_t(const mat4& m);

        // String
        std::string to_string() const;

        // Print
        friend std::ostream& operator<<(std::ostream& os, const quaternion_t& q);

        // this + quaternion
        constexpr quaternion_t operator+(const quaternion_t& q) const
        {
            return quaternion_t(v + q.v);
        }

        // this += quaternion
        constexpr quaternion_t& operator+=(const quaternion_t& q)
        {
            v += q.v;
            return *this;
        }

        // this - quaternion
        constexpr quaternion_t operator-(const quaternion_t& q) const
        {
            return quaternion_t(v - q.v);
        }

        // this -= quaternion
        constexpr quaternion_t& operator-=(const quaternion_t& q)
        {
            v -= q.v;
            return *this;
        }

        // this * scalar
        constexpr quaternion_t operator*(float s) const
        {
            return quaternion_t(v * s);
        }

        // this *= scalar
        constexpr quaternion_t& operator*=(float s)
        {
            v *= s;
            return *this;
        }

        // this / scalar
        constexpr quaternion_t operator/(float s) const
        {
            return quaternion_t(v / s);
        }

        // this /= scalar
        constexpr quaternion_t& operator/=(float s)
        {
            v /= s;
            return *this;
        }

        // (-1) * this
        constexpr quaternion_t operator-() const
        {
            return quaternion_t(-v);
        }

        // Generate a 3D homogeneous transformation matrix based on this quaternion (left-handed)
        constexpr mat4 to_transform() const
        {
            float xx = v.x * v.x, yy = v.y * v.y, zz = v.z * v.z;
            float xy = v.x * v.y, xz = v.x * v.z, yz = v.y * v.z;
            float wx = v.x * v.w, wy = v.y * v.w, wz = v.z * v.w;

            mat4 r;
            r(0, 0) = 1 - 2 * (yy + zz);
            r(0, 1) = 2 * (xy + wz);
            r(0, 2) = 2 * (xz - wy);
            r(1, 0) = 2 * (xy - wz);
            r(1, 1) = 1 - 2 * (xx + zz);
            r(1, 2) = 2 * (yz + wx);
            r(2, 0) = 2 * (xz + wy);
            r(2, 1) = 2 * (yz - wx);
            r(2, 2) = 1 - 2 * (xx + yy);

            // Transpose since we are left-handed
            return transpose(r);
        }

        // this == quaternion
        constexpr bool operator==(const quaternion_t& q) const
        {
            return v == q.v;
        }

        // this != quaternion
        constexpr bool operator!=(const quaternion_t& q) const
        {
            return v != q.v;
        }

    };

    // scalar * quaternion
    constexpr quaternion_t operator*(float s, const quaternion_t& q)
    {
        return q * s;
    }

    // Dot product of two quaternions
    constexpr float dot(const quaternion_t& q1, const quaternion_t& q2)
    {
        return dot(q1.v, q2.v);
    }

    // Normalized copy of a quaternion
    constexpr quaternion_t normalize(const quaternion_t& q)
    {
        return quaternion_t(normalize(q.v));
    }

    // Interpolatee between two quaternions using spherical linear interpolation
    constexpr quaternion_t slerp(const quaternion_t& q1, const quaternion_t& q2, float t)
    {
        float cos_theta = dot(q1, q2);
        if (cos_theta > .9995f)
        {
            return normalize(q1 + t * (q2 - q1));
        }
        else
        {
            float  theta = std::acos(std::clamp(cos_theta, -1.f, 1.f));
            float  thetap = theta * t;
            quaternion_t qperp = normalize(q2 - q1 * cos_theta);
            return q1 * std::cos(thetap) + qperp * std::sin(thetap);
        }
    }

}
