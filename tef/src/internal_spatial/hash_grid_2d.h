#pragma once

// STD
#include <vector>
#include <unordered_set>
#include <stdexcept>
#include <cstdint>

// Internal
#include "../internal_math/all.h"
#include "../internal_misc/all.h"

namespace tef::spatial
{

    // 2D hash grid data structure
    // Note: T must have a copy constructor.
    // Note: T must have a field of type tef::math::vec2 named pos, representing the 2D position.
    template<typename T>
    class hash_grid_2d_t
    {
    public:
        const math::vec2 cell_size;

        hash_grid_2d_t(math::vec2 cell_size, size_t n_containers)
            : cell_size(cell_size)
        {
            if (math::min_component(cell_size) <= 0.0f)
                throw std::runtime_error("Grid cell size must be positive.");

            if (n_containers < 1)
                throw std::runtime_error("Number of containers must be at least 1.");

            containers.resize(n_containers);
        }

        no_default_construct(hash_grid_2d_t);

        void insert(const T& element)
        {
            math::ivec2 cell(math::floor(element.pos / cell_size));
            containers[get_container_index(cell)].push_back(element);
        }

        void query(const math::bounds2& range, std::vector<T*>& out_elements)
        {
            std::unordered_set<size_t> indices;

            math::ivec2 start_cell(math::floor(range.pmin / cell_size));
            math::ivec2 end_cell(math::floor(range.pmax / cell_size));

            for (int32_t y = start_cell.y; y <= end_cell.y; y++)
            {
                for (int32_t x = start_cell.x; x <= end_cell.x; x++)
                {
                    size_t container_index = get_container_index(math::ivec2(x, y));
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

        void clear()
        {
            misc::vec_clear(containers);
        }

    private:
        std::vector<std::vector<T>> containers;

        size_t get_container_index(math::ivec2 cell)
        {
            size_t hash = (size_t)math::abs((cell.x * 92837111) ^ (cell.y * 689287499));
            return hash % containers.size();
        }

    };

}
