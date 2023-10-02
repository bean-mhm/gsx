#pragma once

// STD
#include <vector>
#include <stack>
#include <memory>
#include <cstdint>

// Internal
#include "../math.h"
#include "../misc.h"

namespace tef::spatial
{

    // Quadtree data structure with a given capacity per tile
    // Note: T must have a constructor with no arguments, and an assignment operator.
    // Note: T must have a field of type tef::math::vec2 named pos, representing the 2D position.
    template<typename T, size_t capacity>
    class quadtree_t
    {
    public:
        const math::bounds2 bounds;

        quadtree_t(const math::bounds2& bounds)
            : bounds(bounds)
        {}

        no_default_copy_construct_no_assignment(quadtree_t);

        bool insert(const T& element)
        {
            std::stack<quadtree_t*> stack({ this });
            while (!stack.empty())
            {
                quadtree_t* q = stack.top();
                stack.pop();

                if (!math::inside(element.pos, q->bounds))
                    continue;

                if (q->size < capacity)
                {
                    q->elements[q->size] = element;
                    q->size++;
                    return true;
                }

                q->subdivide();

                stack.push(q->top_left.get());
                stack.push(q->top_right.get());
                stack.push(q->bottom_right.get());
                stack.push(q->bottom_left.get());
            }
            return false;
        }

        void query(const math::bounds2& range, std::vector<T*>& out_elements)
        {
            std::stack<quadtree_t*> stack({ this });
            while (!stack.empty())
            {
                quadtree_t* q = stack.top();
                stack.pop();

                if (!math::overlaps(q->bounds, range))
                    continue;

                for (size_t i = 0; i < q->size; i++)
                {
                    if (math::inside(q->elements[i].pos, range))
                    {
                        out_elements.push_back(&q->elements[i]);
                    }
                }

                if (q->divided)
                {
                    stack.push(q->top_left.get());
                    stack.push(q->top_right.get());
                    stack.push(q->bottom_right.get());
                    stack.push(q->bottom_left.get());
                }
            }
        }

        void query_all(std::vector<T*>& out_elements)
        {
            std::stack<quadtree_t*> stack({ this });
            while (!stack.empty())
            {
                quadtree_t* q = stack.top();
                stack.pop();

                for (size_t i = 0; i < q->size; i++)
                {
                    out_elements.push_back(&q->elements[i]);
                }

                if (q->divided)
                {
                    stack.push(q->top_left.get());
                    stack.push(q->top_right.get());
                    stack.push(q->bottom_right.get());
                    stack.push(q->bottom_left.get());
                }
            }
        }

        void clear()
        {
            size = 0;

            top_left = nullptr;
            top_right = nullptr;
            bottom_right = nullptr;
            bottom_left = nullptr;

            divided = false;
        }

    private:
        T elements[capacity];
        size_t size = 0;

        std::unique_ptr<quadtree_t> top_left;
        std::unique_ptr<quadtree_t> top_right;
        std::unique_ptr<quadtree_t> bottom_right;
        std::unique_ptr<quadtree_t> bottom_left;

        bool divided = false;

        void subdivide()
        {
            if (divided)
                return;
            else
                divided = true;

            math::vec2 center = (bounds.pmin + bounds.pmax) * .5f;

            top_left = std::make_unique<quadtree_t>(
                capacity,
                math::bounds2(center, math::vec2(bounds.pmin.x, bounds.pmax.y))
            );
            top_right = std::make_unique<quadtree_t>(
                capacity,
                math::bounds2(center, bounds.pmax)
            );
            bottom_right = std::make_unique<quadtree_t>(
                capacity,
                math::bounds2(center, math::vec2(bounds.pmax.x, bounds.pmin.y))
            );
            bottom_left = std::make_unique<quadtree_t>(
                capacity,
                math::bounds2(center, bounds.pmin)
            );
        }

    };

}
