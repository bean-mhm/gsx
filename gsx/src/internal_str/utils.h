#pragma once

#include <iostream>
#include <string>
#include <format>
#include <vector>
#include <type_traits>
#include <cstdint>

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
    bool contains(const std::string& s, char ch);
    void replace_inplace(
        std::string& s,
        const std::string& from,
        const std::string& to
    );
    std::string replace(
        std::string s,
        const std::string& from,
        const std::string& to
    );

    std::string wordwrap(
        const std::string& s,
        u64 line_length,
        u64 left_padding = 0
    );

    std::string from_file(const std::string& path);

    std::string from_list(
        const std::vector<std::string>& list,
        const std::string& delimiter
    );
    void to_list(
        const std::string& s,
        char delimiter,
        std::vector<std::string>& out_elements
    );

    std::string from_data_size(u64 bytes);
    std::string from_large_number(u64 n);

    // examples: "7h 9m 32s", "10.7s"
    template<std::floating_point T>
    std::string from_duration(T seconds)
    {
        if (seconds < 1)
        {
            return std::format("{:.3f} s", seconds);
        }
        else if (seconds < 60)
        {
            return std::format("{:.1f} s", seconds);
        }
        else
        {
            u64 isec = (u64)std::floor(seconds);
            u64 ihr = isec / 3600;
            u64 imin = (isec / 60) % 60;
            isec %= 60;
            if (ihr > 0)
            {
                return std::format("{}h {}m {}s", ihr, imin, isec);
            }
            else
            {
                return std::format("{}m {}s", imin, isec);
            }
        }
    }

    // examples: "07:09:32", "00:00:10"
    template<std::floating_point T>
    std::string from_elapsed(T seconds)
    {
        u64 isec = (u64)std::floor(seconds);
        u64 ihr = isec / 3600;
        u64 imin = (isec / 60) % 60;
        isec %= 60;
        return std::format("{:02}:{:02}:{:02}", ihr, imin, isec);
    }

    // example: "2023-07-30 15:38:09"
    std::string from_time();

    template<typename T>
    std::string from_number(T v)
    {
        if constexpr (std::is_floating_point_v<T>)
        {
            return std::format("{:.3f}", v);
        }
        return std::format("{}", v);
    }

    i64 to_i64(const std::string& s);
    u64 to_u64(const std::string& s);
    f32 to_f32(const std::string& s);
    f64 to_f64(const std::string& s);

}
