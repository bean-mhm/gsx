#include "systems.h"

// STD
#include <iostream>

// Internal
#include "utils.h"

// s_movement

movement_system_t::movement_system_t(
    const std::string& name,
    i32 update_order,
    bool run_on_caller_thread,
    std::vector<transform_t>& transforms
)
    : ecs::base_system_t(name, update_order, run_on_caller_thread),
    transforms(transforms)
{}

void movement_system_t::on_update(ecs::world_t& world, const ecs::world_t::iter_t& iter)
{
    for (usize i = 0; i < transforms.size(); i++)
    {
        transform_t& transform = transforms[i];
        f32 theta = .5f * (i + 1.f) * iter.time;
        transform.pos = 3.f * math::vec2(math::cos(theta), sin(theta));
    }
}

// s_circle_renderer

render_system_t::render_system_t(
    const std::string& name,
    i32 update_order,
    bool run_on_caller_thread,
    std::vector<transform_t>& transforms,
    std::vector<circle_t>& circles
)
    : ecs::base_system_t(name, update_order, run_on_caller_thread),
    transforms(transforms), circles(circles)
{}

void render_system_t::on_update(ecs::world_t& world, const ecs::world_t::iter_t& iter)
{
    // Clear the console
    clear_console();

    // Render (per-pixel shader)
    constexpr math::uvec2 res(30, 20);
    f32 px2uv = get_px2uv_ratio(res);
    for (i32 y = 0; y < res.y; y++)
    {
        for (i32 x = 0; x < res.x; x++)
        {
            // UV
            math::vec2 uv = screen_to_uv(math::uvec2(x, y), res);

            // Find the distance from the closest circle
            f32 dist = 1e9f;
            for (auto& circle : circles)
            {
                // See if there's a transform component with the same owner
                transform_t* transform = nullptr;
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
