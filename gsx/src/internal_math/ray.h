#pragma once

#include "vec3.h"
#include "utils.h"
#include "../internal_common/all.h"
#include "../internal_str/all.h"

namespace gsx::math
{

    template<std::floating_point T>
    class base_ray_t
    {
    public:
        base_vec3<T> o;
        base_vec3<T> d;

        constexpr base_ray_t(const base_vec3<T>& o, const base_vec3<T>& d)
            : o(o), d(d)
        {}

        template<std::floating_point U>
        constexpr operator base_ray_t<U>() const
        {
            return base_ray_t<U>((base_vec3<U>)o, (base_vec3<U>)d);
        }

        // evaluate point along the ray
        constexpr base_vec3<T> operator()(T t) const
        {
            return o + d * t;
        }

        std::string to_string() const
        {
            return std::format(
                "[o={}, d={}]",
                o.to_string(),
                d.to_string()
            );
        }

        friend std::ostream& operator<<(std::ostream& os, const base_ray_t& r)
        {
            os << r.to_string();
            return os;
        }

        constexpr bool operator==(const base_ray_t& r) const
        {
            return o == r.o && d == r.d;
        }

        constexpr bool operator!=(const base_ray_t& r) const
        {
            return o != r.o || d != r.d;
        }

    };

    using ray_t = base_ray_t<f32>;
    using dray_t = base_ray_t<f64>;

}
