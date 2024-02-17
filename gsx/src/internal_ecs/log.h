#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include <mutex>

#include "../internal_common/all.h"
#include "../internal_str/all.h"

// for internal use only. use world_t::log() instead.
#define gsx_log(world_ptr, log_level, message) \
if (log_level <= world_ptr->max_log_level) \
logger->log(log_entry_t(\
    log_level, world_ptr->name, std::this_thread::get_id(), message\
))

namespace gsx::ecs
{

    enum class log_level_t
    {
        error = 0,
        warning,
        info,
        verbose
    };

    class log_entry_t
    {
    public:
        log_level_t log_level;
        std::string world_name;
        std::thread::id thread_id;
        std::string message;

        log_entry_t(
            log_level_t log_level,
            const std::string& world_name,
            std::thread::id thread_id,
            const std::string& message
        );

    };

    // * log() may be called from several threads simultaneously, so derived
    //   classes must manually implement synchronization.
    class base_logger_t
    {
    public:
        virtual ~base_logger_t() = default;
        virtual void log(const log_entry_t& entry) = 0;

    };

    class ostream_logger_t : public base_logger_t
    {
    public:
        std::ostream& stream;

        ostream_logger_t(std::ostream& stream);
        no_copy_construct_no_assignment(ostream_logger_t);
        virtual ~ostream_logger_t();

        virtual void log(const log_entry_t& entry) override;

    private:
        std::mutex mutex;

    };

    class csv_logger_t : public base_logger_t
    {
    public:
        csv_logger_t(const std::string& filename);
        no_copy_construct_no_assignment(csv_logger_t);
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
