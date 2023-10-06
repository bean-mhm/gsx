#pragma once

// STD
#include <vector>
#include <stack>
#include <memory>
#include <cstdint>

// Internal
#include "../internal_common/all.h"
#include "../internal_math/all.h"
#include "../internal_misc/all.h"

namespace tef::spatial
{

    // Octree data structure with a given capacity per tile
    // Note: T must be copy constructible.
    // Note: T must have a public field of type tef::math::vec3 named pos, representing the 3D
    // position.
    template<typename T, u8 capacity>
        requires (capacity <= 255)
    class octree_t
    {
    public:
        const math::bounds3 bounds;

        octree_t(const math::bounds3& bounds)
            : bounds(bounds)
        {}

        no_default_copy_construct_no_assignment(octree_t);

        usize size() const
        {
            usize count;
            std::stack<octree_t*> stack({ this });
            while (!stack.empty())
            {
                octree_t* t = stack.top();
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

        bool insert(const T& element)
        {
            std::stack<octree_t*> stack({ this });
            while (!stack.empty())
            {
                octree_t* t = stack.top();
                stack.pop();

                if (!math::inside(element.pos, t->bounds))
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

        void query(const math::bounds3& range, std::vector<T*>& out_elements)
        {
            std::stack<octree_t*> stack({ this });
            while (!stack.empty())
            {
                octree_t* t = stack.top();
                stack.pop();

                if (!math::overlaps(t->bounds, range))
                    continue;

                for (u8 i = 0; i < t->elements.size(); i++)
                {
                    if (math::inside(t->elements[i].pos, range))
                    {
                        out_elements.push_back(&t->elements[i]);
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

        void query_all(std::vector<T*>& out_elements)
        {
            std::stack<octree_t*> stack({ this });
            while (!stack.empty())
            {
                octree_t* t = stack.top();
                stack.pop();

                for (u8 i = 0; i < t->elements.size(); i++)
                {
                    out_elements.push_back(&t->elements[i]);
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

        void query_all(std::vector<T>& out_elements)
        {
            std::stack<octree_t*> stack({ this });
            while (!stack.empty())
            {
                octree_t* t = stack.top();
                stack.pop();

                for (u8 i = 0; i < t->elements.size(); i++)
                {
                    out_elements.push_back(t->elements[i]);
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

        void clear()
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

            math::vec3 center = (bounds.pmin + bounds.pmax) * .5f;

            back_bottom_left = std::make_unique<octree_t>(
                math::bounds3(center, bounds.pmin)
            );
            back_bottom_right = std::make_unique<octree_t>(
                math::bounds3(center, math::vec3(bounds.pmax.x, bounds.pmin.y, bounds.pmin.z))
            );
            back_top_left = std::make_unique<octree_t>(
                math::bounds3(center, math::vec3(bounds.pmin.x, bounds.pmax.y, bounds.pmin.z))
            );
            back_top_right = std::make_unique<octree_t>(
                math::bounds3(center, math::vec3(bounds.pmax.x, bounds.pmax.y, bounds.pmin.z))
            );
            front_bottom_left = std::make_unique<octree_t>(
                math::bounds3(center, math::vec3(bounds.pmin.x, bounds.pmin.y, bounds.pmax.z))
            );
            front_bottom_right = std::make_unique<octree_t>(
                math::bounds3(center, math::vec3(bounds.pmax.x, bounds.pmin.y, bounds.pmax.z))
            );
            front_top_left = std::make_unique<octree_t>(
                math::bounds3(center, math::vec3(bounds.pmin.x, bounds.pmax.y, bounds.pmax.z))
            );
            front_top_right = std::make_unique<octree_t>(
                math::bounds3(center, bounds.pmax)
            );
        }

    };

}
