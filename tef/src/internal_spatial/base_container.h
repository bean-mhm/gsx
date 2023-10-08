#pragma once

// STD
#include <vector>

// Internal
#include "../internal_common/all.h"
#include "../internal_math/all.h"

namespace tef::spatial
{

    template<typename T>
    class base_container_2d_t
    {
    public:
        virtual usize size() const = 0;
        virtual bool insert(const T& element) = 0;
        virtual void query(const math::bounds2& range, std::vector<T*>& out_elements) = 0;
        virtual void query_all(std::vector<T*>& out_elements) = 0;
        virtual void query_all(std::vector<T>& out_elements) const = 0;
        virtual void clear() = 0;
        virtual void rebuild() = 0;

    };

    template<typename T>
    class base_container_3d_t
    {
    public:
        virtual usize size() const = 0;
        virtual bool insert(const T& element) = 0;
        virtual void query(const math::bounds3& range, std::vector<T*>& out_elements) = 0;
        virtual void query_all(std::vector<T*>& out_elements) = 0;
        virtual void query_all(std::vector<T>& out_elements) const = 0;
        virtual void clear() = 0;
        virtual void rebuild() = 0;

    };

}