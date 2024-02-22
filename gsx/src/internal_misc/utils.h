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
    template<std::floating_point T>
    inline void sleep(T seconds)
    {
        std::this_thread::sleep_for(
            std::chrono::nanoseconds((u64)(seconds * (T)1e9))
        );
    }

    template<std::floating_point T>
    T elapsed_sec(std::chrono::steady_clock::time_point t_start)
    {
        return (T)1e-9 * std::chrono::duration_cast<std::chrono::nanoseconds>(
            std::chrono::high_resolution_clock::now() - t_start
        ).count();
    }

    template<std::floating_point T>
    T elapsed_sec(
        std::chrono::steady_clock::time_point t_start,
        std::chrono::steady_clock::time_point t_end
    )
    {
        return (T)1e-9 * std::chrono::duration_cast<std::chrono::nanoseconds>(
            t_end - t_start
        ).count();
    }

}
