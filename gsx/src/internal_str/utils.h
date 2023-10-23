#pragma once

// STD
#include <iostream>
#include <string>
#include <format>
#include <vector>
#include <type_traits>
#include <cstdint>

// Internal
#include "../internal_common/all.h"

namespace gsx::str
{

    std::string lower(std::string s);
    std::string upper(std::string s);

    void ltrim_inplace(std::string& s);
    void rtrim_inplace(std::string& s);
    void trim_inplace(std::string& s);
    std::string ltrim(std::string s);
    std::string rtrim(std::string s);
    std::string trim(std::string s);

    std::string lpad(const std::string& s, u64 length, bool add_space = true);
    std::string rpad(const std::string& s, u64 length, bool add_space = true);

    bool contains(const std::string& s, const std::string& substring);
    void replace_inplace(std::string& s, const std::string& from, const std::string& to);
    std::string replace(std::string s, const std::string& from, const std::string& to);

    std::string wordwrap(const std::string& s, u64 line_length, u64 left_padding = 0);

    std::string from_file(const std::string& path);

    std::string from_list(const std::vector<std::string>& list, const std::string& delimiter);
    void to_list(const std::string& s, char delimiter, std::vector<std::string>& out_elements);

    std::string from_data_size(u64 bytes);
    std::string from_large_number(u64 n);

    // Examples: "7h 9m 32s", "10.7s"
    std::string from_duration(f32 seconds);

    // Examples: "07:09:32", "00:00:10"
    std::string from_elapsed(f32 seconds);

    // Example: "2023-07-30 15:38:09"
    std::string from_time();

    template<typename T>
    std::string from_number(T v)
    {
        if constexpr (std::is_same_v<T, f32> || std::is_same_v<T, f64>)
        {
            return std::format("{:.3f}", v);
        }
        return std::format("{}", v);
    }

    i64 to_i64(const std::string& s);
    f32 to_f32(const std::string& s);

}
