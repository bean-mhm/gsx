#pragma once

// STD
#include <iostream>
#include <string>
#include <format>
#include <functional>
#include <stdexcept>

namespace test
{

    inline void start_group(const char* name)
    {
        std::cout << std::format("test group: {}\n", name);
        std::cout << "----------------------------------------\n";
    }

    inline void end_group()
    {
        std::cout << "----------------------------------------\n\n";
    }

    template <typename fn_t, typename... arg_t>
    inline void run(const char* name, fn_t fn, arg_t... args)
    {
        try
        {
            fn(args...);
            std::cout << std::format("[ PASS ] {}\n", name);
        }
        catch (const std::exception& e)
        {
            std::string message = e.what();
            if (message.empty())
            {
                std::cout << std::format("[ FAIL ] {}\n", name);
            }
            else
            {
                std::cout << std::format("[ FAIL ] {}: {}\n", name, message);
            }
        }
    }

    inline void assert(bool cond, const char* message)
    {
        if (!cond)
            throw std::runtime_error(message);
    }

    inline void assert(bool cond, const std::string& message)
    {
        if (!cond)
            throw std::runtime_error(message.c_str());
    }

}
