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

    // 3D grid container
    template<typename T>
    class grid_3d_t : public base_structure_3d_t<T>
    {
    public:
        grid_3d_t(math::bounds3 bounds, math::ivec3 resolution)
            : _bounds(bounds),
            _resolution(resolution),
            cell_ratio(math::vec3(resolution) / bounds.diagonal())
        {
            if (math::min_component(resolution) < 1)
                throw std::runtime_error("Grid resolution must be at least 1 in each dimension.");

            containers.resize((usize)resolution.x * (usize)resolution.y);
        }

        no_default_construct(grid_3d_t);

        math::bounds3 bounds() const
        {
            return _bounds;
        }

        math::ivec3 resolution() const
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

        virtual void query(const math::bounds3& range, std::vector<T*>& out_elements) override
        {
            math::ivec3 start_cell(math::floor(cell_ratio * (range.pmin - _bounds.pmin)));
            start_cell.x = math::clamp(start_cell.x, 0, _resolution.x - 1);
            start_cell.y = math::clamp(start_cell.y, 0, _resolution.y - 1);
            start_cell.z = math::clamp(start_cell.z, 0, _resolution.z - 1);

            math::ivec3 end_cell(math::floor(cell_ratio * (range.pmax - _bounds.pmin)));
            end_cell.x = math::clamp(end_cell.x, 0, _resolution.x - 1);
            end_cell.y = math::clamp(end_cell.y, 0, _resolution.y - 1);
            end_cell.z = math::clamp(end_cell.z, 0, _resolution.z - 1);

            for (usize z = start_cell.z; z <= end_cell.z; z++)
            {
                for (usize y = start_cell.y; y <= end_cell.y; y++)
                {
                    for (usize x = start_cell.x; x <= end_cell.x; x++)
                    {
                        usize container_index =
                            z * (usize)_resolution.x * (usize)_resolution.y
                            + y * (usize)_resolution.x
                            + x;

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
        }

        virtual void query(const math::sphere_t& range, std::vector<T*>& out_elements) override
        {
            const math::bounds3 range_b = range.bounds();

            math::ivec3 start_cell(math::floor(cell_ratio * (range_b.pmin - _bounds.pmin)));
            start_cell.x = math::clamp(start_cell.x, 0, _resolution.x - 1);
            start_cell.y = math::clamp(start_cell.y, 0, _resolution.y - 1);
            start_cell.z = math::clamp(start_cell.z, 0, _resolution.z - 1);

            math::ivec3 end_cell(math::floor(cell_ratio * (range_b.pmax - _bounds.pmin)));
            end_cell.x = math::clamp(end_cell.x, 0, _resolution.x - 1);
            end_cell.y = math::clamp(end_cell.y, 0, _resolution.y - 1);
            end_cell.z = math::clamp(end_cell.z, 0, _resolution.z - 1);

            for (usize z = start_cell.z; z <= end_cell.z; z++)
            {
                for (usize y = start_cell.y; y <= end_cell.y; y++)
                {
                    for (usize x = start_cell.x; x <= end_cell.x; x++)
                    {
                        math::bounds3 cell_bounds(
                            _bounds.pmin + math::vec3(x, y, z) / cell_ratio,
                            _bounds.pmin + math::vec3(x + 1, y + 1, z + 1) / cell_ratio
                        );

                        if (!math::overlaps(cell_bounds, range))
                            continue;

                        usize container_index =
                            z * (usize)_resolution.x * (usize)_resolution.y
                            + y * (usize)_resolution.x
                            + x;

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
        }

        virtual void query_all(std::vector<T*>& out_elements) override
        {
            for (auto& container : containers)
            {
                out_elements.reserve(out_elements.size() + container.size());
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
                misc::vec_append(out_elements, container);
            }
        }

        virtual bool insert(const T& element) override
        {
            math::ivec3 cell(math::floor(cell_ratio * (element.pos - _bounds.pmin)));
            cell.x = math::clamp(cell.x, 0, _resolution.x - 1);
            cell.y = math::clamp(cell.y, 0, _resolution.y - 1);
            cell.z = math::clamp(cell.z, 0, _resolution.z - 1);

            usize index =
                (usize)cell.z * (usize)_resolution.x * (usize)_resolution.y
                + (usize)cell.y * (usize)_resolution.x
                + (usize)cell.x;

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
        math::bounds3 _bounds;
        math::ivec3 _resolution;
        math::vec3 cell_ratio;
        std::vector<std::vector<T>> containers;

    };

}
