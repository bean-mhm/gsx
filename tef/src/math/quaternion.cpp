#include "quaternion.h"

namespace tef::math
{

    quaternion_t::quaternion_t(const mat3& m)
    {
        float mtrace = m(0, 0) + m(1, 1) + m(2, 2);
        if (mtrace > 0.f)
        {
            // Compute w from matrix trace, then xyz
            // 4w^2 = m(0, 0) + m(1, 1) + m(2, 2) + m(3, 3) (but m(3, 3) == 1)
            float s = sqrt(mtrace + 1.f);
            v.w = s / 2.f;
            s = 0.5f / s;
            v.x = (m(2, 1) - m(1, 2)) * s;
            v.y = (m(0, 2) - m(2, 0)) * s;
            v.z = (m(1, 0) - m(0, 1)) * s;
        }
        else
        {
            // Compute largest of $x$, $y$, or $z$, then remaining components
            const int nxt[3] = { 1, 2, 0 };
            float q[3];
            int i = 0;
            if (m(1, 1) > m(0, 0)) i = 1;
            if (m(2, 2) > m(i, i)) i = 2;
            int j = nxt[i];
            int k = nxt[j];
            float s = sqrt((m(i, i) - (m(j, j) + m(k, k))) + 1.f);
            q[i] = s * 0.5f;
            if (s != 0.f) s = 0.5f / s;
            v.w = (m(k, j) - m(j, k)) * s;
            q[j] = (m(j, i) + m(i, j)) * s;
            q[k] = (m(k, i) + m(i, k)) * s;
            v.x = q[0];
            v.y = q[1];
            v.z = q[2];
        }
    }

    quaternion_t::quaternion_t(const mat4& m)
        : quaternion_t(m.sub<3>())
    {}

    std::string quaternion_t::to_string() const
    {
        return v.to_string();
    }

    std::ostream& operator<<(std::ostream& os, const quaternion_t& q)
    {
        os << q.to_string();
        return os;
    }

}
