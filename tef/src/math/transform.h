#pragma once

#include "vec3.h"
#include "matrix.h"
#include "ray.h"
#include "bounds3.h"
#include "utils.h"

namespace tef::math::transform
{

    // Note: In the functions below, the overwrite argument means whether the output matrices
    // should be overwritten by the newly generated matrices, or just multiplied. If overwrite is
    // false, the output inverse matrix is updated according to the fact that
    // inv(A * B) = inv(B) * inv(A).

    // 2D translation matrix (3x3)
    constexpr void translate2D(
        const vec2& delta,
        mat3* out_m,
        mat3* out_minv = nullptr,
        const bool overwrite = false
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

    // 3D translation matrix (4x4)
    constexpr void translate3D(
        const vec3& delta,
        mat4* out_m,
        mat4* out_minv = nullptr,
        const bool overwrite = false
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

    // 2D scaling matrix (2x2)
    constexpr void scale2D(
        const vec2& fac,
        mat2* out_m,
        mat2* out_minv = nullptr,
        const bool overwrite = false
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

    // 2D scaling matrix (3x3)
    constexpr void scale2D(
        const vec2& fac,
        mat3* out_m,
        mat3* out_minv = nullptr,
        const bool overwrite = false
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

    // 3D scaling matrix (3x3)
    constexpr void scale3D(
        const vec3& fac,
        mat3* out_m,
        mat3* out_minv = nullptr,
        const bool overwrite = false
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

    // 3D scaling matrix (4x4)
    constexpr void scale3D(
        const vec3& fac,
        mat4* out_m,
        mat4* out_minv = nullptr,
        const bool overwrite = false
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

    // 2D rotation matrix (2x2)
    constexpr void rotate2D(
        float theta,
        mat2* out_m,
        mat2* out_minv = nullptr,
        const bool overwrite = false
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

    // 2D rotation matrix (3x3)
    constexpr void rotate2D(
        float theta,
        mat3* out_m,
        mat3* out_minv = nullptr,
        const bool overwrite = false
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

    // 3D rotation matrix around the X axis (left-handed) (3x3)
    constexpr void rotate3D_x(
        float theta,
        mat3* out_m,
        mat3* out_minv = nullptr,
        const bool overwrite = false
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

    // 3D rotation matrix around the X axis (left-handed) (4x4)
    constexpr void rotate3D_x(
        float theta,
        mat4* out_m,
        mat4* out_minv = nullptr,
        const bool overwrite = false
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

    // 3D rotation matrix around the Y axis (left-handed) (3x3)
    constexpr void rotate3D_y(
        float theta,
        mat3* out_m,
        mat3* out_minv = nullptr,
        const bool overwrite = false
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

    // 3D rotation matrix around the Y axis (left-handed) (4x4)
    constexpr void rotate3D_y(
        float theta,
        mat4* out_m,
        mat4* out_minv = nullptr,
        const bool overwrite = false
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

    // 3D rotation matrix around the Z axis (left-handed) (3x3)
    constexpr void rotate3D_z(
        float theta,
        mat3* out_m,
        mat3* out_minv = nullptr,
        const bool overwrite = false
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

    // 3D rotation matrix around the Z axis (left-handed) (4x4)
    constexpr void rotate3D_z(
        float theta,
        mat4* out_m,
        mat4* out_minv = nullptr,
        const bool overwrite = false
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

    // 3D rotation matrix around an arbitrary axis (left-handed) (3x3)
    constexpr void rotate3D(
        float theta,
        const vec3& axis,
        mat3* out_m,
        mat3* out_minv = nullptr,
        const bool overwrite = false
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

    // 3D rotation matrix around an arbitrary axis (left-handed) (4x4)
    constexpr void rotate3D(
        float theta,
        const vec3& axis,
        mat4* out_m,
        mat4* out_minv = nullptr,
        const bool overwrite = false
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

    // 3D transformation from a left-handed viewing coordinate system where the camera is at the
    // origin looking along the +z axis, where the +y axis is along the up direction (4x4)
    constexpr void lookat3D(
        const vec3& pos,
        const vec3& look,
        const vec3& up,
        mat4* out_m,
        mat4* out_minv = nullptr,
        const bool overwrite = false
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

    // Transform a 2D point using a 2x2 matrix.
    constexpr vec2 apply_point2D(const mat2& m, const vec2& p)
    {
        return vec2(m * mat2x1(p));
    }

    // Transform a 2D point using a 3x3 matrix.
    constexpr vec2 apply_point2D(const mat3& m, const vec2& p)
    {
        vec3 r(m * mat3x1(vec3(p, 1)));
        if (r.z == 0)
            return r.permute(0, 1);
        else
            return r.permute(0, 1) / r.z;
    }

    // Transform a 3D point using a 3x3 matrix.
    constexpr vec3 apply_point3D(const mat3& m, const vec3& p)
    {
        return vec3(m * mat3x1(p));
    }

    // Transform a 3D point using a 4x4 matrix.
    constexpr vec3 apply_point3D(const mat4& m, const vec3& p)
    {
        vec4 r(m * mat4x1(vec4(p, 1)));
        if (r.w == 0)
            return r.permute(0, 1, 2);
        else
            return r.permute(0, 1, 2) / r.w;
    }

    // Transform a 2D vector using a 2x2 matrix.
    constexpr vec2 apply_vector2D(const mat2& m, const vec2& v)
    {
        return vec2(m * mat2x1(v));
    }

    // Transform a 2D vector using a 3x3 matrix.
    constexpr vec2 apply_vector2D(const mat3& m, const vec2& v)
    {
        return vec2(m.sub<2>() * mat2x1(v));
    }

    // Transform a 3D vector using a 3x3 matrix.
    constexpr vec3 apply_vector3D(const mat3& m, const vec3& v)
    {
        return vec3(m * mat3x1(v));
    }

    // Transform a 3D vector using a 4x4 matrix.
    constexpr vec3 apply_vector3D(const mat4& m, const vec3& v)
    {
        return vec3(m.sub<3>() * mat3x1(v));
    }

    // Transform a 2D normal vector using a 2x2 matrix.
    // Note: You need to input the inverted version of your transformation matrix. Use the
    // inverse() function if you only have the original transformation matrix.
    constexpr vec2 apply_normal2D(const mat2& minv, const vec2& n)
    {
        return vec2(transpose(minv) * mat2x1(n));
    }

    // Transform a 2D normal vector using a 3x3 matrix.
    // Note: You need to input the inverted version of your transformation matrix. Use the
    // inverse() function if you only have the original transformation matrix.
    constexpr vec2 apply_normal2D(const mat3& minv, const vec2& n)
    {
        return vec2(transpose(minv.sub<2>()) * mat2x1(n));
    }

    // Transform a 3D normal vector using a 3x3 matrix.
    // Note: You need to input the inverted version of your transformation matrix. Use the
    // inverse() function if you only have the original transformation matrix.
    constexpr vec3 apply_normal3D(const mat3& minv, const vec3& n)
    {
        return vec3(transpose(minv) * mat3x1(n));
    }

    // Transform a 3D normal vector using a 4x4 matrix.
    // Note: You need to input the inverted version of your transformation matrix. Use the
    // inverse() function if you only have the original transformation matrix.
    constexpr vec3 apply_normal3D(const mat4& minv, const vec3& n)
    {
        return vec3(transpose(minv.sub<3>()) * mat3x1(n));
    }

    // Transform a 3D ray using a 3x3 matrix.
    constexpr ray_t apply_ray3D(const mat3& m, ray_t r)
    {
        r.o = apply_point3D(m, r.o);
        r.d = apply_vector3D(m, r.d);
        return r;
    }

    // Transform a 3D ray using a 4x4 matrix.
    constexpr ray_t apply_ray3D(const mat4& m, ray_t r)
    {
        r.o = apply_point3D(m, r.o);
        r.d = apply_vector3D(m, r.d);
        return r;
    }

    // Transform a 2D AABB using a 2x2 matrix.
    constexpr bounds2 apply_bounds2D(const mat2& m, const bounds2& b)
    {
        bounds2 r(apply_point2D(m, b.pmin));
        r = union_(r, apply_point2D(m, vec2(b.pmin.x, b.pmax.y)));
        r = union_(r, apply_point2D(m, vec2(b.pmax.x, b.pmin.y)));
        r = union_(r, apply_point2D(m, b.pmax));
        return r;
    }

    // Transform a 2D AABB using a 3x3 matrix.
    constexpr bounds2 apply_bounds2D(const mat3& m, const bounds2& b)
    {
        bounds2 r(apply_point2D(m, b.pmin));
        r = union_(r, apply_point2D(m, vec2(b.pmin.x, b.pmax.y)));
        r = union_(r, apply_point2D(m, vec2(b.pmax.x, b.pmin.y)));
        r = union_(r, apply_point2D(m, b.pmax));
        return r;
    }

    // Transform a 3D AABB using a 3x3 matrix.
    constexpr bounds3 apply_bounds3D(const mat3& m, const bounds3& b)
    {
        bounds3 r(apply_point3D(m, b.pmin));
        r = union_(r, apply_point3D(m, vec3(b.pmax.x, b.pmin.y, b.pmin.z)));
        r = union_(r, apply_point3D(m, vec3(b.pmin.x, b.pmax.y, b.pmin.z)));
        r = union_(r, apply_point3D(m, vec3(b.pmin.x, b.pmin.y, b.pmax.z)));
        r = union_(r, apply_point3D(m, vec3(b.pmin.x, b.pmax.y, b.pmax.z)));
        r = union_(r, apply_point3D(m, vec3(b.pmax.x, b.pmax.y, b.pmin.z)));
        r = union_(r, apply_point3D(m, vec3(b.pmax.x, b.pmin.y, b.pmax.z)));
        r = union_(r, apply_point3D(m, b.pmax));
        return r;
    }

    // Transform a 3D AABB using a 4x4 matrix.
    constexpr bounds3 apply_bounds3D(const mat4& m, const bounds3& b)
    {
        bounds3 r(apply_point3D(m, b.pmin));
        r = union_(r, apply_point3D(m, vec3(b.pmax.x, b.pmin.y, b.pmin.z)));
        r = union_(r, apply_point3D(m, vec3(b.pmin.x, b.pmax.y, b.pmin.z)));
        r = union_(r, apply_point3D(m, vec3(b.pmin.x, b.pmin.y, b.pmax.z)));
        r = union_(r, apply_point3D(m, vec3(b.pmin.x, b.pmax.y, b.pmax.z)));
        r = union_(r, apply_point3D(m, vec3(b.pmax.x, b.pmax.y, b.pmin.z)));
        r = union_(r, apply_point3D(m, vec3(b.pmax.x, b.pmin.y, b.pmax.z)));
        r = union_(r, apply_point3D(m, b.pmax));
        return r;
    }

    // Check if a 2D transformation matrix has a scaling term in it. (2x2)
    constexpr bool has_scale2D(const mat2& m)
    {
        float la2 = length_squared(apply_vector2D(m, vec2(1, 0)));
        float lb2 = length_squared(apply_vector2D(m, vec2(0, 1)));
        return abs(la2 - 1.f) > .001f || abs(lb2 - 1.f) > .001f;
    }

    // Check if a 2D transformation matrix has a scaling term in it. (3x3)
    constexpr bool has_scale2D(const mat3& m)
    {
        float la2 = length_squared(apply_vector2D(m, vec2(1, 0)));
        float lb2 = length_squared(apply_vector2D(m, vec2(0, 1)));
        return abs(la2 - 1.f) > .001f || abs(lb2 - 1.f) > .001f;
    }

    // Check if a 3D transformation matrix has a scaling term in it. (3x3)
    constexpr bool has_scale3D(const mat3& m)
    {
        float la2 = length_squared(apply_vector3D(m, vec3(1, 0, 0)));
        float lb2 = length_squared(apply_vector3D(m, vec3(0, 1, 0)));
        float lc2 = length_squared(apply_vector3D(m, vec3(0, 0, 1)));
        return
            abs(la2 - 1.f) > .001f ||
            abs(lb2 - 1.f) > .001f ||
            abs(lc2 - 1.f) > .001f;
    }

    // Check if a 3D transformation matrix has a scaling term in it. (4x4)
    constexpr bool has_scale3D(const mat4& m)
    {
        float la2 = length_squared(apply_vector3D(m, vec3(1, 0, 0)));
        float lb2 = length_squared(apply_vector3D(m, vec3(0, 1, 0)));
        float lc2 = length_squared(apply_vector3D(m, vec3(0, 0, 1)));
        return
            abs(la2 - 1.f) > .001f ||
            abs(lb2 - 1.f) > .001f ||
            abs(lc2 - 1.f) > .001f;
    }

    // Check if handedness is changed by a 3D transformation matrix. (3x3)
    constexpr bool swaps_handedness3D(const mat3& m)
    {
        return determinant(m) < 0;
    }

    // Check if handedness is changed by a 3D transformation matrix. (4x4)
    constexpr bool swaps_handedness3D(const mat4& m)
    {
        return determinant(m.sub<3>()) < 0;
    }

};
