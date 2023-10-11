#pragma once

// STD
#include <utility>

// Internal
#include "base_container.h"
#include "../internal_common/all.h"
#include "../internal_math/all.h"
#include "../internal_misc/all.h"

namespace tef::spatial
{

    // 2D container with contiguous memory and no spatial optimizations (uses std::vector)
    template<typename T>
    class linear_2d_t : public base_container_2d_t<T>
    {
    public:
        std::vector<T> vec;

        linear_2d_t() = default;

        linear_2d_t(const std::vector<T>& vec)
            : vec(vec)
        {}

        linear_2d_t(std::vector<T>&& vec)
            : vec(std::move(vec))
        {}

        virtual usize size() const override
        {
            return vec.size();
        }

        virtual bool insert(const T& element) override
        {
            vec.push_back(element);
            return true;
        }

        virtual void query(const math::bounds2& range, std::vector<T*>& out_elements) override
        {
            for (auto& element : vec)
            {
                if (math::inside(element.pos, range))
                {
                    out_elements.push_back(&element);
                }
            }
        }

        virtual void query(const math::circle_t& range, std::vector<T*>& out_elements) override
        {
            for (auto& element : vec)
            {
                if (math::inside(element.pos, range))
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
