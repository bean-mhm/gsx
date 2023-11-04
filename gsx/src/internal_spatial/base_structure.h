#pragma once

// STD
#include <vector>

// Internal
#include "../internal_common/all.h"
#include "../internal_math/all.h"

struct test2
{
    gsx::math::vec2 pos;
};

struct test3
{
    gsx::math::vec3 pos;
};

namespace gsx::spatial
{

    // Base class for 2D spatial data structures
    // Note: T must be copy constructible.
    // Note: T must have a public field of type gsx::math::vec2 named pos, representing the 2D
    // position.
    template<typename T>
    class base_structure_2d_t
    {
    public:
        virtual usize size() const = 0;
        virtual void query(const math::bounds2& range, std::vector<T*>& out_elements) = 0;
        virtual void query(const math::circle_t& range, std::vector<T*>& out_elements) = 0;
        virtual void query_all(std::vector<T*>& out_elements) = 0;
        virtual void query_all(std::vector<T>& out_elements) const = 0;
        virtual bool insert(const T& element) = 0;
        virtual void clear() = 0;
        virtual void rebuild() = 0;

    };

    // Base class for 3D spatial data structures
    // Note: T must be copy constructible.
    // Note: T must have a public field of type gsx::math::vec3 named pos, representing the 3D
    // position.
    template<typename T>
    class base_structure_3d_t
    {
    public:
        virtual usize size() const = 0;
        virtual void query(const math::bounds3& range, std::vector<T*>& out_elements) = 0;
        virtual void query(const math::sphere_t& range, std::vector<T*>& out_elements) = 0;
        virtual void query_all(std::vector<T*>& out_elements) = 0;
        virtual void query_all(std::vector<T>& out_elements) const = 0;
        virtual bool insert(const T& element) = 0;
        virtual void clear() = 0;
        virtual void rebuild() = 0;

    };

}
