#include "app.h"

#include <iostream>
#include <fstream>
#include <string>
#include <cstdint>

#include "tef.h"

#include "components.h"
#include "systems.h"
#include "utils.h"

static std::ofstream log_file;

// Log callback, writes to log_file in the CSV format
void tef_log_cb(
    tef::log_level_t log_level,
    const std::string& world_name,
    std::thread::id thread_id,
    const std::string& message)
{
    // Replace " with "" because CSV
    std::string world_name_copy = str_replace(world_name, "\"", "\"\"");
    std::string message_copy = str_replace(message, "\"", "\"\"");

    // time
    log_file << '"' << str_from_time() << "\",";

    // log_level
    switch (log_level)
    {
    case tef::log_level_t::error:
        log_file << "error,";
        break;
    case tef::log_level_t::warning:
        log_file << "warning,";
        break;
    case tef::log_level_t::info:
        log_file << "info,";
        break;
    case tef::log_level_t::verbose:
        log_file << "verbose,";
        break;
    default:
        break;
    }

    // world_name, thread_id, message
    log_file << '"' << world_name_copy << "\",";
    log_file << thread_id << ',';
    log_file << '"' << message_copy << "\"\n";

    // Flush
    log_file.flush();
}

void app_t::run()
{
    // Log file
    log_file.open("./log.csv", std::ofstream::out | std::ofstream::trunc);
    if (!log_file.is_open())
    {
        throw std::runtime_error("A log file could not be created/opened.");
    }
    else
    {
        log_file << "time,log_level,world_name,thread_id,message\n";
        log_file.flush();
    }

    // Create a world
    tef::world_t world("Circles", tef_log_cb);

    // Add entities
    for (size_t i = 0; i < 5; i++)
    {
        // Add a transform component
        if (i != 0)
        {
            c_transform transform;
            transform.owner = i;
            world.add_component_of_type<c_transform>(transform);
        }

        // Add a circle component
        c_circle circle;
        circle.owner = i;
        circle.radius = world.prng.next_float(.05f, .2f);
        world.add_component_of_type<c_circle>(circle);
    }

    // Add systems
    {
        world.add_system(std::make_shared<s_movement>("Movement"));
        world.add_system(std::make_shared<s_circle_renderer>("Circle Renderer"));
    }

    // Run
    world.run(10, 10);
}
