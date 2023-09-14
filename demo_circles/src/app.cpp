#include "app.h"

// STD
#include <iostream>
#include <string>
#include <thread>
#include <memory>
#include <cstdint>

// TEF
#include "tef.h"

// Internal
#include "components.h"
#include "systems.h"
#include "utils.h"

void app_t::run()
{
    // Create a world
    tef::world_t world(
        "Circles",
        std::make_shared<tef::csv_logger_t>("./log.csv")
    );

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
        world.add_system(std::make_shared<s_movement>("Movement", 0));
        world.add_system(std::make_shared<s_circle_renderer>("Circle Renderer", 1));
    }

    // Run
    world.run(10, 5);
}
