#pragma once

// STD
#include <vector>
#include <chrono>
#include <cstdint>

#define no_default_constructor(CLASS) CLASS() = delete
#define no_copy_constructor(CLASS) CLASS(const CLASS&) = delete
#define no_move_constructor(CLASS) CLASS& operator= (const CLASS&) = delete
#define no_copy_move_constructor(CLASS) no_copy_constructor(CLASS); no_move_constructor(CLASS);
#define no_default_copy_move_constructor(CLASS) no_default_constructor(CLASS); \
no_copy_constructor(CLASS); no_move_constructor(CLASS);

namespace tef::misc
{

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
    void vec_remove(std::vector<T>& vec, size_t pos)
    {
        vec.erase(vec.begin() + pos);
    }

    template<typename T>
    ptrdiff_t vec_find_index(const std::vector<T>& vec, const T& value)
    {
        for (size_t i = 0; i < vec.size(); i++)
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

    float elapsed_sec(std::chrono::steady_clock::time_point t_start);

    float elapsed_sec(
        std::chrono::steady_clock::time_point t_start,
        std::chrono::steady_clock::time_point t_end
    );

}
