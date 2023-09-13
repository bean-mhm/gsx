#include "app.h"

#include <iostream>
#include <string>
#include <thread>
#include <memory>
#include <cstdint>

#include "tef.h"

#include "components.h"
#include "systems.h"
#include "utils.h"

static std::shared_ptr<tef::csv_logger_t> logger;

void cb_log(
    tef::log_level_t log_level,
    const std::string& world_name,
    std::thread::id thread_id,
    const std::string& message)
{
    logger->add(log_level, world_name, thread_id, message);
}

void app_t::run()
{
    // Log file
    logger = std::make_shared<tef::csv_logger_t>("./log.csv");

    // Create a world
    tef::world_t world("Circles", cb_log);

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
