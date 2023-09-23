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

        constexpr quaternion_t(const mat4& m)
        {
            float mtrace = m.m[0][0] + m.m[1][1] + m.m[2][2];
            if (mtrace > 0.f)
            {
                // Compute w from matrix trace, then xyz
                // 4w^2 = m[0][0] + m[1][1] + m[2][2] + m[3][3] (but m[3][3] == 1)
                float s = sqrt(mtrace + 1.f);
                v.w = s / 2.f;
                s = 0.5f / s;
                v.x = (m.m[2][1] - m.m[1][2]) * s;
                v.y = (m.m[0][2] - m.m[2][0]) * s;
                v.z = (m.m[1][0] - m.m[0][1]) * s;
            }
            else
            {
                // Compute largest of $x$, $y$, or $z$, then remaining components
                const int nxt[3] = { 1, 2, 0 };
                float q[3];
                int i = 0;
                if (m.m[1][1] > m.m[0][0]) i = 1;
                if (m.m[2][2] > m.m[i][i]) i = 2;
                int j = nxt[i];
                int k = nxt[j];
                float s = sqrt((m.m[i][i] - (m.m[j][j] + m.m[k][k])) + 1.f);
                q[i] = s * 0.5f;
                if (s != 0.f) s = 0.5f / s;
                v.w = (m.m[k][j] - m.m[j][k]) * s;
                q[j] = (m.m[j][i] + m.m[i][j]) * s;
                q[k] = (m.m[k][i] + m.m[i][k]) * s;
                v.x = q[0];
                v.y = q[1];
                v.z = q[2];
            }
        }

        constexpr quaternion_t(const transform_t& t)
            : quaternion_t(t.m)
        {}

        // String
        constexpr std::string to_string() const
        {
            return v.to_string();
        }

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

        // Generate a transform based on this quaternion
        constexpr transform_t to_transform() const
        {
            float xx = v.x * v.x, yy = v.y * v.y, zz = v.z * v.z;
            float xy = v.x * v.y, xz = v.x * v.z, yz = v.y * v.z;
            float wx = v.x * v.w, wy = v.y * v.w, wz = v.z * v.w;

            mat4 m;
            m.m[0][0] = 1 - 2 * (yy + zz);
            m.m[0][1] = 2 * (xy + wz);
            m.m[0][2] = 2 * (xz - wy);
            m.m[1][0] = 2 * (xy - wz);
            m.m[1][1] = 1 - 2 * (xx + zz);
            m.m[1][2] = 2 * (yz + wx);
            m.m[2][0] = 2 * (xz + wy);
            m.m[2][1] = 2 * (yz - wx);
            m.m[2][2] = 1 - 2 * (xx + yy);

            // Transpose since we are left-handed
            return transform_t(transpose(m), m);
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
        return q / std::sqrt(dot(q, q));
    }

    // Interpolatee between two quaternions using spherical linear interpolation
    constexpr quaternion_t slerp(const quaternion_t& q1, const quaternion_t& q2, float t)
    {
        float cos_theta = dot(q1, q2);
        if (cos_theta > .9995f)
        {
            return normalize((1 - t) * q1 + t * q2);
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
