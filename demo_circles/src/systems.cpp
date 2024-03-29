#include "systems.h"

#include <iostream>

#include "utils.h"

movement_system_t::movement_system_t(
    const std::string& name,
    const ecs::execution_scheme_t& exec_scheme,
    std::vector<transform_t>& transforms
)
    : ecs::base_system_t(name, exec_scheme),
    transforms(transforms)
{}

void movement_system_t::on_update(
    ecs::world_t& world,
    const ecs::iteration_t& iter
)
{
    for (usize i = 0; i < transforms.size(); i++)
    {
        transform_t& transform = transforms[i];
        f32 theta = .5f * (i + 1.f) * iter.time;
        transform.pos = 3.f * math::vec2(math::cos(theta), sin(theta));
    }
}

render_system_t::render_system_t(
    const std::string& name,
    const ecs::execution_scheme_t& exec_scheme,
    std::vector<transform_t>& transforms,
    std::vector<circle_t>& circles
)
    : ecs::base_system_t(name, exec_scheme),
    transforms(transforms), circles(circles)
{}

void render_system_t::on_update(
    ecs::world_t& world,
    const ecs::iteration_t& iter
)
{
    clear_console();

    // render (per-pixel shader)
    constexpr math::uvec2 res(30, 20);
    f32 px2uv = get_px2uv_ratio(res);
    for (i32 y = 0; y < res.y; y++)
    {
        for (i32 x = 0; x < res.x; x++)
        {
            math::vec2 uv = screen_to_uv(math::uvec2(x, y), res);

            // distance from the closest circle
            f32 dist = 1e9f;
            for (auto& circle : circles)
            {
                // see if there's a transform component with the same owner
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
                    // use transform.pos as the center
                    dist = math::min(
                        dist,
                        sd_circle(uv, transform->pos, circle.radius)
                    );
                }
                else
                {
                    // use the origin as the center
                    dist = math::min(
                        dist,
                        sd_circle(uv, math::vec2(0), circle.radius)
                    );
                }
            }

            std::cout << (dist < px2uv ? 'o' : ' ') << " ";
        }
        std::cout << '\n';
    }
    std::cout.flush();
}
