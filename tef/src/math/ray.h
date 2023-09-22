#pragma once

#include "vec3.h"
#include "utils.h"

#include "../str/utils.h"

namespace tef::math
{

    class ray_t
    {
    public:
        vec3 o;
        vec3 d;

        // Constructors
        constexpr ray_t(const vec3& o, const vec3& d)
            : o(o), d(d)
        {}

        // Evaluate point along the ray
        vec3 operator()(float t) const;

        // String
        operator std::string() const;

        // Print
        friend std::ostream& operator<<(std::ostream& os, const ray_t& r);

    };

}
