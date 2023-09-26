#include "transform.h"

namespace tef::math::transform
{

    void translate2D(
        const vec2& delta,
        mat3* out_m,
        mat3* out_minv,
        const bool overwrite
    )
    {
        if (out_m)
        {
            mat3 r({
                1, 0, delta.x,
                0, 1, delta.y,
                0, 0, 1 }
            );

            if (overwrite) *out_m = r;
            else *out_m = r * *out_m;
        }

        if (out_minv)
        {
            mat3 rinv({
                1, 0, -delta.x,
                0, 1, -delta.y,
                0, 0, 1 }
            );

            if (overwrite) *out_minv = rinv;
            else *out_minv = *out_minv * rinv;
        }
    }

    mat3 translate2D(const vec2& delta)
    {
        mat3 m;
        translate2D(delta, &m, nullptr, true);
        return m;
    }

    void translate3D(
        const vec3& delta,
        mat4* out_m,
        mat4* out_minv,
        const bool overwrite
    )
    {
        if (out_m)
        {
            mat4 r({
                1, 0, 0, delta.x,
                0, 1, 0, delta.y,
                0, 0, 1, delta.z,
                0, 0, 0, 1 }
            );

            if (overwrite) *out_m = r;
            else *out_m = r * *out_m;
        }

        if (out_minv)
        {
            mat4 rinv({
                1, 0, 0, -delta.x,
                0, 1, 0, -delta.y,
                0, 0, 1, -delta.z,
                0, 0, 0, 1 }
            );

            if (overwrite) *out_minv = rinv;
            else *out_minv = *out_minv * rinv;
        }
    }

    mat4 translate3D(const vec3& delta)
    {
        mat4 m;
        translate3D(delta, &m, nullptr, true);
        return m;
    }

    void scale2D(
        const vec2& fac,
        mat2* out_m,
        mat2* out_minv,
        const bool overwrite
    )
    {
        if (out_m)
        {
            mat2 r({
                fac.x, 0,
                0, fac.y }
            );

            if (overwrite) *out_m = r;
            else *out_m = r * *out_m;
        }

        if (out_minv)
        {
            mat2 rinv({
                1 / fac.x, 0,
                0, 1 / fac.y }
            );

            if (overwrite) *out_minv = rinv;
            else *out_minv = *out_minv * rinv;
        }
    }

    void scale2D(
        const vec2& fac,
        mat3* out_m,
        mat3* out_minv,
        const bool overwrite
    )
    {
        if (out_m)
        {
            mat3 r({
                fac.x, 0, 0,
                0, fac.y, 0,
                0, 0, 1 }
            );

            if (overwrite) *out_m = r;
            else *out_m = r * *out_m;
        }

        if (out_minv)
        {
            mat3 rinv({
                1 / fac.x, 0, 0,
                0, 1 / fac.y, 0,
                0, 0, 1 }
            );

            if (overwrite) *out_minv = rinv;
            else *out_minv = *out_minv * rinv;
        }
    }

    mat2 scale2D_2x2(const vec2& fac)
    {
        mat2 m;
        scale2D(fac, &m, nullptr, true);
        return m;
    }

    mat3 scale2D_3x3(const vec2& fac)
    {
        mat3 m;
        scale2D(fac, &m, nullptr, true);
        return m;
    }

    void scale3D(
        const vec3& fac,
        mat3* out_m,
        mat3* out_minv,
        const bool overwrite
    )
    {
        if (out_m)
        {
            mat3 r({
                fac.x, 0, 0,
                0, fac.y, 0,
                0, 0, fac.z }
            );

            if (overwrite) *out_m = r;
            else *out_m = r * *out_m;
        }

        if (out_minv)
        {
            mat3 rinv({
                1 / fac.x, 0, 0,
                0, 1 / fac.y, 0,
                0, 0, 1 / fac.z }
            );

            if (overwrite) *out_minv = rinv;
            else *out_minv = *out_minv * rinv;
        }
    }

    void scale3D(
        const vec3& fac,
        mat4* out_m,
        mat4* out_minv,
        const bool overwrite
    )
    {
        if (out_m)
        {
            mat4 r({
                fac.x, 0, 0, 0,
                0, fac.y, 0, 0,
                0, 0, fac.z, 0,
                0, 0, 0, 1 }
            );

            if (overwrite) *out_m = r;
            else *out_m = r * *out_m;
        }

        if (out_minv)
        {
            mat4 rinv({
                1 / fac.x, 0, 0, 0,
                0, 1 / fac.y, 0, 0,
                0, 0, 1 / fac.z, 0,
                0, 0, 0, 1 }
            );

            if (overwrite) *out_minv = rinv;
            else *out_minv = *out_minv * rinv;
        }
    }

    mat3 scale3D_3x3(const vec3& fac)
    {
        mat3 m;
        scale3D(fac, &m, nullptr, true);
        return m;
    }

    mat4 scale3D_4x4(const vec3& fac)
    {
        mat4 m;
        scale3D(fac, &m, nullptr, true);
        return m;
    }

    void rotate2D(
        float theta,
        mat2* out_m,
        mat2* out_minv,
        const bool overwrite
    )
    {
        float s = sin(theta);
        float c = cos(theta);

        if (out_m)
        {
            mat2 r({
                c, -s,
                s, c }
            );

            if (overwrite) *out_m = r;
            else *out_m = r * *out_m;
        }

        if (out_minv)
        {
            mat2 rinv({
                c, s,
                -s, c }
            );

            if (overwrite) *out_minv = rinv;
            else *out_minv = *out_minv * rinv;
        }
    }

    void rotate2D(
        float theta,
        mat3* out_m,
        mat3* out_minv,
        const bool overwrite
    )
    {
        float s = sin(theta);
        float c = cos(theta);

        if (out_m)
        {
            mat3 r({
                c, -s, 0,
                s, c, 0,
                0, 0, 1 }
            );

            if (overwrite) *out_m = r;
            else *out_m = r * *out_m;
        }

        if (out_minv)
        {
            mat3 rinv({
                c, s, 0,
                -s, c, 0,
                0, 0, 1 }
            );

            if (overwrite) *out_minv = rinv;
            else *out_minv = *out_minv * rinv;
        }
    }

    mat2 rotate2D_2x2(float theta)
    {
        mat2 m;
        rotate2D(theta, &m, nullptr, true);
        return m;
    }

    mat3 rotate2D_3x3(float theta)
    {
        mat3 m;
        rotate2D(theta, &m, nullptr, true);
        return m;
    }

    void rotate3D_x(
        float theta,
        mat3* out_m,
        mat3* out_minv,
        const bool overwrite
    )
    {
        float s = sin(theta);
        float c = cos(theta);

        mat3 r({
            1, 0, 0,
            0, c, -s,
            0, s, c }
        );

        if (out_m)
        {
            if (overwrite) *out_m = r;
            else *out_m = r * *out_m;
        }

        if (out_minv)
        {
            if (overwrite) *out_minv = transpose(r);
            else *out_minv = *out_minv * transpose(r);
        }
    }

    void rotate3D_x(
        float theta,
        mat4* out_m,
        mat4* out_minv,
        const bool overwrite
    )
    {
        float s = sin(theta);
        float c = cos(theta);

        mat4 r({
            1, 0, 0, 0,
            0, c, -s, 0,
            0, s, c, 0,
            0, 0, 0, 1 }
        );

        if (out_m)
        {
            if (overwrite) *out_m = r;
            else *out_m = r * *out_m;
        }

        if (out_minv)
        {
            if (overwrite) *out_minv = transpose(r);
            else *out_minv = *out_minv * transpose(r);
        }
    }

    mat3 rotate3D_x_3x3(float theta)
    {
        mat3 m;
        rotate3D_x(theta, &m, nullptr, true);
        return m;
    }

    mat4 rotate3D_x_4x4(float theta)
    {
        mat4 m;
        rotate3D_x(theta, &m, nullptr, true);
        return m;
    }

    void rotate3D_y(
        float theta,
        mat3* out_m,
        mat3* out_minv,
        const bool overwrite
    )
    {
        float s = sin(theta);
        float c = cos(theta);

        mat3 r({
            c, 0, s,
            0, 1, 0,
            -s, 0, c }
        );

        if (out_m)
        {
            if (overwrite) *out_m = r;
            else *out_m = r * *out_m;
        }

        if (out_minv)
        {
            if (overwrite) *out_minv = transpose(r);
            else *out_minv = *out_minv * transpose(r);
        }
    }

    void rotate3D_y(
        float theta,
        mat4* out_m,
        mat4* out_minv,
        const bool overwrite
    )
    {
        float s = sin(theta);
        float c = cos(theta);

        mat4 r({
            c, 0, s, 0,
            0, 1, 0, 0,
            -s, 0, c, 0,
            0, 0, 0, 1 }
        );

        if (out_m)
        {
            if (overwrite) *out_m = r;
            else *out_m = r * *out_m;
        }

        if (out_minv)
        {
            if (overwrite) *out_minv = transpose(r);
            else *out_minv = *out_minv * transpose(r);
        }
    }

    mat3 rotate3D_y_3x3(float theta)
    {
        mat3 m;
        rotate3D_y(theta, &m, nullptr, true);
        return m;
    }

    mat4 rotate3D_y_4x4(float theta)
    {
        mat4 m;
        rotate3D_y(theta, &m, nullptr, true);
        return m;
    }

    void rotate3D_z(
        float theta,
        mat3* out_m,
        mat3* out_minv,
        const bool overwrite
    )
    {
        float s = sin(theta);
        float c = cos(theta);

        mat3 r({
            c, -s, 0,
            s, c, 0,
            0, 0, 1 }
        );

        if (out_m)
        {
            if (overwrite) *out_m = r;
            else *out_m = r * *out_m;
        }

        if (out_minv)
        {
            if (overwrite) *out_minv = transpose(r);
            else *out_minv = *out_minv * transpose(r);
        }
    }

    void rotate3D_z(
        float theta,
        mat4* out_m,
        mat4* out_minv,
        const bool overwrite
    )
    {
        float s = sin(theta);
        float c = cos(theta);

        mat4 r({
            c, -s, 0, 0,
            s, c, 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1 }
        );

        if (out_m)
        {
            if (overwrite) *out_m = r;
            else *out_m = r * *out_m;
        }

        if (out_minv)
        {
            if (overwrite) *out_minv = transpose(r);
            else *out_minv = *out_minv * transpose(r);
        }
    }

    mat3 rotate3D_z_3x3(float theta)
    {
        mat3 m;
        rotate3D_z(theta, &m, nullptr, true);
        return m;
    }

    mat4 rotate3D_z_4x4(float theta)
    {
        mat4 m;
        rotate3D_z(theta, &m, nullptr, true);
        return m;
    }

    void rotate3D(
        float theta,
        const vec3& axis,
        mat3* out_m,
        mat3* out_minv,
        const bool overwrite
    )
    {
        vec3 a = normalize(axis);
        float s = sin(theta);
        float c = cos(theta);

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

        if (out_m)
        {
            if (overwrite) *out_m = r;
            else *out_m = r * *out_m;
        }

        if (out_minv)
        {
            if (overwrite) *out_minv = transpose(r);
            else *out_minv = *out_minv * transpose(r);
        }
    }

    void rotate3D(
        float theta,
        const vec3& axis,
        mat4* out_m,
        mat4* out_minv,
        const bool overwrite
    )
    {
        vec3 a = normalize(axis);
        float s = sin(theta);
        float c = cos(theta);

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

        if (out_m)
        {
            if (overwrite) *out_m = r;
            else *out_m = r * *out_m;
        }

        if (out_minv)
        {
            if (overwrite) *out_minv = transpose(r);
            else *out_minv = *out_minv * transpose(r);
        }
    }

    mat3 rotate3D_3x3(float theta, const vec3& axis)
    {
        mat3 m;
        rotate3D(theta, axis, &m, nullptr, true);
        return m;
    }

    mat4 rotate3D_4x4(float theta, const vec3& axis)
    {
        mat4 m;
        rotate3D(theta, axis, &m, nullptr, true);
        return m;
    }

    void lookat3D(
        const vec3& pos,
        const vec3& look,
        const vec3& up,
        mat4* out_m,
        mat4* out_minv,
        const bool overwrite
    )
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

        if (out_m)
        {
            if (overwrite) *out_m = inverse(cam_to_world);
            else *out_m = inverse(cam_to_world) * *out_m;
        }

        if (out_minv)
        {
            if (overwrite) *out_minv = cam_to_world;
            else *out_minv = *out_minv * cam_to_world;
        }
    }

    mat4 lookat3D(const vec3& pos, const vec3& look, const vec3& up)
    {
        mat4 m;
        lookat3D(pos, look, up, &m, nullptr, true);
        return m;
    }

}
