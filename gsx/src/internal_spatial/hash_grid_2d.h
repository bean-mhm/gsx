#pragma once

// STD
#include <vector>
#include <unordered_set>
#include <stdexcept>
#include <cstdint>

// Internal
#include "base_structure.h"
#include "../internal_common/all.h"
#include "../internal_math/all.h"
#include "../internal_misc/all.h"

namespace gsx::spatial
{

    // 2D hash grid structure
    template<typename T>
    class hash_grid_2d_t : public base_structure_2d_t<T>
    {
    public:
        hash_grid_2d_t(math::vec2 cell_size, u32 n_containers)
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

        virtual void query(
            const math::bounds2& range,
            std::vector<std::remove_pointer_t<T>*>& out_elements
        ) override
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

            for (usize index : indices)
            {
                for (auto& element : containers[index])
                {
                    if (math::inside(misc::remove_ptr(element).pos, range))
                    {
                        out_elements.push_back(misc::add_ptr(element));
                    }
                }
            }
        }

        virtual void query(
            const math::circle_t& range,
            std::vector<std::remove_pointer_t<T>*>& out_elements
        ) override
        {
            std::unordered_set<usize> indices;

            const math::bounds2 range_b = range.bounds();
            math::ivec2 start_cell(math::floor(range_b.pmin / _cell_size));
            math::ivec2 end_cell(math::floor(range_b.pmax / _cell_size));

            for (i32 y = start_cell.y; y <= end_cell.y; y++)
            {
                for (i32 x = start_cell.x; x <= end_cell.x; x++)
                {
                    math::bounds2 cell_bounds(
                        math::vec2(x, y) * _cell_size,
                        math::vec2(x + 1, y + 1) * _cell_size
                    );

                    if (!math::overlaps(cell_bounds, range))
                        continue;

                    usize container_index = get_container_index(math::ivec2(x, y));
                    indices.insert(container_index);
                }
            }

            for (usize index : indices)
            {
                for (auto& element : containers[index])
                {
                    if (math::inside(misc::remove_ptr(element).pos, range))
                    {
                        out_elements.push_back(misc::add_ptr(element));
                    }
                }
            }
        }

        virtual void query_all(std::vector<std::remove_pointer_t<T>*>& out_elements) override
        {
            for (auto& container : containers)
            {
                out_elements.reserve(out_elements.size() + container.size());
                for (auto& element : container)
                {
                    out_elements.push_back(misc::add_ptr(element));
                }
            }
        }

        virtual void query_all(std::vector<std::remove_pointer_t<T>>& out_elements) const override
        {
            for (auto& container : containers)
            {
                out_elements.reserve(out_elements.size() + container.size());
                for (auto& element : container)
                {
                    out_elements.push_back(misc::remove_ptr(element));
                }
            }
        }

        virtual bool insert(const T& element) override
        {
            math::ivec2 cell(math::floor(
                misc::remove_ptr(element).pos / _cell_size
            ));
            containers[get_container_index(cell)].push_back(element);
            return true;
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
