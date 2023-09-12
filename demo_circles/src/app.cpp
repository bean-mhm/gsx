#include "app.h"

#include <iostream>
#include <string>
#include <cmath>

#include "tef.h"

#include "components.h"
#include "systems.h"

void tef_log_cb(
    tef::log_level_t log_level,
    const std::string& world_name,
    std::thread::id thread_id,
    const std::string& message)
{
    std::ostringstream o;
    const char* separator = " | ";
    
    switch (log_level)
    {
    case tef::log_level_t::error:
        o << "error" << separator;
        break;
    case tef::log_level_t::warn:
        o << "warn" << separator;
        break;
    case tef::log_level_t::info:
        o << "info" << separator;
        break;
    case tef::log_level_t::verbose:
        o << "verbose" << separator;
        break;
    default:
        break;
    }

    o << world_name << separator;
    o << thread_id << separator;
    o << message;

    std::cout << o.str() << std::endl;
}

void app_t::run()
{
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
        //world.add_system(std::make_shared<s_circle_renderer>("Circle Renderer"));
    }

    // Run
    world.run(10, .5);
}
