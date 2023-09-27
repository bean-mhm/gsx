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

    inline void run(const char* name, std::function<void()> f)
    {
        try
        {
            f();
            std::cout << std::format("[  PASS  ] {}\n", name);
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

    template <typename... arg_types>
    inline void run(const char* name, std::function<void(arg_types...)> f, arg_types... args)
    {
        run(name,
            [&]()
            {
                f(args...);
            }
        );
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
