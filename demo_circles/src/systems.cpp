#include "systems.h"

// STD
#include <iostream>

// TEF
#include "tef/math.h"

// Internal
#include "utils.h"

// s_movement

s_movement::s_movement(
    const std::string& name,
    int32_t update_order,
    bool run_on_caller_thread,
    std::vector<c_transform>& transforms
)
    : ecs::base_system_t(name, update_order, run_on_caller_thread),
    transforms(transforms)
{}

void s_movement::on_update(ecs::world_t& world, const ecs::world_t::iter_t& iter)
{
    for (size_t i = 0; i < transforms.size(); i++)
    {
        c_transform& transform = transforms[i];
        float theta = .5f * (i + 1.f) * iter.time;
        transform.pos = 3.f * math::vec2(math::cos(theta), sin(theta));
    }
}

// s_circle_renderer

s_circle_renderer::s_circle_renderer(
    const std::string& name,
    int32_t update_order,
    bool run_on_caller_thread,
    std::vector<c_transform>& transforms,
    std::vector<c_circle>& circles
)
    : ecs::base_system_t(name, update_order, run_on_caller_thread),
    transforms(transforms), circles(circles)
{}

void s_circle_renderer::on_update(ecs::world_t& world, const ecs::world_t::iter_t& iter)
{
    // Clear the console
    clear_console();

    // Render (per-pixel shader)
    constexpr math::uvec2 res(30, 20);
    float px2uv = get_px2uv_ratio(res);
    for (int y = 0; y < res.y; y++)
    {
        for (int x = 0; x < res.x; x++)
        {
            // UV
            math::vec2 uv = screen_to_uv(math::uvec2(x, y), res);

            // Find the distance from the closest circle
            float dist = 1e9f;
            for (auto& circle : circles)
            {
                // See if there's a transform component with the same owner
                c_transform* transform = nullptr;
                for (auto& t : transforms)
                {
                    if (t.owner == circle.owner)
                    {
                        transform = &t;
                        break;
                    }
                }

                if (transform)
                {
                    // Use transform.pos as the center
                    dist = math::min(dist, sd_circle(uv, transform->pos, circle.radius));
                }
                else
                {
                    // Use the origin as the center
                    dist = math::min(dist, sd_circle(uv, math::vec2(0), circle.radius));
                }
            }

            // Print
            std::cout << (dist < px2uv ? 'o' : ' ') << " ";
        }
        std::cout << '\n';
    }
    std::cout.flush();
}
