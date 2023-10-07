#pragma once

// STD
#include <vector>
#include <unordered_set>
#include <stdexcept>
#include <cstdint>

// Internal
#include "base_container.h"
#include "../internal_common/all.h"
#include "../internal_math/all.h"
#include "../internal_misc/all.h"

namespace tef::spatial
{

    // 2D hash grid container
    // Note: T must be copy constructible.
    // Note: T must have a public field of type tef::math::vec2 named pos, representing the 2D
    // position.
    template<typename T>
    class hash_grid_2d_t : public base_container_t<T>
    {
    public:
        hash_grid_2d_t(math::vec2 cell_size, usize n_containers)
            : _cell_size(cell_size)
        {
            if (math::min_component(cell_size) <= 0.0f)
                throw std::runtime_error("Grid cell size must be positive.");

            if (n_containers < 1)
                throw std::runtime_error("Number of containers must be at least 1.");

            containers.resize(n_containers);
        }

        no_default_construct(hash_grid_2d_t);

        math::vec2 cell_size() const
        {
            return _cell_size;
        }

        virtual usize size() const override
        {
            usize count = 0;
            for (auto& container : containers)
            {
                count += container.size();
            }
            return count;
        }

        virtual bool insert(const T& element) override
        {
            math::ivec2 cell(math::floor(element.pos / _cell_size));
            containers[get_container_index(cell)].push_back(element);
            return true;
        }

        void query(const math::bounds2& range, std::vector<T*>& out_elements)
        {
            std::unordered_set<usize> indices;

            math::ivec2 start_cell(math::floor(range.pmin / _cell_size));
            math::ivec2 end_cell(math::floor(range.pmax / _cell_size));

            for (i32 y = start_cell.y; y <= end_cell.y; y++)
            {
                for (i32 x = start_cell.x; x <= end_cell.x; x++)
                {
                    usize container_index = get_container_index(math::ivec2(x, y));
                    indices.insert(container_index);
                }
            }

            for (int index : indices)
            {
                for (auto& element : containers[index])
                {
                    if (math::inside(element.pos, range))
                    {
                        out_elements.push_back(&element);
                    }
                }
            }
        }

        virtual void query_all(std::vector<T*>& out_elements) override
        {
            for (auto& container : containers)
            {
                for (auto& element : container)
                {
                    out_elements.push_back(&element);
                }
            }
        }

        virtual void query_all(std::vector<T>& out_elements) const override
        {
            for (auto& container : containers)
            {
                for (auto& element : container)
                {
                    out_elements.push_back(element);
                }
            }
        }

        virtual void clear() override
        {
            for (auto& container : containers)
            {
                misc::vec_clear(container);
            }
        }

        virtual void rebuild() override
        {
            std::vector<T> elements_vec;
            query_all(elements_vec);
            clear();
            for (auto& element : elements_vec)
            {
                insert(element);
            }
        }

    private:
        math::vec2 _cell_size;
        std::vector<std::vector<T>> containers;

        usize get_container_index(math::ivec2 cell)
        {
            usize hash = (usize)math::abs((cell.x * 92837111) ^ (cell.y * 689287499));
            return hash % containers.size();
        }

    };

}
