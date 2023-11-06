#pragma once

// STD
#include <vector>
#include <stack>
#include <memory>
#include <cstdint>

// Internal
#include "base_structure.h"
#include "../internal_common/all.h"
#include "../internal_math/all.h"
#include "../internal_misc/all.h"

namespace gsx::spatial
{

    // Octree with a given capacity per tile
    template<typename T, u8 capacity>
        requires (capacity <= 255)
    class octree_t : public base_structure_3d_t<T>
    {
    public:
        octree_t(const math::bounds3& bounds)
            : _bounds(bounds)
        {}

        octree_t(const octree_t& other)
        {
            _bounds = other._bounds;
            std::vector<std::remove_pointer_t<T>> elements_vec;
            other.query_all(elements_vec);
            for (auto& element : elements_vec)
            {
                insert(misc::add_or_remove_ptr<std::remove_pointer_t<T>, T>(element));
            }
        }

        octree_t& operator=(const octree_t& other)
        {
            clear();
            _bounds = other._bounds;
            std::vector<std::remove_pointer_t<T>> elements_vec;
            other.query_all(elements_vec);
            for (auto& element : elements_vec)
            {
                insert(misc::add_or_remove_ptr<std::remove_pointer_t<T>, T>(element));
            }
        }

        octree_t& operator=(octree_t&& other)
        {
            clear();
            _bounds = other._bounds;
            std::vector<std::remove_pointer_t<T>> elements_vec;
            other.query_all(elements_vec);
            other.clear();
            for (auto& element : elements_vec)
            {
                insert(misc::add_or_remove_ptr<std::remove_pointer_t<T>, T>(element));
            }
        }

        no_default_construct(octree_t);

        math::bounds3 bounds() const
        {
            return _bounds;
        }

        virtual usize size() const override
        {
            usize count = 0;
            std::stack<const octree_t*> stack({ this });
            while (!stack.empty())
            {
                const octree_t* t = stack.top();
                stack.pop();

                count += t->elements.size();

                if (t->divided)
                {
                    stack.push(t->back_bottom_left.get());
                    stack.push(t->back_bottom_right.get());
                    stack.push(t->back_top_left.get());
                    stack.push(t->back_top_right.get());
                    stack.push(t->front_bottom_left.get());
                    stack.push(t->front_bottom_right.get());
                    stack.push(t->front_top_left.get());
                    stack.push(t->front_top_right.get());
                }
            }
        }

        virtual void query(
            const math::bounds3& range,
            std::vector<std::remove_pointer_t<T>*>& out_elements
        ) override
        {
            std::stack<octree_t*> stack({ this });
            while (!stack.empty())
            {
                octree_t* t = stack.top();
                stack.pop();

                if (!math::overlaps(t->_bounds, range))
                    continue;

                for (u8 i = 0; i < t->elements.size(); i++)
                {
                    if (math::inside(misc::remove_ptr(t->elements[i]).pos, range))
                    {
                        out_elements.push_back(misc::add_ptr(t->elements[i]));
                    }
                }

                if (t->divided)
                {
                    stack.push(t->back_bottom_left.get());
                    stack.push(t->back_bottom_right.get());
                    stack.push(t->back_top_left.get());
                    stack.push(t->back_top_right.get());
                    stack.push(t->front_bottom_left.get());
                    stack.push(t->front_bottom_right.get());
                    stack.push(t->front_top_left.get());
                    stack.push(t->front_top_right.get());
                }
            }
        }

        virtual void query(
            const math::sphere_t& range,
            std::vector<std::remove_pointer_t<T>*>& out_elements
        ) override
        {
            std::stack<octree_t*> stack({ this });
            while (!stack.empty())
            {
                octree_t* t = stack.top();
                stack.pop();

                if (!math::overlaps(t->_bounds, range))
                    continue;

                for (u8 i = 0; i < t->elements.size(); i++)
                {
                    if (math::inside(misc::remove_ptr(t->elements[i]).pos, range))
                    {
                        out_elements.push_back(misc::add_ptr(t->elements[i]));
                    }
                }

                if (t->divided)
                {
                    stack.push(t->back_bottom_left.get());
                    stack.push(t->back_bottom_right.get());
                    stack.push(t->back_top_left.get());
                    stack.push(t->back_top_right.get());
                    stack.push(t->front_bottom_left.get());
                    stack.push(t->front_bottom_right.get());
                    stack.push(t->front_top_left.get());
                    stack.push(t->front_top_right.get());
                }
            }
        }

        virtual void query_all(std::vector<std::remove_pointer_t<T>*>& out_elements) override
        {
            std::stack<octree_t*> stack({ this });
            while (!stack.empty())
            {
                octree_t* t = stack.top();
                stack.pop();

                out_elements.reserve(out_elements.size() + t->elements.size());
                for (u8 i = 0; i < t->elements.size(); i++)
                {
                    out_elements.push_back(misc::add_ptr(t->elements[i]));
                }

                if (t->divided)
                {
                    stack.push(t->back_bottom_left.get());
                    stack.push(t->back_bottom_right.get());
                    stack.push(t->back_top_left.get());
                    stack.push(t->back_top_right.get());
                    stack.push(t->front_bottom_left.get());
                    stack.push(t->front_bottom_right.get());
                    stack.push(t->front_top_left.get());
                    stack.push(t->front_top_right.get());
                }
            }
        }

        virtual void query_all(std::vector<std::remove_pointer_t<T>>& out_elements) const override
        {
            std::stack<const octree_t*> stack({ this });
            while (!stack.empty())
            {
                const octree_t* t = stack.top();
                stack.pop();

                out_elements.reserve(out_elements.size() + t->elements.size());
                for (u8 i = 0; i < t->elements.size(); i++)
                {
                    out_elements.push_back(misc::remove_ptr(t->elements[i]));
                }

                if (t->divided)
                {
                    stack.push(t->back_bottom_left.get());
                    stack.push(t->back_bottom_right.get());
                    stack.push(t->back_top_left.get());
                    stack.push(t->back_top_right.get());
                    stack.push(t->front_bottom_left.get());
                    stack.push(t->front_bottom_right.get());
                    stack.push(t->front_top_left.get());
                    stack.push(t->front_top_right.get());
                }
            }
        }

        virtual bool insert(const T& element) override
        {
            std::stack<octree_t*> stack({ this });
            while (!stack.empty())
            {
                octree_t* t = stack.top();
                stack.pop();

                if (!math::inside(misc::remove_ptr(element).pos, t->_bounds))
                    continue;

                if (t->elements.size() < capacity)
                {
                    t->elements.push_back(element);
                    return true;
                }

                t->subdivide();

                stack.push(t->back_bottom_left.get());
                stack.push(t->back_bottom_right.get());
                stack.push(t->back_top_left.get());
                stack.push(t->back_top_right.get());
                stack.push(t->front_bottom_left.get());
                stack.push(t->front_bottom_right.get());
                stack.push(t->front_top_left.get());
                stack.push(t->front_top_right.get());
            }
            return false;
        }

        virtual void clear() override
        {
            elements.clear();

            back_bottom_left = nullptr;
            back_bottom_right = nullptr;
            back_top_left = nullptr;
            back_top_right = nullptr;
            front_bottom_left = nullptr;
            front_bottom_right = nullptr;
            front_top_left = nullptr;
            front_top_right = nullptr;

            divided = false;
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
        misc::fixed_vector_t<T, capacity> elements;

        std::unique_ptr<octree_t> back_bottom_left;
        std::unique_ptr<octree_t> back_bottom_right;
        std::unique_ptr<octree_t> back_top_left;
        std::unique_ptr<octree_t> back_top_right;
        std::unique_ptr<octree_t> front_bottom_left;
        std::unique_ptr<octree_t> front_bottom_right;
        std::unique_ptr<octree_t> front_top_left;
        std::unique_ptr<octree_t> front_top_right;

        bool divided = false;

        void subdivide()
        {
            if (divided)
                return;
            else
                divided = true;

            math::vec3 center = (_bounds.pmin + _bounds.pmax) * .5f;

            back_bottom_left = std::make_unique<octree_t>(
                math::bounds3(center, _bounds.pmin)
            );
            back_bottom_right = std::make_unique<octree_t>(
                math::bounds3(center, math::vec3(_bounds.pmax.x, _bounds.pmin.y, _bounds.pmin.z))
            );
            back_top_left = std::make_unique<octree_t>(
                math::bounds3(center, math::vec3(_bounds.pmin.x, _bounds.pmax.y, _bounds.pmin.z))
            );
            back_top_right = std::make_unique<octree_t>(
                math::bounds3(center, math::vec3(_bounds.pmax.x, _bounds.pmax.y, _bounds.pmin.z))
            );
            front_bottom_left = std::make_unique<octree_t>(
                math::bounds3(center, math::vec3(_bounds.pmin.x, _bounds.pmin.y, _bounds.pmax.z))
            );
            front_bottom_right = std::make_unique<octree_t>(
                math::bounds3(center, math::vec3(_bounds.pmax.x, _bounds.pmin.y, _bounds.pmax.z))
            );
            front_top_left = std::make_unique<octree_t>(
                math::bounds3(center, math::vec3(_bounds.pmin.x, _bounds.pmax.y, _bounds.pmax.z))
            );
            front_top_right = std::make_unique<octree_t>(
                math::bounds3(center, _bounds.pmax)
            );
        }

    };

}
