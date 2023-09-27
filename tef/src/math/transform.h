#pragma once

// Internal
#include "vec2.h"
#include "vec3.h"
#include "vec4.h"
#include "bounds2.h"
#include "bounds3.h"
#include "ray.h"
#include "matrix.h"
#include "utils.h"

namespace tef::math::transform
{

    // 2D translation matrix (homogeneous)
    mat3 translate2D_h(const vec2& delta, mat3* out_minv = nullptr);

    // 3D translation matrix (homogeneous)
    mat4 translate3D_h(const vec3& delta, mat4* out_minv = nullptr);

    // 2D scaling matrix
    mat2 scale2D(const vec2& fac, mat2* out_minv = nullptr);

    // 2D scaling matrix (homogeneous)
    mat3 scale2D_h(const vec2& fac, mat3* out_inv);

    // 3D scaling matrix
    mat3 scale3D(const vec3& fac, mat3* out_minv = nullptr);

    // 3D scaling matrix (homogeneous)
    mat4 scale3D_h(const vec3& fac, mat4* out_minv = nullptr);

    // 2D rotation matrix
    mat2 rotate2D(float angle, mat2* out_minv = nullptr);

    // 2D rotation matrix (homogeneous)
    mat3 rotate2D_h(float angle, mat3* out_minv = nullptr);

    // 3D rotation matrix around the X axis (left-handed)
    mat3 rotate3D_x(float angle, mat3* out_minv = nullptr);

    // 3D rotation matrix around the X axis (left-handed) (homogeneous)
    mat4 rotate3D_x_h(float angle, mat4* out_minv = nullptr);

    // 3D rotation matrix around the Y axis (left-handed)
    mat3 rotate3D_y(float angle, mat3* out_minv = nullptr);

    // 3D rotation matrix around the Y axis (left-handed) (homogeneous)
    mat4 rotate3D_y_h(float angle, mat4* out_minv = nullptr);

    // 3D rotation matrix around the Z axis (left-handed)
    mat3 rotate3D_z(float angle, mat3* out_minv = nullptr);

    // 3D rotation matrix around the Z axis (left-handed) (homogeneous)
    mat4 rotate3D_z_h(float angle, mat4* out_minv = nullptr);

    // 3D rotation matrix around an arbitrary axis (left-handed)
    mat3 rotate3D(float angle, const vec3& axis, mat3* out_minv = nullptr);

    // 3D rotation matrix around an arbitrary axis (left-handed) (homogeneous)
    mat4 rotate3D_h(float angle, const vec3& axis, mat4* out_minv = nullptr);

    // 3D homogeneous transformation from a left-handed viewing coordinate system where the camera
    // is at the origin looking along the +z axis, where the +y axis is along the up direction
    mat4 lookat3D_h(const vec3& pos, const vec3& look, const vec3& up, mat4* out_minv = nullptr);

    // Transform a 2D point using a 2x2 matrix.
    constexpr vec2 apply_point2D(const mat2& m, const vec2& p)
    {
        return vec2(m * mat2x1(p));
    }

    // Transform a 2D point using a 3x3 homogeneous matrix.
    constexpr vec2 apply_point2D_h(const mat3& m, const vec2& p)
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

    // Transform a 3D point using a 4x4 homogeneous matrix.
    constexpr vec3 apply_point3D_h(const mat4& m, const vec3& p)
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

    // Transform a 2D vector using a 3x3 homogeneous matrix.
    constexpr vec2 apply_vector2D_h(const mat3& m, const vec2& v)
    {
        return vec2(m.sub<2>() * mat2x1(v));
    }

    // Transform a 3D vector using a 3x3 matrix.
    constexpr vec3 apply_vector3D(const mat3& m, const vec3& v)
    {
        return vec3(m * mat3x1(v));
    }

    // Transform a 3D vector using a 4x4 homogeneous matrix.
    constexpr vec3 apply_vector3D_h(const mat4& m, const vec3& v)
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

    // Transform a 2D normal vector using a 3x3 homogeneous matrix.
    // Note: You need to input the inverted version of your transformation matrix. Use the
    // inverse() function if you only have the original transformation matrix.
    constexpr vec2 apply_normal2D_h(const mat3& minv, const vec2& n)
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

    // Transform a 3D normal vector using a 4x4 homogeneous matrix.
    // Note: You need to input the inverted version of your transformation matrix. Use the
    // inverse() function if you only have the original transformation matrix.
    constexpr vec3 apply_normal3D_h(const mat4& minv, const vec3& n)
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

    // Transform a 3D ray using a 4x4 homogeneous matrix.
    constexpr ray_t apply_ray3D_h(const mat4& m, ray_t r)
    {
        r.o = apply_point3D_h(m, r.o);
        r.d = apply_vector3D_h(m, r.d);
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

    // Transform a 2D AABB using a 3x3 homogeneous matrix.
    constexpr bounds2 apply_bounds2D_h(const mat3& m, const bounds2& b)
    {
        bounds2 r(apply_point2D_h(m, b.pmin));
        r = union_(r, apply_point2D_h(m, vec2(b.pmin.x, b.pmax.y)));
        r = union_(r, apply_point2D_h(m, vec2(b.pmax.x, b.pmin.y)));
        r = union_(r, apply_point2D_h(m, b.pmax));
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

    // Transform a 3D AABB using a 4x4 homogeneous matrix.
    constexpr bounds3 apply_bounds3D_h(const mat4& m, const bounds3& b)
    {
        bounds3 r(apply_point3D_h(m, b.pmin));
        r = union_(r, apply_point3D_h(m, vec3(b.pmax.x, b.pmin.y, b.pmin.z)));
        r = union_(r, apply_point3D_h(m, vec3(b.pmin.x, b.pmax.y, b.pmin.z)));
        r = union_(r, apply_point3D_h(m, vec3(b.pmin.x, b.pmin.y, b.pmax.z)));
        r = union_(r, apply_point3D_h(m, vec3(b.pmin.x, b.pmax.y, b.pmax.z)));
        r = union_(r, apply_point3D_h(m, vec3(b.pmax.x, b.pmax.y, b.pmin.z)));
        r = union_(r, apply_point3D_h(m, vec3(b.pmax.x, b.pmin.y, b.pmax.z)));
        r = union_(r, apply_point3D_h(m, b.pmax));
        return r;
    }

    // Check if a 2D transformation matrix has a scaling term in it.
    constexpr bool has_scale2D(const mat2& m)
    {
        float la2 = length_squared(apply_vector2D(m, vec2(1, 0)));
        float lb2 = length_squared(apply_vector2D(m, vec2(0, 1)));
        return
            la2 < .9999f || la2 > 1.0001f ||
            lb2 < .9999f || lb2 > 1.0001f;
    }

    // Check if a 2D homogeneous transformation matrix has a scaling term in it.
    constexpr bool has_scale2D_h(const mat3& m)
    {
        float la2 = length_squared(apply_vector2D_h(m, vec2(1, 0)));
        float lb2 = length_squared(apply_vector2D_h(m, vec2(0, 1)));
        return
            la2 < .9999f || la2 > 1.0001f ||
            lb2 < .9999f || lb2 > 1.0001f;
    }

    // Check if a 3D transformation matrix has a scaling term in it.
    constexpr bool has_scale3D(const mat3& m)
    {
        float la2 = length_squared(apply_vector3D(m, vec3(1, 0, 0)));
        float lb2 = length_squared(apply_vector3D(m, vec3(0, 1, 0)));
        float lc2 = length_squared(apply_vector3D(m, vec3(0, 0, 1)));
        return
            la2 < .9999f || la2 > 1.0001f ||
            lb2 < .9999f || lb2 > 1.0001f ||
            lc2 < .9999f || lc2 > 1.0001f;
    }

    // Check if a 3D homogeneous transformation matrix has a scaling term in it.
    constexpr bool has_scale3D_h(const mat4& m)
    {
        float la2 = length_squared(apply_vector3D_h(m, vec3(1, 0, 0)));
        float lb2 = length_squared(apply_vector3D_h(m, vec3(0, 1, 0)));
        float lc2 = length_squared(apply_vector3D_h(m, vec3(0, 0, 1)));
        return
            la2 < .9999f || la2 > 1.0001f ||
            lb2 < .9999f || lb2 > 1.0001f ||
            lc2 < .9999f || lc2 > 1.0001f;
    }

    // Check if handedness is changed by a 3D transformation matrix.
    constexpr bool swaps_handedness3D(const mat3& m)
    {
        return determinant(m) < 0;
    }

    // Check if handedness is changed by a 3D homogeneous transformation matrix.
    constexpr bool swaps_handedness3D_h(const mat4& m)
    {
        return determinant(m.sub<3>()) < 0;
    }

};
