#pragma once

#include "vec4.h"
#include "matrix.h"
#include "transform.h"
#include "utils.h"
#include "../internal_common/all.h"
#include "../internal_str/all.h"

namespace gsx::math
{

    template<std::floating_point T>
    class base_quaternion_t
    {
    public:
        base_vec4<T> v;

        constexpr base_quaternion_t()
            : v(0, 0, 0, 1)
        {}

        constexpr base_quaternion_t(const base_vec4<T>& v)
            : v(v)
        {}

        template<std::floating_point U>
        constexpr operator base_quaternion_t<U>() const
        {
            return base_quaternion_t<U>((base_vec4<U>)v);
        }

        base_quaternion_t(const base_mat<T, 3, 3>& m)
        {
            T mtrace = m(0, 0) + m(1, 1) + m(2, 2);
            if (mtrace > 0)
            {
                // compute w from matrix trace, then xyz
                // 4w^2 = m(0, 0) + m(1, 1) + m(2, 2) + m(3, 3)
                // (but m(3, 3) == 1)
                T s = sqrt(mtrace + (T)1.);
                v.w = s * (T).5;
                s = (T).5 / s;
                v.x = (m(2, 1) - m(1, 2)) * s;
                v.y = (m(0, 2) - m(2, 0)) * s;
                v.z = (m(1, 0) - m(0, 1)) * s;
            }
            else
            {
                // compute largest of $x$, $y$, or $z$, then remaining
                // components
                const i32 nxt[3] = { 1, 2, 0 };
                T q[3];
                i32 i = 0;
                if (m(1, 1) > m(0, 0))
                {
                    i = 1;
                }
                if (m(2, 2) > m(i, i))
                {
                    i = 2;
                }
                i32 j = nxt[i];
                i32 k = nxt[j];
                T s = sqrt((m(i, i) - (m(j, j) + m(k, k))) + (T)1.);
                q[i] = s * (T).5;
                if (s != (T)0.)
                {
                    s = (T).5 / s;
                }
                v.w = (m(k, j) - m(j, k)) * s;
                q[j] = (m(j, i) + m(i, j)) * s;
                q[k] = (m(k, i) + m(i, k)) * s;
                v.x = q[0];
                v.y = q[1];
                v.z = q[2];
            }
        }

        base_quaternion_t(const base_mat<T, 4, 4>& m)
            : base_quaternion_t(m.sub<3>())
        {}

        std::string to_string() const
        {
            return v.to_string();
        }

        friend std::ostream& operator<<(
            std::ostream& os,
            const base_quaternion_t& q
            )
        {
            os << q.to_string();
            return os;
        }

        constexpr base_quaternion_t operator+(const base_quaternion_t& q) const
        {
            return base_quaternion_t(v + q.v);
        }

        constexpr base_quaternion_t& operator+=(const base_quaternion_t& q)
        {
            v += q.v;
            return *this;
        }

        constexpr base_quaternion_t operator-(const base_quaternion_t& q) const
        {
            return base_quaternion_t(v - q.v);
        }

        constexpr base_quaternion_t& operator-=(const base_quaternion_t& q)
        {
            v -= q.v;
            return *this;
        }

        constexpr base_quaternion_t operator*(T s) const
        {
            return base_quaternion_t(v * s);
        }

        constexpr base_quaternion_t& operator*=(T s)
        {
            v *= s;
            return *this;
        }

        constexpr base_quaternion_t operator/(T s) const
        {
            return base_quaternion_t(v / s);
        }

        constexpr base_quaternion_t& operator/=(T s)
        {
            v /= s;
            return *this;
        }

        constexpr base_quaternion_t operator-() const
        {
            return base_quaternion_t(-v);
        }

        // generate a 3D homogeneous transformation matrix based on this
        // quaternion (left-handed)
        constexpr base_mat<T, 4, 4> to_transform() const
        {
            T xx = v.x * v.x, yy = v.y * v.y, zz = v.z * v.z;
            T xy = v.x * v.y, xz = v.x * v.z, yz = v.y * v.z;
            T wx = v.x * v.w, wy = v.y * v.w, wz = v.z * v.w;

            base_mat<T, 4, 4> r;
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

        constexpr bool operator==(const base_quaternion_t& q) const
        {
            return v == q.v;
        }

        constexpr bool operator!=(const base_quaternion_t& q) const
        {
            return v != q.v;
        }

    };

    template<std::floating_point T>
    constexpr base_quaternion_t<T> operator*(T s, const base_quaternion_t<T>& q)
    {
        return q * s;
    }

    template<std::floating_point T>
    constexpr T dot(
        const base_quaternion_t<T>& q1,
        const base_quaternion_t<T>& q2
    )
    {
        return dot(q1.v, q2.v);
    }

    template<std::floating_point T>
    constexpr base_quaternion_t<T> normalize(const base_quaternion_t<T>& q)
    {
        return base_quaternion_t<T>(normalize(q.v));
    }

    // interpolate between two quaternions using spherical linear interpolation
    template<std::floating_point T>
    constexpr base_quaternion_t<T> slerp(
        const base_quaternion_t<T>& q1,
        const base_quaternion_t<T>& q2,
        T t
    )
    {
        T cos_theta = dot(q1, q2);
        if (cos_theta > (T).9995)
        {
            return normalize(q1 + t * (q2 - q1));
        }
        else
        {
            T theta = std::acos(std::clamp(cos_theta, (T)(-1.), (T)1.));
            T thetap = theta * t;
            base_quaternion_t qperp = normalize(q2 - q1 * cos_theta);
            return q1 * std::cos(thetap) + qperp * std::sin(thetap);
        }
    }

    using quaternion_t = base_quaternion_t<f32>;
    using dquaternion_t = base_quaternion_t<f64>;

}
