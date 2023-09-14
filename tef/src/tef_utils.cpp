#include "tef_utils.h"

// STD
#include <time.h>

namespace tef::utils
{
    // https://stackoverflow.com/a/3418285/18049911
    void str_replace_in_place(std::string& s, const std::string& from, const std::string& to)
    {
        if (from.empty())
            return;

        uint64_t start_pos = 0;
        while ((start_pos = s.find(from, start_pos)) != std::string::npos)
        {
            s.replace(start_pos, from.length(), to);
            start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
        }
    }

    std::string str_replace(const std::string& s, const std::string& from, const std::string& to)
    {
        std::string s_copy = s;
        str_replace_in_place(s_copy, from, to);
        return s_copy;
    }

    std::string str_from_time()
    {
        time_t raw_time;
        tm time_info;
        char buffer[128];

        time(&raw_time);
        localtime_s(&time_info, &raw_time);
        strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &time_info);

        return std::string(buffer);
    }

    const char* cstr_from_bool(bool v)
    {
        return v ? "true" : "false";
    }

    float elapsed_sec(std::chrono::steady_clock::time_point t_start)
    {
        return 1e-9f * std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - t_start).count();
    }

    float elapsed_sec(
        std::chrono::steady_clock::time_point t_start,
        std::chrono::steady_clock::time_point t_end
    )
    {
        return 1e-9f * std::chrono::duration_cast<std::chrono::nanoseconds>(t_end - t_start).count();
    }

}
