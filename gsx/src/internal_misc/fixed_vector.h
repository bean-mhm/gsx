#pragma once

#include <utility>
#include <stdexcept>
#include <cstddef>
#include <cstdint>

#include "../internal_common/all.h"

namespace gsx::misc
{

    template<typename T, u8 _capacity>
        requires (_capacity <= 255)
    class fixed_vector_t
    {
    private:
        alignas(T) std::byte storage[sizeof(T) * _capacity];
        u8 _size = 0;

    public:
        fixed_vector_t() = default;

        ~fixed_vector_t()
        {
            clear();
        }

        fixed_vector_t(const fixed_vector_t& other)
        {
            for (u8 i = 0; i < other.size(); i++)
            {
                emplace_back(other[i]);
            }
        }

        fixed_vector_t(fixed_vector_t&& other)
        {
            for (u8 i = 0; i < other.size(); i++)
            {
                emplace_back(std::move(other[i]));
            }
            other.clear();
        }

        fixed_vector_t& operator= (const fixed_vector_t& other)
        {
            if (this == &other) return;
            clear();
            for (u8 i = 0; i < other.size(); i++)
            {
                emplace_back(other[i]);
            }
        }

        fixed_vector_t& operator= (fixed_vector_t&& other)
        {
            if (this == &other) return;
            clear();
            for (u8 i = 0; i < other.size(); i++)
            {
                emplace_back(std::move(other[i]));
            }
            other.clear();
        }

        T& operator[](u8 pos) noexcept
        {
            return reinterpret_cast<T*>(storage)[pos];
        }

        const T& operator[](u8 pos) const noexcept
        {
            return reinterpret_cast<const T*>(storage)[pos];
        }

        constexpr u8 size() const noexcept
        {
            return _size;
        }

        constexpr u8 capacity() const noexcept
        {
            return _capacity;
        }

        void clear() noexcept
        {
            for (u8 i = 0; i < _size; i++)
            {
                std::destroy_at(reinterpret_cast<T*>(storage) + i);
            }
            _size = 0;
        }

        void push_back(const T& value)
        {
            emplace_back(value);
        }

        void push_back(T&& value)
        {
            emplace_back(std::move(value));
        }

        template<typename... Args>
        T& emplace_back(Args&&... args)
        {
            if (_size >= _capacity)
                throw std::out_of_range("capacity full");
            return *new(reinterpret_cast<T*>(storage) + _size++) T(
                std::forward<Args>(args)...
            );
        }

    };

}
