#include "systems.h"

#include <iostream>

// s_movement

s_movement::s_movement(const std::string& name)
    : tef::base_system_t(name)
{}

s_movement::~s_movement()
{}

void s_movement::on_update(tef::world_t& world, const tef::world_iteration_t& iter)
{
    // Get all transform components
    c_transform* components; size_t size;
    world.get_components_of_type<c_transform>(components, size);

    // Iterate and update
    for (size_t i = 0; i < size; i++)
    {
        c_transform& transform = components[i];

        float theta = .5f * (i + 1.f) * iter.elapsed;
        transform.pos = 3.f * glm::vec2(std::cos(theta), std::sin(theta));
    }
}

// s_circle_renderer

s_circle_renderer::s_circle_renderer(const std::string& name)
    : tef::base_system_t(name)
{}

s_circle_renderer::~s_circle_renderer()
{}

void s_circle_renderer::on_update(tef::world_t& world, const tef::world_iteration_t& iter)
{
    // Clear the screen
#if defined(_WIN32) || defined(_WIN64) || defined(WINDOWS)
    std::system("cls");
#else
    std::system("clear");
#endif

    // Get all circle components
    c_circle* components; size_t size;
    world.get_components_of_type<c_circle>(components, size);

    // Render (per-pixel shader)
    constexpr glm::uvec2 res(30, 20);
    float px2uv = get_px2uv_ratio(res);
    for (int y = 0; y < res.y; y++)
    {
        for (int x = 0; x < res.x; x++)
        {
            // UV
            glm::vec2 uv = screen_to_uv(glm::uvec2(x, y), res);

            // Minimum distance from the closest circle
            float dist = 1e9f;
            for (size_t i = 0; i < size; i++)
            {
                c_circle& circle = components[i];
                c_transform* transform = world.get_component_of_type_owned_by<c_transform>(circle.owner);
                if (transform)
                {
                    // Use transform.pos as the center
                    dist = std::min(dist, sd_circle(uv, transform->pos, circle.radius));
                }
                else
                {
                    // Use the origin as the center
                    dist = std::min(dist, sd_circle(uv, glm::vec2(0), circle.radius));
                }
            }

            // Print
            std::cout << (dist < px2uv ? 'o' : ' ') << " ";
        }
        std::cout << '\n';
    }
    std::cout.flush();
}
