#pragma once

// STD
#include <vector>
#include <stdexcept>
#include <cstdint>

// Internal
#include "../internal_common/all.h"
#include "../internal_math/all.h"
#include "../internal_misc/all.h"

namespace tef::spatial
{

    // 2D grid data structure
    // Note: T must be copy constructible.
    // Note: T must have a public field of type tef::math::vec2 named pos, representing the 2D
    // position.
    template<typename T>
    class grid_2d_t
    {
    public:
        const math::bounds2 bounds;
        const math::ivec2 resolution;

        grid_2d_t(math::bounds2 bounds, math::ivec2 resolution)
            : bounds(bounds),
            resolution(resolution),
            cell_ratio(math::vec2(resolution) / bounds.diagonal())
        {
            if (math::min_component(resolution) < 1)
                throw std::runtime_error("Grid resolution must be at least 1 in each dimension.");

            containers.resize((usize)resolution.x * (usize)resolution.y);
        }

        no_default_construct(grid_2d_t);

        usize size() const
        {
            usize count = 0;
            for (auto& container : containers)
            {
                count += container.size();
            }
            return count;
        }

        void insert(const T& element)
        {
            math::ivec2 cell(math::floor(cell_ratio * (element.pos - bounds.pmin)));
            cell.x = math::clamp(cell.x, 0, resolution.x - 1);
            cell.y = math::clamp(cell.y, 0, resolution.y - 1);

            usize index = (usize)cell.y * (usize)resolution.x + (usize)cell.x;
            containers[index].push_back(element);
        }

        void query(const math::bounds2& range, std::vector<T*>& out_elements)
        {
            math::ivec2 start_cell(math::floor(cell_ratio * (range.pmin - bounds.pmin)));
            start_cell.x = math::clamp(start_cell.x, 0, resolution.x - 1);
            start_cell.y = math::clamp(start_cell.y, 0, resolution.y - 1);

            math::ivec2 end_cell(math::floor(cell_ratio * (range.pmax - bounds.pmin)));
            end_cell.x = math::clamp(end_cell.x, 0, resolution.x - 1);
            end_cell.y = math::clamp(end_cell.y, 0, resolution.y - 1);

            for (usize y = start_cell.y; y <= end_cell.y; y++)
            {
                for (usize x = start_cell.x; x <= end_cell.x; x++)
                {
                    usize container_index = y * (usize)resolution.x + x;
                    for (auto& element : containers[container_index])
                    {
                        if (math::inside(element.pos, range))
                        {
                            out_elements.push_back(&element);
                        }
                    }
                }
            }
        }

        void query_all(std::vector<T*>& out_elements)
        {
            for (auto& container : containers)
            {
                for (auto& element : container)
                {
                    out_elements.push_back(&element);
                }
            }
        }

        void query_all(std::vector<T>& out_elements)
        {
            for (auto& container : containers)
            {
                for (auto& element : container)
                {
                    out_elements.push_back(element);
                }
            }
        }

        void clear()
        {
            for (auto& container : containers)
            {
                misc::vec_clear(container);
            }
        }

        void rebuild()
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
        const math::vec2 cell_ratio;
        std::vector<std::vector<T>> containers;

    };

}
