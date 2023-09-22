#pragma once

#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <array>
#include <algorithm>
#include <cctype>
#include <locale>
#include <clocale>
#include <ctime>
#include <cstdint>

namespace tef::str
{

    template<typename ... Args>
    std::string format(const std::string& format, Args ... args)
    {
        int size_s = std::snprintf(nullptr, 0, format.c_str(), args ...) + 1;
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
    std::string from_bool(bool v);
    std::string from_enum_values(
        const std::string& name,
        const std::vector<std::string>& values,
        int starting_index = 0
    );

    // Examples: "7h 9m 32s", "10.7s"
    std::string from_duration(float seconds);

    // Examples: "07:09:32", "00:00:10"
    std::string from_elapsed(float seconds);

    // Example: "2023-07-30 15:38:09"
    std::string from_time();

    template <typename T>
    std::string from_number(T v)
    {
        if constexpr (std::is_same_v<T, float> || std::is_same_v<T, double>)
        {
            return format("%.3f", (float)v);
        }
        return std::to_string(v);
    }

    int64_t to_int(const std::string& s);
    float to_float(const std::string& s);

    template <size_t size>
    std::string from_float_array(const std::array<float, size>& arr)
    {
        std::string result = "";
        for (size_t i = 0; i < size; i++)
        {
            if (i != 0) result += ", ";
            result += format("%.3f", arr[i]);
        }
        return result;
    }

    std::array<float, 4> to_rgba(const std::string& s);
    std::array<float, 3> to_rgb(const std::string& s);
    std::array<float, 2> to_xy(const std::string& s);

    template< typename T >
    std::string from_hex(T i)
    {
        std::stringstream stream;
        stream << "0x"
            << std::setfill('0') << std::setw(sizeof(T) * 2)
            << std::hex << i;
        return stream.str();
    }

}
