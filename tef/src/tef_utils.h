#pragma once

#include <string>
#include <vector>
#include <chrono>
#include <cstdint>

#define no_default_constructor(CLASS) CLASS() = delete
#define no_copy_constructor(CLASS) CLASS(const CLASS&) = delete
#define no_move_constructor(CLASS) CLASS& operator= (const CLASS&) = delete
#define no_copy_move_constructor(CLASS) no_copy_constructor(CLASS); no_move_constructor(CLASS);
#define no_default_copy_move_constructor(CLASS) no_default_constructor(CLASS); no_copy_constructor(CLASS); no_move_constructor(CLASS);

namespace tef
{

    template<typename ... Args>
    std::string str_format(const std::string& format, Args ... args)
    {
        int size_s = std::snprintf(nullptr, 0, format.c_str(), args ...) + 1; // Extra space for '\0'
        if (size_s <= 0)
        {
            // Error during formatting
            return format;
        }
        auto size = static_cast<uint64_t>(size_s);
        std::unique_ptr<char[]> buf(new char[size]);
        std::snprintf(buf.get(), size, format.c_str(), args ...);
        return std::string(buf.get(), buf.get() + size - 1); // We don't want the '\0' inside
    }

    const char* cstr_from_bool(bool v);

    template <typename T>
    void vec_clear(std::vector<T>& vec)
    {
        vec.clear();
        std::vector<T>().swap(vec);
    }

    template <typename T>
    void vec_insert_contents(std::vector<T>& a, const std::vector<T>& b)
    {
        a.reserve(a.size() + b.size());
        a.insert(a.end(), b.begin(), b.end());
    }

    template <typename T>
    void vec_remove(std::vector<T>& vec, size_t pos)
    {
        vec.erase(vec.begin() + pos);
    }

    template <typename T>
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

    template <typename T>
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
