#include "app.h"

#include <iostream>
#include <memory>
#include <cstdint>

#include "gsx/gsx.h"

#include "components.h"
#include "systems.h"
#include "utils.h"

void app_t::run()
{
    ecs::world_t world(
        "Circles",
        ecs::log_level_t::verbose,
        std::make_shared<ecs::csv_logger_t>("./log.csv")
    );

    math::prng_t prng;

    std::vector<transform_t> transforms;
    std::vector<circle_t> circles;
    for (usize i = 0; i < 5; i++)
    {
        // add a transform component
        if (i != 0)
        {
            transform_t transform;
            transform.owner = i;
            transforms.push_back(transform);
        }

        // add a circle component
        circle_t circle;
        circle.owner = i;
        circle.radius = prng.next_f32(.05f, .2f);
        circles.push_back(circle);
    }

    world.add_system(std::make_shared<movement_system_t>(
        "movement", ecs::execution_scheme_t(0), transforms
    ));

    world.add_system(std::make_shared<render_system_t>(
        "circle renderer", ecs::execution_scheme_t(1), transforms, circles
    ));

    world.run(10, 8);
}
