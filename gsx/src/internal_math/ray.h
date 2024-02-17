#pragma once

#include "vec3.h"
#include "utils.h"
#include "../internal_common/all.h"
#include "../internal_str/all.h"

namespace gsx::math
{

    class ray_t
    {
    public:
        vec3 o;
        vec3 d;

        constexpr ray_t(const vec3& o, const vec3& d)
            : o(o), d(d)
        {}

        // evaluate point along the ray
        constexpr vec3 operator()(f32 t) const
        {
            return o + d * t;
        }

        std::string to_string() const;

        friend std::ostream& operator<<(std::ostream& os, const ray_t& r);

        constexpr bool operator==(const ray_t& r) const
        {
            return o == r.o && d == r.d;
        }

        constexpr bool operator!=(const ray_t& r) const
        {
            return o != r.o || d != r.d;
        }

    };

}
