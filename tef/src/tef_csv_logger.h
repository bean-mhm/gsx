#pragma once

#include <fstream>
#include <string>
#include <thread>
#include <mutex>

#include "tef_log.h"
#include "tef_utils.h"

namespace tef
{

    // Utility class for logging into a file in the CSV format
    class csv_logger_t
    {
    public:
        csv_logger_t(const std::string& filename);
        no_default_copy_move_constructor(csv_logger_t);

        bool is_open() const;

        void add(tef::log_level_t log_level,
            std::string world_name,
            std::thread::id thread_id,
            std::string message);

        void close();

    private:
        std::string filename;
        std::ofstream log_file;
        std::mutex mutex;

    };

}
