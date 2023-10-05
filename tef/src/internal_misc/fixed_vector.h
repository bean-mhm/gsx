#pragma once

// STD
#include <utility>
#include <stdexcept>
#include <cstddef>
#include <cstdint>

namespace tef::misc
{

    template<typename T, uint8_t _capacity>
        requires (_capacity <= 255)
    class fixed_vector_t
    {
    private:
        alignas(T) std::byte storage[sizeof(T) * _capacity];
        uint8_t _size = 0;

    public:
        fixed_vector_t() = default;

        ~fixed_vector_t()
        {
            clear();
        }

        fixed_vector_t(const fixed_vector_t& other)
        {
            for (size_t i = 0; i < other.size(); i++)
            {
                emplace_back(other[i]);
            }
        }

        fixed_vector_t(fixed_vector_t&& other)
        {
            for (size_t i = 0; i < other.size(); i++)
            {
                emplace_back(std::move(other[i]));
            }
            other.clear();
        }

        fixed_vector_t& operator= (const fixed_vector_t& other)
        {
            if (this == &other) return;
            clear();
            for (size_t i = 0; i < other.size(); i++)
            {
                emplace_back(other[i]);
            }
        }

        fixed_vector_t& operator= (fixed_vector_t&& other)
        {
            if (this == &other) return;
            clear();
            for (size_t i = 0; i < other.size(); i++)
            {
                emplace_back(std::move(other[i]));
            }
            other.clear();
        }

        T& operator[](uint8_t pos) noexcept
        {
            return reinterpret_cast<T*>(storage)[pos];
        }

        const T& operator[](uint8_t pos) const noexcept
        {
            return reinterpret_cast<const T*>(storage)[pos];
        }

        constexpr uint8_t size() const noexcept
        {
            return _size;
        }

        constexpr uint8_t capacity() const noexcept
        {
            return _capacity;
        }

        void clear() noexcept
        {
            for (uint8_t i = 0; i < _size; i++)
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
            return *new(reinterpret_cast<T*>(storage) + _size++) T(std::forward<Args>(args)...);
        }

    };

}
