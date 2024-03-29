#include "log.h"

#include <stdexcept>

namespace gsx::ecs
{

    log_entry_t::log_entry_t(
        log_level_t log_level,
        const std::string& world_name,
        std::thread::id thread_id,
        const std::string& message
    )
        : log_level(log_level),
        world_name(world_name),
        thread_id(thread_id),
        message(message)
    {}

    ostream_logger_t::ostream_logger_t(std::ostream& stream)
        : stream(stream)
    {
        if (!stream)
        {
            throw std::runtime_error(
                "the log output stream is in failure state"
            );
        }
    }

    ostream_logger_t::~ostream_logger_t()
    {
        std::scoped_lock(mutex);
    }

    void ostream_logger_t::log(const log_entry_t& entry)
    {
        std::scoped_lock(mutex);

        if (!stream)
        {
            throw std::runtime_error(
                "the log output stream is in failure state"
            );
        }

        stream << std::format("{} | ", str::from_time());

        switch (entry.log_level)
        {
        case log_level_t::error:
            stream << "E | ";
            break;
        case log_level_t::warning:
            stream << "W | ";
            break;
        case log_level_t::info:
            stream << "I | ";
            break;
        case log_level_t::verbose:
            stream << "V | ";
            break;
        default:
            break;
        }

        stream << std::format("{} | ", entry.world_name);
        stream << entry.thread_id << " | ";
        stream << entry.message << '\n';

        stream.flush();
    }

    csv_logger_t::csv_logger_t(const std::string& filename)
        : filename(filename)
    {
        log_file.open(filename, std::ofstream::out | std::ofstream::trunc);
        if (!log_file.is_open())
        {
            throw std::runtime_error(std::format(
                "log file \"{}\" couldn't be created/opened",
                filename
            ));
        }
        else
        {
            log_file << "time,log_level,world_name,thread_id,message\n";
            log_file.flush();
        }
    }

    csv_logger_t::~csv_logger_t()
    {
        std::scoped_lock(mutex);
    }

    void csv_logger_t::log(const log_entry_t& entry)
    {
        std::scoped_lock(mutex);

        if (!log_file)
        {
            throw std::runtime_error(std::format(
                "the log file stream \"{}\" is in failure state",
                filename
            ));
        }

        // replace " with "" because CSV
        std::string world_name_copy = str::replace(
            entry.world_name,
            "\"",
            "\"\""
        );
        std::string message_copy = str::replace(
            entry.message,
            "\"",
            "\"\""
        );

        log_file << '"' << str::from_time() << "\",";

        switch (entry.log_level)
        {
        case log_level_t::error:
            log_file << "error,";
            break;
        case log_level_t::warning:
            log_file << "warning,";
            break;
        case log_level_t::info:
            log_file << "info,";
            break;
        case log_level_t::verbose:
            log_file << "verbose,";
            break;
        default:
            break;
        }

        log_file << '"' << world_name_copy << "\",";
        log_file << entry.thread_id << ',';
        log_file << '"' << message_copy << "\"\n";

        log_file.flush();
    }

    bool csv_logger_t::is_open() const
    {
        return log_file.is_open();
    }

    void csv_logger_t::close()
    {
        std::scoped_lock(mutex);
        log_file.close();
    }

}
