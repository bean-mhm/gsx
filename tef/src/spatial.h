#pragma once

// STD
#include <vector>
#include <memory>
#include <cstdint>

// Internal
#include "math/math.h"
#include "utils.h"

namespace tef::spatial
{

    struct point_t
    {
        math::vec2 pos;
    };

    // Quadtree data structure with a given capacity per tile
    // Note: T must have a constructor with no arguments.
    // Note: T must have a field of type tef::math::vec2 named pos, representing the 2D position.
    template<typename T, size_t capacity>
    class quadtree_t
    {
    public:
        const math::bounds2 bounds;

        quadtree_t(const math::bounds2& bounds)
            : bounds(bounds)
        {}

        no_default_copy_move_constructor(quadtree_t);

        bool insert(const T& elem)
        {
            if (!math::inside(elem.pos, bounds))
                return false;

            if (size < capacity)
            {
                elements[size] = elem;
                size++;
                return true;
            }

            subdivide();

            if (top_left->insert(elem)) return true;
            if (top_right->insert(elem)) return true;
            if (bottom_right->insert(elem)) return true;
            if (bottom_left->insert(elem)) return true;
        }

        void query(const math::bounds2& range, std::vector<T*>& out_elements)
        {
            if (!math::overlaps(bounds, range))
                return;

            for (size_t i = 0; i < size; i++)
            {
                if (math::inside(elements[i].pos, range))
                {
                    out_elements.push_back(&elements[i]);
                }
            }

            if (divided)
            {
                top_left->query(range, out_elements);
                top_right->query(range, out_elements);
                bottom_right->query(range, out_elements);
                bottom_left->query(range, out_elements);
            }
        }

        void query_all(std::vector<T*>& out_elements)
        {
            for (size_t i = 0; i < size; i++)
            {
                out_elements.push_back(&elements[i]);
            }

            if (divided)
            {
                top_left->query_all(out_elements);
                top_right->query_all(out_elements);
                bottom_right->query_all(out_elements);
                bottom_left->query_all(out_elements);
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
            if (divided) return;
            divided = true;

            math::vec2 center = (bounds.pmin + bounds.pmax) * .5f;

            top_left = std::make_unique<quadtree_t>(
                capacity,
                math::bounds2(center, { bounds.pmin.x, bounds.pmax.y })
            );
            top_right = std::make_unique<quadtree_t>(
                capacity,
                math::bounds2(center, bounds.pmax)
            );
            bottom_right = std::make_unique<quadtree_t>(
                capacity,
                math::bounds2(center, { bounds.pmax.x, bounds.pmin.y })
            );
            bottom_left = std::make_unique<quadtree_t>(
                capacity,
                math::bounds2(center, bounds.pmin)
            );
        }

    };

}
