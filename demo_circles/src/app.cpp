#include <iostream>
#include <string>
#include <cmath>

#include "tef.h"

#include "components.h"
#include "systems.h"

class app_t
{
public:
    void run()
    {
        // Create a world
        tef::world_t world("Test");

        // Add entities
        for (size_t i = 0; i < 5; i++)
        {
            // Add a new entity
            tef::entity_t entity = world.add_entity();

            // Add a transform component
            if (i != 0)
            {
                c_transform transform;
                transform.owner = entity;
                world.add_component(transform);
            }

            // Add a circle component
            c_circle circle;
            circle.owner = entity;
            circle.radius = world.prng.next_float(.05f, .15f);
            world.add_component(circle);
        }

        // Add systems
        world.add_system(std::make_shared<s_movement>("Movement"));
        world.add_system(std::make_shared<s_circle_renderer>("Circle Renderer"));

        // Run
        world.run(20);
    }

private:

};
