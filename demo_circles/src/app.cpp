#include "app.h"

// STD
#include <iostream>
#include <memory>
#include <cstdint>

// GSX
#include "gsx/gsx.h"

// Internal
#include "components.h"
#include "systems.h"
#include "utils.h"

void app_t::run()
{
    // Create a world
    ecs::world_t world(
        "Circles",
        ecs::log_level_t::verbose,
        std::make_shared<ecs::csv_logger_t>("./log.csv")
    );

    // Create a PRNG
    math::prng_t prng;

    // Add components
    std::vector<transform_t> transforms;
    std::vector<circle_t> circles;
    for (usize i = 0; i < 5; i++)
    {
        // Add a transform component
        if (i != 0)
        {
            transform_t transform;
            transform.owner = i;
            transforms.push_back(transform);
        }

        // Add a circle component
        circle_t circle;
        circle.owner = i;
        circle.radius = prng.next_f32(.05f, .2f);
        circles.push_back(circle);
    }

    // Add systems
    {
        world.add_system(std::make_shared<movement_system_t>(
            "Movement", 0, false, transforms
        ));

        world.add_system(std::make_shared<render_system_t>(
            "Circle Renderer", 1, false, transforms, circles
        ));
    }

    // Run
    world.run(10, 8);
}
