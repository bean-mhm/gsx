#pragma once

// STD
#include <vector>
#include <stdexcept>
#include <cstdint>

// Internal
#include "base_structure.h"
#include "../internal_common/all.h"
#include "../internal_math/all.h"
#include "../internal_misc/all.h"

namespace gsx::spatial
{

    // 2D grid structure
    template<typename T>
    class grid_2d_t : public base_structure_2d_t<T>
    {
    public:
        grid_2d_t(math::bounds2 bounds, math::ivec2 resolution)
            : _bounds(bounds),
            _resolution(resolution),
            cell_ratio(math::vec2(resolution) / bounds.diagonal())
        {
            if (math::min_component(resolution) < 1)
                throw std::runtime_error("Grid resolution must be at least 1 in each dimension.");

            containers.resize((usize)resolution.x * (usize)resolution.y);
        }

        math::bounds2 bounds() const
        {
            return _bounds;
        }

        math::ivec2 resolution() const
        {
            return _resolution;
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
            math::ivec2 start_cell(math::floor(cell_ratio * (range.pmin - _bounds.pmin)));
            start_cell.x = math::clamp(start_cell.x, 0, _resolution.x - 1);
            start_cell.y = math::clamp(start_cell.y, 0, _resolution.y - 1);

            math::ivec2 end_cell(math::floor(cell_ratio * (range.pmax - _bounds.pmin)));
            end_cell.x = math::clamp(end_cell.x, 0, _resolution.x - 1);
            end_cell.y = math::clamp(end_cell.y, 0, _resolution.y - 1);

            for (usize y = start_cell.y; y <= end_cell.y; y++)
            {
                for (usize x = start_cell.x; x <= end_cell.x; x++)
                {
                    usize container_index = y * (usize)_resolution.x + x;
                    for (auto& element : containers[container_index])
                    {
                        if (math::inside(misc::remove_ptr(element).pos, range))
                        {
                            out_elements.push_back(misc::add_ptr(element));
                        }
                    }
                }
            }
        }

        virtual void query(
            const math::circle_t& range,
            std::vector<std::remove_pointer_t<T>*>& out_elements
        ) override
        {
            const math::bounds2 range_b = range.bounds();

            math::ivec2 start_cell(math::floor(cell_ratio * (range_b.pmin - _bounds.pmin)));
            start_cell.x = math::clamp(start_cell.x, 0, _resolution.x - 1);
            start_cell.y = math::clamp(start_cell.y, 0, _resolution.y - 1);

            math::ivec2 end_cell(math::floor(cell_ratio * (range_b.pmax - _bounds.pmin)));
            end_cell.x = math::clamp(end_cell.x, 0, _resolution.x - 1);
            end_cell.y = math::clamp(end_cell.y, 0, _resolution.y - 1);

            for (usize y = start_cell.y; y <= end_cell.y; y++)
            {
                for (usize x = start_cell.x; x <= end_cell.x; x++)
                {
                    math::bounds2 cell_bounds(
                        _bounds.pmin + math::vec2(x, y) / cell_ratio,
                        _bounds.pmin + math::vec2(x + 1, y + 1) / cell_ratio
                    );

                    if (!math::overlaps(cell_bounds, range))
                        continue;

                    usize container_index = y * (usize)_resolution.x + x;
                    for (auto& element : containers[container_index])
                    {
                        if (math::inside(misc::remove_ptr(element).pos, range))
                        {
                            out_elements.push_back(misc::add_ptr(element));
                        }
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
                cell_ratio * (misc::remove_ptr(element).pos - _bounds.pmin)
            ));
            cell.x = math::clamp(cell.x, 0, _resolution.x - 1);
            cell.y = math::clamp(cell.y, 0, _resolution.y - 1);

            usize index = (usize)cell.y * (usize)_resolution.x + (usize)cell.x;
            containers[index].push_back(element);

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
        math::bounds2 _bounds;
        math::ivec2 _resolution;
        math::vec2 cell_ratio;
        std::vector<std::vector<T>> containers;

    };

}
