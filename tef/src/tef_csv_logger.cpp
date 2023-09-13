#include "tef_csv_logger.h"

namespace tef
{

    csv_logger_t::csv_logger_t(const std::string& filename)
        : filename(filename)
    {
        log_file.open(filename, std::ofstream::out | std::ofstream::trunc);
        if (!log_file.is_open())
        {
            throw std::runtime_error(str_format("Log file \"%s\" could not be created/opened.", filename.c_str()));
        }
        else
        {
            log_file << "time,log_level,world_name,thread_id,message\n";
            log_file.flush();
        }
    }

    bool csv_logger_t::is_open() const
    {
        return log_file.is_open();
    }

    void csv_logger_t::add(log_level_t log_level, std::string world_name, std::thread::id thread_id, std::string message)
    {
        std::scoped_lock(mutex);

        if (!log_file.is_open())
        {
            throw std::runtime_error(str_format("Log file \"%s\" has been closed.", filename.c_str()));
        }

        // Replace " with "" because CSV
        str_replace_in_place(world_name, "\"", "\"\"");
        str_replace_in_place(message, "\"", "\"\"");

        // time
        log_file << '"' << str_from_time() << "\",";

        // log_level
        switch (log_level)
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

        // world_name, thread_id, message
        log_file << '"' << world_name << "\",";
        log_file << thread_id << ',';
        log_file << '"' << message << "\"\n";

        // Flush
        log_file.flush();
    }

    void csv_logger_t::close()
    {
        std::scoped_lock(mutex);
        log_file.close();
    }

}
