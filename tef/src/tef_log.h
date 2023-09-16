#pragma once

// STD
#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include <mutex>

// Internal
#include "tef_utils.h"

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
    static constexpr log_level_t max_log_level = log_level_t::error;

    // Log entry
    struct log_entry_t
    {
        log_level_t log_level;
        std::string world_name;
        std::thread::id thread_id;
        std::string message;

        log_entry_t(log_level_t log_level,
            const std::string& world_name,
            std::thread::id thread_id,
            const std::string& message);
    };

    // Abstract class for logging
    // Note: The abstract function log() may be called from several threads simultaneously, so
    // any derived class must manually implement synchronization.
    class base_logger_t
    {
    public:
        virtual ~base_logger_t();
        virtual void log(const log_entry_t& entry) = 0;

    };

    // std::ostream logger
    class ostream_logger_t : public base_logger_t
    {
    public:
        std::ostream& stream;

        ostream_logger_t(std::ostream& stream);
        no_default_copy_move_constructor(ostream_logger_t);
        virtual ~ostream_logger_t();

        virtual void log(const log_entry_t& entry) override;

    private:
        std::mutex mutex;

    };

    // CSV logger
    class csv_logger_t : public base_logger_t
    {
    public:
        csv_logger_t(const std::string& filename);
        no_default_copy_move_constructor(csv_logger_t);
        virtual ~csv_logger_t();

        virtual void log(const log_entry_t& entry) override;
        bool is_open() const;
        void close();

    private:
        const std::string filename;
        std::ofstream log_file;
        std::mutex mutex;

    };

}

// Log macro (used internally)
#define tef_log(log_level, world_name, message) if constexpr(log_level <= max_log_level) \
logger->log(log_entry_t(log_level, world_name, std::this_thread::get_id(), message))
