#pragma once

// STD
#include <vector>
#include <stack>
#include <memory>
#include <cstdint>

// Internal
#include "base_container.h"
#include "../internal_common/all.h"
#include "../internal_math/all.h"
#include "../internal_misc/all.h"

namespace gsx::spatial
{

    // Quadtree with a given capacity per tile
    template<typename T, u8 capacity>
        requires (capacity <= 255)
    class quadtree_t : public base_container_2d_t<T>
    {
    public:
        quadtree_t(const math::bounds2& bounds)
            : _bounds(bounds)
        {}

        quadtree_t(const quadtree_t& other)
            : _bounds(other._bounds)
        {
            std::vector<T> elements_vec;
            other.query_all(elements_vec);
            for (auto& element : elements_vec)
            {
                insert(element);
            }
        }

        quadtree_t& operator= (const quadtree_t& other)
        {
            clear();
            _bounds = other._bounds;
            std::vector<T> elements_vec;
            other.query_all(elements_vec);
            for (auto& element : elements_vec)
            {
                insert(element);
            }
        }

        no_default_construct(quadtree_t);

        math::bounds2 bounds() const
        {
            return _bounds;
        }

        virtual usize size() const override
        {
            usize count = 0;
            std::stack<const quadtree_t*> stack({ this });
            while (!stack.empty())
            {
                const quadtree_t* q = stack.top();
                stack.pop();

                count += q->elements.size();

                if (q->divided)
                {
                    stack.push(q->bottom_left.get());
                    stack.push(q->bottom_right.get());
                    stack.push(q->top_left.get());
                    stack.push(q->top_right.get());
                }
            }
        }

        virtual bool insert(const T& element) override
        {
            std::stack<quadtree_t*> stack({ this });
            while (!stack.empty())
            {
                quadtree_t* q = stack.top();
                stack.pop();

                if (!math::inside(element.pos, q->_bounds))
                    continue;

                if (q->elements.size() < capacity)
                {
                    q->elements.push_back(element);
                    return true;
                }

                q->subdivide();

                stack.push(q->bottom_left.get());
                stack.push(q->bottom_right.get());
                stack.push(q->top_left.get());
                stack.push(q->top_right.get());
            }
            return false;
        }

        virtual void query(const math::bounds2& range, std::vector<T*>& out_elements) override
        {
            std::stack<quadtree_t*> stack({ this });
            while (!stack.empty())
            {
                quadtree_t* q = stack.top();
                stack.pop();

                if (!math::overlaps(q->_bounds, range))
                    continue;

                for (u8 i = 0; i < q->elements.size(); i++)
                {
                    if (math::inside(q->elements[i].pos, range))
                    {
                        out_elements.push_back(&q->elements[i]);
                    }
                }

                if (q->divided)
                {
                    stack.push(q->bottom_left.get());
                    stack.push(q->bottom_right.get());
                    stack.push(q->top_left.get());
                    stack.push(q->top_right.get());
                }
            }
        }

        virtual void query(const math::circle_t& range, std::vector<T*>& out_elements) override
        {
            std::stack<quadtree_t*> stack({ this });
            while (!stack.empty())
            {
                quadtree_t* q = stack.top();
                stack.pop();

                if (!math::overlaps(q->_bounds, range))
                    continue;

                for (u8 i = 0; i < q->elements.size(); i++)
                {
                    if (math::inside(q->elements[i].pos, range))
                    {
                        out_elements.push_back(&q->elements[i]);
                    }
                }

                if (q->divided)
                {
                    stack.push(q->bottom_left.get());
                    stack.push(q->bottom_right.get());
                    stack.push(q->top_left.get());
                    stack.push(q->top_right.get());
                }
            }
        }

        virtual void query_all(std::vector<T*>& out_elements) override
        {
            std::stack<quadtree_t*> stack({ this });
            while (!stack.empty())
            {
                quadtree_t* q = stack.top();
                stack.pop();

                out_elements.reserve(out_elements.size() + q->elements.size());
                for (u8 i = 0; i < q->elements.size(); i++)
                {
                    out_elements.push_back(&q->elements[i]);
                }

                if (q->divided)
                {
                    stack.push(q->bottom_left.get());
                    stack.push(q->bottom_right.get());
                    stack.push(q->top_left.get());
                    stack.push(q->top_right.get());
                }
            }
        }

        virtual void query_all(std::vector<T>& out_elements) const override
        {
            std::stack<const quadtree_t*> stack({ this });
            while (!stack.empty())
            {
                const quadtree_t* q = stack.top();
                stack.pop();

                out_elements.reserve(out_elements.size() + q->elements.size());
                for (u8 i = 0; i < q->elements.size(); i++)
                {
                    out_elements.push_back(q->elements[i]);
                }

                if (q->divided)
                {
                    stack.push(q->bottom_left.get());
                    stack.push(q->bottom_right.get());
                    stack.push(q->top_left.get());
                    stack.push(q->top_right.get());
                }
            }
        }

        virtual void clear() override
        {
            elements.clear();

            bottom_left = nullptr;
            bottom_right = nullptr;
            top_left = nullptr;
            top_right = nullptr;

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
        math::bounds2 _bounds;
        misc::fixed_vector_t<T, capacity> elements;

        std::unique_ptr<quadtree_t> bottom_left;
        std::unique_ptr<quadtree_t> bottom_right;
        std::unique_ptr<quadtree_t> top_left;
        std::unique_ptr<quadtree_t> top_right;

        bool divided = false;

        void subdivide()
        {
            if (divided)
                return;
            else
                divided = true;

            math::vec2 center = (_bounds.pmin + _bounds.pmax) * .5f;

            bottom_left = std::make_unique<quadtree_t>(
                math::bounds2(center, _bounds.pmin)
            );
            bottom_right = std::make_unique<quadtree_t>(
                math::bounds2(center, math::vec2(_bounds.pmax.x, _bounds.pmin.y))
            );
            top_left = std::make_unique<quadtree_t>(
                math::bounds2(center, math::vec2(_bounds.pmin.x, _bounds.pmax.y))
            );
            top_right = std::make_unique<quadtree_t>(
                math::bounds2(center, _bounds.pmax)
            );
        }

    };

}
