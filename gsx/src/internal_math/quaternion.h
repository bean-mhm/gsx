#pragma once

#include "vec4.h"
#include "matrix.h"
#include "transform.h"
#include "utils.h"
#include "../internal_common/all.h"
#include "../internal_str/all.h"

namespace gsx::math
{

    class quaternion_t
    {
    public:
        vec4 v;

        constexpr quaternion_t()
            : v(0, 0, 0, 1)
        {}

        constexpr quaternion_t(const vec4& v)
            : v(v)
        {}

        quaternion_t(const mat3& m);
        quaternion_t(const mat4& m);

        std::string to_string() const;

        friend std::ostream& operator<<(
            std::ostream& os,
            const quaternion_t& q
            );

        constexpr quaternion_t operator+(const quaternion_t& q) const
        {
            return quaternion_t(v + q.v);
        }

        constexpr quaternion_t& operator+=(const quaternion_t& q)
        {
            v += q.v;
            return *this;
        }

        constexpr quaternion_t operator-(const quaternion_t& q) const
        {
            return quaternion_t(v - q.v);
        }

        constexpr quaternion_t& operator-=(const quaternion_t& q)
        {
            v -= q.v;
            return *this;
        }

        constexpr quaternion_t operator*(f32 s) const
        {
            return quaternion_t(v * s);
        }

        constexpr quaternion_t& operator*=(f32 s)
        {
            v *= s;
            return *this;
        }

        constexpr quaternion_t operator/(f32 s) const
        {
            return quaternion_t(v / s);
        }

        constexpr quaternion_t& operator/=(f32 s)
        {
            v /= s;
            return *this;
        }

        constexpr quaternion_t operator-() const
        {
            return quaternion_t(-v);
        }

        // generate a 3D homogeneous transformation matrix based on this
        // quaternion (left-handed)
        constexpr mat4 to_transform() const
        {
            f32 xx = v.x * v.x, yy = v.y * v.y, zz = v.z * v.z;
            f32 xy = v.x * v.y, xz = v.x * v.z, yz = v.y * v.z;
            f32 wx = v.x * v.w, wy = v.y * v.w, wz = v.z * v.w;

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

            // transpose since we are left-handed
            return transpose(r);
        }

        constexpr bool operator==(const quaternion_t& q) const
        {
            return v == q.v;
        }

        constexpr bool operator!=(const quaternion_t& q) const
        {
            return v != q.v;
        }

    };

    constexpr quaternion_t operator*(f32 s, const quaternion_t& q)
    {
        return q * s;
    }

    constexpr f32 dot(const quaternion_t& q1, const quaternion_t& q2)
    {
        return dot(q1.v, q2.v);
    }

    constexpr quaternion_t normalize(const quaternion_t& q)
    {
        return quaternion_t(normalize(q.v));
    }

    // interpolate between two quaternions using spherical linear interpolation
    constexpr quaternion_t slerp(
        const quaternion_t& q1,
        const quaternion_t& q2,
        f32 t
    )
    {
        f32 cos_theta = dot(q1, q2);
        if (cos_theta > .9995f)
        {
            return normalize(q1 + t * (q2 - q1));
        }
        else
        {
            f32 theta = std::acos(std::clamp(cos_theta, -1.f, 1.f));
            f32 thetap = theta * t;
            quaternion_t qperp = normalize(q2 - q1 * cos_theta);
            return q1 * std::cos(thetap) + qperp * std::sin(thetap);
        }
    }

}
