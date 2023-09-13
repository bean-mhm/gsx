#pragma once

#include <string>
#include <functional>

namespace tef
{

    // Log level
    enum class log_level_t
    {
        error = 0,
        warning,
        info,
        verbose
    };

    // Maximum log level to ever use (global)
    static constexpr log_level_t max_log_level = log_level_t::verbose;

    // Callback function for logging
    using cb_log_t = std::function<
        void(
            log_level_t log_level,
            const std::string& world_name,
            std::thread::id thread_id,
            const std::string& message)
    >;

}

// Log macro (used internally)
#define tef_log(log_level, world_name, message) if constexpr(log_level <= max_log_level) \
cb_log(log_level, world_name, std::this_thread::get_id(), message)
