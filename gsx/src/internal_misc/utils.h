#pragma once

#include <vector>
#include <chrono>
#include <thread>
#include <cstdint>

#include "../internal_common/all.h"

namespace gsx::misc
{

    // type& -> type*
    template<typename T>
    constexpr T* add_ptr(T& instance)
    {
        return &instance;
    };

    // type* -> type*
    template<typename T>
    constexpr T* add_ptr(T* instance)
    {
        return instance;
    };

    // type& -> type&
    template<typename T>
    constexpr T& remove_ptr(T& instance)
    {
        return instance;
    };

    // type* -> type&
    template<typename T>
    constexpr T& remove_ptr(T* instance)
    {
        return *instance;
    };

    // type& -> type*
    template<typename T_from, typename T_to>
    constexpr typename std::enable_if<std::is_same_v<T_to, T_from*>, T_to>::type
        add_or_remove_ptr(T_from& instance)
    {
        return &instance;
    }

    // type* -> type&
    template<typename T_from, typename T_to>
    constexpr typename
        std::enable_if<std::is_same_v<T_from, T_to*>, T_to&>::type
        add_or_remove_ptr(T_from& instance)
    {
        return *instance;
    }

    // type& -> type&
    // type* -> type*
    template<typename T_from, typename T_to>
    constexpr typename std::enable_if<std::is_same_v<T_from, T_to>, T_to&>::type
        add_or_remove_ptr(T_from& instance)
    {
        return instance;
    }

    template<typename T>
    void vec_clear(std::vector<T>& vec)
    {
        vec.clear();
        std::vector<T>().swap(vec);
    }

    template<typename T>
    void vec_append(std::vector<T>& vec, const std::vector<T>& to_add)
    {
        vec.reserve(vec.size() + to_add.size());
        vec.insert(vec.end(), to_add.begin(), to_add.end());
    }

    template<typename T>
    void vec_remove(std::vector<T>& vec, usize pos)
    {
        vec.erase(vec.begin() + pos);
    }

    template<typename T>
    ptrdiff_t vec_find_index(const std::vector<T>& vec, const T& value)
    {
        for (usize i = 0; i < vec.size(); i++)
        {
            if (value == vec[i])
            {
                return i;
            }
        }
        return -1;
    }

    template<typename T>
    bool vec_contains(const std::vector<T>& vec, const T& value)
    {
        return vec_find_index(vec, value) > -1;
    }

    // sleep using std::this_thread::sleep_for().
    inline void sleep(f32 seconds)
    {
        std::this_thread::sleep_for(
            std::chrono::nanoseconds((u64)(seconds * 1e9f))
        );
    }

    f32 elapsed_sec(std::chrono::steady_clock::time_point t_start);

    f32 elapsed_sec(
        std::chrono::steady_clock::time_point t_start,
        std::chrono::steady_clock::time_point t_end
    );

}
