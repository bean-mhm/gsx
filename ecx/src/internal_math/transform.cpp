#include "transform.h"

namespace ecx::math::transform
{

    mat3 translate_2d_h(const vec2& delta, mat3* out_minv)
    {
        if (out_minv)
        {
            *out_minv = mat3({
                1, 0, -delta.x,
                0, 1, -delta.y,
                0, 0, 1 }
            );
        }

        return mat3({
            1, 0, delta.x,
            0, 1, delta.y,
            0, 0, 1 }
        );
    }

    mat4 translate_3d_h(const vec3& delta, mat4* out_minv)
    {
        if (out_minv)
        {
            *out_minv = mat4({
                1, 0, 0, -delta.x,
                0, 1, 0, -delta.y,
                0, 0, 1, -delta.z,
                0, 0, 0, 1 }
            );
        }

        return mat4({
            1, 0, 0, delta.x,
            0, 1, 0, delta.y,
            0, 0, 1, delta.z,
            0, 0, 0, 1 }
        );
    }

    mat2 scale_2d(const vec2& fac, mat2* out_minv)
    {
        if (out_minv)
        {
            *out_minv = mat2({
                1 / fac.x, 0,
                0, 1 / fac.y }
            );
        }

        return mat2({
            fac.x, 0,
            0, fac.y }
        );
    }

    mat3 scale_2d_h(const vec2& fac, mat3* out_minv)
    {
        if (out_minv)
        {
            *out_minv = mat3({
                1 / fac.x, 0, 0,
                0, 1 / fac.y, 0,
                0, 0, 1 }
            );
        }

        return mat3({
            fac.x, 0, 0,
            0, fac.y, 0,
            0, 0, 1 }
        );
    }

    mat3 scale_3d(const vec3& fac, mat3* out_minv)
    {
        if (out_minv)
        {
            *out_minv = mat3({
                1 / fac.x, 0, 0,
                0, 1 / fac.y, 0,
                0, 0, 1 / fac.z }
            );
        }

        return mat3({
            fac.x, 0, 0,
            0, fac.y, 0,
            0, 0, fac.z }
        );
    }

    mat4 scale_3d_h(const vec3& fac, mat4* out_minv)
    {
        if (out_minv)
        {
            *out_minv = mat4({
                1 / fac.x, 0, 0, 0,
                0, 1 / fac.y, 0, 0,
                0, 0, 1 / fac.z, 0,
                0, 0, 0, 1 }
            );
        }

        return mat4({
            fac.x, 0, 0, 0,
            0, fac.y, 0, 0,
            0, 0, fac.z, 0,
            0, 0, 0, 1 }
        );
    }

    mat2 rotate_2d(f32 angle, mat2* out_minv)
    {
        f32 s = sin(angle);
        f32 c = cos(angle);

        if (out_minv)
        {
            *out_minv = mat2({
                c, s,
                -s, c }
            );
        }

        return mat2({
            c, -s,
            s, c }
        );
    }

    mat3 rotate_2d_h(f32 angle, mat3* out_minv)
    {
        f32 s = sin(angle);
        f32 c = cos(angle);

        if (out_minv)
        {
            *out_minv = mat3({
                c, s, 0,
                -s, c, 0,
                0, 0, 1 }
            );
        }

        return mat3({
            c, -s, 0,
            s, c, 0,
            0, 0, 1 }
        );
    }

    mat3 rotate_3d_x(f32 angle, mat3* out_minv)
    {
        f32 s = sin(angle);
        f32 c = cos(angle);

        mat3 r({
            1, 0, 0,
            0, c, -s,
            0, s, c }
        );

        if (out_minv)
            *out_minv = transpose(r);

        return r;
    }

    mat4 rotate_3d_x_h(f32 angle, mat4* out_minv)
    {
        f32 s = sin(angle);
        f32 c = cos(angle);

        mat4 r({
            1, 0, 0, 0,
            0, c, -s, 0,
            0, s, c, 0,
            0, 0, 0, 1 }
        );

        if (out_minv)
            *out_minv = transpose(r);

        return r;
    }

    mat3 rotate_3d_y(f32 angle, mat3* out_minv)
    {
        f32 s = sin(angle);
        f32 c = cos(angle);

        mat3 r({
            c, 0, s,
            0, 1, 0,
            -s, 0, c }
        );

        if (out_minv)
            *out_minv = transpose(r);

        return r;
    }

    mat4 rotate_3d_y_h(f32 angle, mat4* out_minv)
    {
        f32 s = sin(angle);
        f32 c = cos(angle);

        mat4 r({
            c, 0, s, 0,
            0, 1, 0, 0,
            -s, 0, c, 0,
            0, 0, 0, 1 }
        );

        if (out_minv)
            *out_minv = transpose(r);

        return r;
    }

    mat3 rotate_3d_z(f32 angle, mat3* out_minv)
    {
        f32 s = sin(angle);
        f32 c = cos(angle);

        mat3 r({
            c, -s, 0,
            s, c, 0,
            0, 0, 1 }
        );

        if (out_minv)
            *out_minv = transpose(r);

        return r;
    }

    mat4 rotate_3d_z_h(f32 angle, mat4* out_minv)
    {
        f32 s = sin(angle);
        f32 c = cos(angle);

        mat4 r({
            c, -s, 0, 0,
            s, c, 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1 }
        );

        if (out_minv)
            *out_minv = transpose(r);

        return r;
    }

    mat3 rotate_3d(f32 angle, const vec3& axis, mat3* out_minv)
    {
        vec3 a = normalize(axis);
        f32 s = sin(angle);
        f32 c = cos(angle);

        mat3 r;

        // Compute rotation of first basis vector
        r(0, 0) = a.x * a.x + (1 - a.x * a.x) * c;
        r(0, 1) = a.x * a.y * (1 - c) - a.z * s;
        r(0, 2) = a.x * a.z * (1 - c) + a.y * s;

        // Second basis vector
        r(1, 0) = a.x * a.y * (1 - c) + a.z * s;
        r(1, 1) = a.y * a.y + (1 - a.y * a.y) * c;
        r(1, 2) = a.y * a.z * (1 - c) - a.x * s;

        // Third basis vector
        r(2, 0) = a.x * a.z * (1 - c) - a.y * s;
        r(2, 1) = a.y * a.z * (1 - c) + a.x * s;
        r(2, 2) = a.z * a.z + (1 - a.z * a.z) * c;

        if (out_minv)
            *out_minv = transpose(r);

        return r;
    }

    mat4 rotate_3d_h(f32 angle, const vec3& axis, mat4* out_minv)
    {
        vec3 a = normalize(axis);
        f32 s = sin(angle);
        f32 c = cos(angle);

        mat4 r;

        // Compute rotation of first basis vector
        r(0, 0) = a.x * a.x + (1 - a.x * a.x) * c;
        r(0, 1) = a.x * a.y * (1 - c) - a.z * s;
        r(0, 2) = a.x * a.z * (1 - c) + a.y * s;
        r(0, 3) = 0;

        // Second basis vector
        r(1, 0) = a.x * a.y * (1 - c) + a.z * s;
        r(1, 1) = a.y * a.y + (1 - a.y * a.y) * c;
        r(1, 2) = a.y * a.z * (1 - c) - a.x * s;
        r(1, 3) = 0;

        // Third basis vector
        r(2, 0) = a.x * a.z * (1 - c) - a.y * s;
        r(2, 1) = a.y * a.z * (1 - c) + a.x * s;
        r(2, 2) = a.z * a.z + (1 - a.z * a.z) * c;
        r(2, 3) = 0;

        if (out_minv)
            *out_minv = transpose(r);

        return r;
    }

    mat4 lookat_3d_h(const vec3& pos, const vec3& look, const vec3& up, mat4* out_minv)
    {
        mat4 cam_to_world;

        // Initialize fourth column of viewing matrix
        cam_to_world(0, 3) = pos.x;
        cam_to_world(1, 3) = pos.y;
        cam_to_world(2, 3) = pos.z;
        cam_to_world(3, 3) = 1;

        // Initialize first three columns of viewing matrix
        vec3 dir = normalize(look - pos);
        vec3 right = normalize(cross(normalize(up), dir));
        vec3 new_up = cross(dir, right);
        cam_to_world(0, 0) = right.x;
        cam_to_world(1, 0) = right.y;
        cam_to_world(2, 0) = right.z;
        cam_to_world(3, 0) = 0.;
        cam_to_world(0, 1) = new_up.x;
        cam_to_world(1, 1) = new_up.y;
        cam_to_world(2, 1) = new_up.z;
        cam_to_world(3, 1) = 0.;
        cam_to_world(0, 2) = dir.x;
        cam_to_world(1, 2) = dir.y;
        cam_to_world(2, 2) = dir.z;
        cam_to_world(3, 2) = 0.;

        if (out_minv)
            *out_minv = cam_to_world;

        return inverse(cam_to_world);
    }

}
