#pragma once

// STD
#include <iostream>
#include <string>
#include <format>
#include <vector>
#include <type_traits>
#include <cstdint>

namespace tef::str
{

    std::string lower(std::string s);
    std::string upper(std::string s);

    void ltrim_inplace(std::string& s);
    void rtrim_inplace(std::string& s);
    void trim_inplace(std::string& s);
    std::string ltrim(std::string s);
    std::string rtrim(std::string s);
    std::string trim(std::string s);

    std::string lpad(const std::string& s, uint64_t length, bool add_space = true);
    std::string rpad(const std::string& s, uint64_t length, bool add_space = true);

    bool contains(const std::string& s, const std::string& substring);
    void replace_inplace(std::string& s, const std::string& from, const std::string& to);
    std::string replace(std::string s, const std::string& from, const std::string& to);

    std::string wordwrap(const std::string& s, uint64_t line_length, uint64_t left_padding = 0);

    std::string from_list(const std::vector<std::string>& list, const std::string& delimiter);
    void to_list(const std::string& s, char delimiter, std::vector<std::string>& out_elements);

    std::string from_data_size(uint64_t bytes);
    std::string from_large_number(uint64_t n);

    // Examples: "7h 9m 32s", "10.7s"
    std::string from_duration(float seconds);

    // Examples: "07:09:32", "00:00:10"
    std::string from_elapsed(float seconds);

    // Example: "2023-07-30 15:38:09"
    std::string from_time();

    template<typename T>
    std::string from_number(T v)
    {
        if constexpr (std::is_same_v<T, float> || std::is_same_v<T, double>)
        {
            return std::format("{:.3f}", v);
        }
        return std::format("{}", v);
    }

    int64_t to_i64(const std::string& s);
    float to_float(const std::string& s);

}
