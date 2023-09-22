#pragma once

#include "vec4.h"
#include "mat4.h"
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

        quaternion_t(const mat4& m);
        quaternion_t(const transform_t& t);

        // String
        operator std::string() const;

        // Print
        friend std::ostream& operator<<(std::ostream& os, const quaternion_t& q);

        // this + quaternion
        quaternion_t operator+(const quaternion_t& q) const;

        // this += quaternion
        quaternion_t& operator+=(const quaternion_t& q);

        // this - quaternion
        quaternion_t operator-(const quaternion_t& q) const;

        // this -= quaternion
        quaternion_t& operator-=(const quaternion_t& q);

        // this * scalar
        quaternion_t operator*(float s) const;

        // this *= scalar
        quaternion_t& operator*=(float s);

        // this / scalar
        quaternion_t operator/(float s) const;

        // this /= scalar
        quaternion_t& operator/=(float s);

        // (-1) * this
        quaternion_t operator-() const;

        // Generate a transform based on this quaternion
        transform_t to_transform() const;

    };

    // scalar * quaternion
    inline quaternion_t operator*(float s, const quaternion_t& q)
    {
        return q * s;
    }

    // Dot product of two quaternions
    inline float dot(const quaternion_t& q1, const quaternion_t& q2)
    {
        return dot(q1.v, q2.v);
    }

    // Normalized copy of a quaternion
    inline quaternion_t normalize(const quaternion_t& q)
    {
        return q / std::sqrt(dot(q, q));
    }

    // Interpolatee between two quaternions using spherical linear interpolation
    quaternion_t slerp(const quaternion_t& q1, const quaternion_t& q2, float t);

}
