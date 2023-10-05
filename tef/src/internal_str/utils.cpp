#include "utils.h"

// STD
#include <sstream>
#include <algorithm>
#include <stdexcept>
#include <ctime>
#include <cmath>
#include <cctype>

namespace tef::str
{

    std::string lower(std::string s)
    {
        std::transform(s.cbegin(), s.cend(), s.begin(),
            [](unsigned char c)
            {
                return std::tolower(c);
            }
        );
        return s;
    }

    std::string upper(std::string s)
    {
        std::transform(s.cbegin(), s.cend(), s.begin(),
            [](unsigned char c)
            {
                return std::toupper(c);
            }
        );
        return s;
    }

    void ltrim_inplace(std::string& s)
    {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch)
            {
                return !std::isspace(ch);
            }));
    }

    void rtrim_inplace(std::string& s)
    {
        s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch)
            {
                return !std::isspace(ch);
            }).base(), s.end());
    }

    void trim_inplace(std::string& s)
    {
        ltrim_inplace(s);
        rtrim_inplace(s);
    }

    std::string ltrim(std::string s)
    {
        ltrim_inplace(s);
        return s;
    }

    std::string rtrim(std::string s)
    {
        rtrim_inplace(s);
        return s;
    }

    std::string trim(std::string s)
    {
        trim_inplace(s);
        return s;
    }

    std::string lpad(const std::string& s, uint64_t length, bool add_space)
    {
        if (length > s.size())
        {
            uint64_t n_spaces = (length - s.size());
            std::string padding;
            for (uint64_t i = 0; i < n_spaces; i++)
                padding += " ";
            return padding + s;
        }
        return add_space ? (" " + s) : s;
    }

    std::string rpad(const std::string& s, uint64_t length, bool add_space)
    {
        if (length > s.size())
        {
            uint64_t n_spaces = (length - s.size());
            std::string padding;
            for (uint64_t i = 0; i < n_spaces; i++)
                padding += " ";
            return s + padding;
        }
        return add_space ? (s + " ") : s;
    }

    bool contains(const std::string& s, const std::string& substring)
    {
        return (s.find(substring) != std::string::npos);
    }

    // https://stackoverflow.com/a/3418285/18049911
    void replace_inplace(std::string& s, const std::string& from, const std::string& to)
    {
        if (from.empty())
            return;

        uint64_t start_pos = 0;
        while ((start_pos = s.find(from, start_pos)) != std::string::npos)
        {
            s.replace(start_pos, from.length(), to);

            // In case 'to' contains 'from', like replacing 'x' with 'yx'
            start_pos += to.length();
        }
    }

    std::string replace(std::string s, const std::string& from, const std::string& to)
    {
        replace_inplace(s, from, to);
        return s;
    }

    std::string wordwrap(const std::string& s, uint64_t line_length, uint64_t left_padding)
    {
        std::istringstream i(s);
        std::ostringstream o("");

        uint64_t length = line_length - left_padding;
        line_length = 0;

        uint64_t num_words = 0;
        std::string word;

        while (i >> word)
        {
            if (((line_length + word.size()) > length) && (num_words > 0))
            {
                o << '\n';
                for (uint64_t i = 0; i < left_padding; i++)
                    o << ' ';
                line_length = 0;
            }

            if (word.size() > length)
            {
                o << word.substr(0, length - 1) << "-\n";
                for (uint64_t i = 0; i < left_padding; i++)
                    o << ' ';
                o << word.substr(length - 1) << ' ';
                line_length = (word.size() - length + 1) + 1;
            }
            else
            {
                o << word << ' ';
                line_length += word.size() + 1;
            }

            num_words++;
        }

        std::string result = o.str();
        result = result.substr(0, result.size() - 1);
        return result;
    }

    std::string from_list(const std::vector<std::string>& list, const std::string& delimiter)
    {
        std::string result = "";
        for (uint64_t i = 0; i < list.size(); i++)
        {
            if (i > 0) result += delimiter;
            result += list[i];
        }
        return result;
    }

    void to_list(const std::string& s, char delimiter, std::vector<std::string>& out_elements)
    {
        out_elements.clear();

        std::uint64_t from = 0;
        for (std::uint64_t i = 0; i < s.size(); ++i)
        {
            if (s[i] == delimiter)
            {
                out_elements.push_back(trim(s.substr(from, i - from)));
                from = i + 1;
            }
        }

        if (from < s.size())
            out_elements.push_back(trim(s.substr(from, s.size() - from)));
    }

    std::string from_data_size(uint64_t bytes)
    {
        static const char* suffixes[]{ "bytes", "KiB", "MiB", "GiB", "TiB" };
        static double powers[]
        {
            std::pow(1024., 0.), // 1 byte
            std::pow(1024., 1.), // 1 KB
            std::pow(1024., 2.), // 1 MB
            std::pow(1024., 3.), // 1 GB
            std::pow(1024., 4.)  // 1 TB
        };

        uint64_t mag = std::clamp(
            (uint64_t)std::floor(std::log((double)bytes) / std::log(1024.)),
            0ull,
            4ull
        );

        if (mag == 0)
            return std::format("{} {}", bytes, suffixes[mag]);
        else
            return std::format("{:.1f} {}", (double)bytes / powers[mag], suffixes[mag]);
    }

    std::string from_large_number(uint64_t n)
    {
        static const char* suffixes[]{ "", "K", "M", "B", "T" };
        static double powers[]
        {
            std::pow(1000., 0.), // 1
            std::pow(1000., 1.), // 1K
            std::pow(1000., 2.), // 1M
            std::pow(1000., 3.), // 1B
            std::pow(1000., 4.)  // 1T
        };

        uint64_t mag = std::clamp(
            (uint64_t)std::floor(std::log((double)n) / std::log(1000.)),
            0ull,
            4ull
        );

        if (mag == 0)
            return std::format("{}", n);
        else
            return std::format("{:.1f}{}", (double)n / powers[mag], suffixes[mag]);
    }

    std::string from_duration(float seconds)
    {
        if (seconds < 1.f)
        {
            return std::format("{:.3f} s", seconds);
        }
        else if (seconds < 60.f)
        {
            return std::format("{:.1f} s", seconds);
        }
        else
        {
            uint64_t isec = (uint64_t)std::floor(seconds);
            uint64_t ihr = isec / 3600;
            uint64_t imin = (isec / 60) % 60;
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

    std::string from_elapsed(float seconds)
    {
        uint64_t isec = (uint64_t)std::floor(seconds);
        uint64_t ihr = isec / 3600;
        uint64_t imin = (isec / 60) % 60;
        isec %= 60;
        return std::format("{:02}:{:02}:{:02}", ihr, imin, isec);
    }

    std::string from_time()
    {
        time_t rawtime;
        struct tm timeinfo;
        char buffer[128];

        time(&rawtime);
        localtime_s(&timeinfo, &rawtime);
        strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &timeinfo);

        return std::string(buffer);
    }

    int64_t to_i64(const std::string& s)
    {
        try
        {
            return std::stoll(s);
        }
        catch (const std::exception& e)
        {
            throw std::runtime_error(std::format(
                "Couldn't parse a 64-bit signed integer from \"{}\".",
                s
            ).c_str());
        }
    }

    float to_float(const std::string& s)
    {
        try
        {
            return std::stof(s);
        }
        catch (const std::exception& e)
        {
            throw std::runtime_error(std::format(
                "Couldn't parse a float from \"{}\".",
                s
            ).c_str());
        }
    }

}