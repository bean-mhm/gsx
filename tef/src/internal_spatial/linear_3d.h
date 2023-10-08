#pragma once

// Internal
#include "base_container.h"
#include "../internal_common/all.h"
#include "../internal_math/all.h"
#include "../internal_misc/all.h"

namespace tef::spatial
{

    // 3D container with contiguous memory and no spatial optimizations (uses std::vector)
    // Note: T must be copy constructible.
    // Note: T must have a public field of type tef::math::vec3 named pos, representing the 3D
    // position.
    template<typename T>
    class linear_3d_t : public base_container_3d_t<T>
    {
    public:
        std::vector<T> vec;

        linear_3d_t() = default;

        virtual usize size() const override
        {
            return vec.size();
        }

        virtual bool insert(const T& element) override
        {
            vec.push_back(element);
            return true;
        }

        virtual void query(const math::bounds3& range, std::vector<T*>& out_elements) override
        {
            for (auto& element : vec)
            {
                if (math::inside(element.pos, range))
                {
                    out_elements.push_back(&element);
                }
            }
        }

        virtual void query(
            const math::vec3& center,
            f32 radius,
            std::vector<T*>& out_elements
        ) override
        {
            const f32 radius_squared = radius * radius;
            for (auto& element : vec)
            {
                if (math::distance_squared(center, element.pos) <= radius_squared)
                {
                    out_elements.push_back(&element);
                }
            }
        }

        virtual void query_all(std::vector<T*>& out_elements) override
        {
            out_elements.reserve(out_elements.size() + vec.size());
            for (auto& element : vec)
            {
                out_elements.push_back(&element);
            }
        }

        virtual void query_all(std::vector<T>& out_elements) const override
        {
            misc::vec_append(out_elements, vec);
        }

        virtual void clear() override
        {
            vec.clear();
        }

        virtual void rebuild() override
        {}

    private:
    };

}
