#include "utils.h"

namespace tef::math
{

    bool solve_quadratic(float a, float b, float c, float& t0, float& t1)
    {
        float discrim = b * b - 4 * a * c;
        if (discrim < 0)
            return false;
        float root_discrim = std::sqrt(discrim);
        float q;
        if (b < 0)
        {
            q = -.5f * (b - root_discrim);
        }
        else
        {
            q = -.5f * (b + root_discrim);
        }
        t0 = q / a;
        t1 = c / q;
        if (t0 > t1)
            std::swap(t0, t1);
        return true;
    }

    bool solve_linear_2x2(const float a[2][2], const float b[2], float& x0, float& x1)
    {
        float det = a[0][0] * a[1][1] - a[0][1] * a[1][0];
        if (std::abs(det) < 1e-10f)
            return false;
        x0 = (a[1][1] * b[0] - a[0][1] * b[1]) / det;
        x1 = (a[0][0] * b[1] - a[1][0] * b[0]) / det;
        if (std::isnan(x0) || std::isnan(x1))
            return false;
        return true;
    }

}
