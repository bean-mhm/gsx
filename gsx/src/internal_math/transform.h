#pragma once

#include "vec2.h"
#include "vec3.h"
#include "vec4.h"
#include "bounds2.h"
#include "bounds3.h"
#include "ray.h"
#include "matrix.h"
#include "utils.h"
#include "../internal_common/all.h"

namespace gsx::math::transform
{

    // 2D translation matrix (homogeneous)
    mat3 translate_2d_h(const vec2& delta, mat3* out_minv = nullptr);

    // 3D translation matrix (homogeneous)
    mat4 translate_3d_h(const vec3& delta, mat4* out_minv = nullptr);

    // 2D scaling matrix
    mat2 scale_2d(const vec2& fac, mat2* out_minv = nullptr);

    // 2D scaling matrix (homogeneous)
    mat3 scale_2d_h(const vec2& fac, mat3* out_inv);

    // 3D scaling matrix
    mat3 scale_3d(const vec3& fac, mat3* out_minv = nullptr);

    // 3D scaling matrix (homogeneous)
    mat4 scale_3d_h(const vec3& fac, mat4* out_minv = nullptr);

    // 2D rotation matrix
    mat2 rotate_2d(f32 angle, mat2* out_minv = nullptr);

    // 2D rotation matrix (homogeneous)
    mat3 rotate_2d_h(f32 angle, mat3* out_minv = nullptr);

    // 3D rotation matrix around the X axis (left-handed)
    mat3 rotate_3d_x(f32 angle, mat3* out_minv = nullptr);

    // 3D rotation matrix around the X axis (left-handed) (homogeneous)
    mat4 rotate_3d_x_h(f32 angle, mat4* out_minv = nullptr);

    // 3D rotation matrix around the Y axis (left-handed)
    mat3 rotate_3d_y(f32 angle, mat3* out_minv = nullptr);

    // 3D rotation matrix around the Y axis (left-handed) (homogeneous)
    mat4 rotate_3d_y_h(f32 angle, mat4* out_minv = nullptr);

    // 3D rotation matrix around the Z axis (left-handed)
    mat3 rotate_3d_z(f32 angle, mat3* out_minv = nullptr);

    // 3D rotation matrix around the Z axis (left-handed) (homogeneous)
    mat4 rotate_3d_z_h(f32 angle, mat4* out_minv = nullptr);

    // 3D rotation matrix around an arbitrary axis (left-handed)
    mat3 rotate_3d(f32 angle, const vec3& axis, mat3* out_minv = nullptr);

    // 3D rotation matrix around an arbitrary axis (left-handed) (homogeneous)
    mat4 rotate_3d_h(f32 angle, const vec3& axis, mat4* out_minv = nullptr);

    // 3D homogeneous transformation from a left-handed viewing coordinate
    // system where the camera is at the origin looking along the +z axis, where
    // the +y axis is along the up direction
    mat4 lookat_3d_h(
        const vec3& pos,
        const vec3& look,
        const vec3& up,
        mat4* out_minv = nullptr
    );

    // transform a 2D point using a 2x2 matrix
    constexpr vec2 apply_point_2d(const mat2& m, const vec2& p)
    {
        return vec2(m * mat2x1(p));
    }

    // transform a 2D point using a 3x3 homogeneous matrix
    constexpr vec2 apply_point_2d_h(const mat3& m, const vec2& p)
    {
        vec3 r(m * mat3x1(vec3(p, 1)));
        if (r.z == 0)
            return r.permute(0, 1);
        else
            return r.permute(0, 1) / r.z;
    }

    // transform a 3D point using a 3x3 matrix
    constexpr vec3 apply_point_3d(const mat3& m, const vec3& p)
    {
        return vec3(m * mat3x1(p));
    }

    // transform a 3D point using a 4x4 homogeneous matrix
    constexpr vec3 apply_point_3d_h(const mat4& m, const vec3& p)
    {
        vec4 r(m * mat4x1(vec4(p, 1)));
        if (r.w == 0)
            return r.permute(0, 1, 2);
        else
            return r.permute(0, 1, 2) / r.w;
    }

    // transform a 2D vector using a 2x2 matrix
    constexpr vec2 apply_vector_2d(const mat2& m, const vec2& v)
    {
        return vec2(m * mat2x1(v));
    }

    // transform a 2D vector using a 3x3 homogeneous matrix
    constexpr vec2 apply_vector_2d_h(const mat3& m, const vec2& v)
    {
        return vec2(m.sub<2>() * mat2x1(v));
    }

    // transform a 3D vector using a 3x3 matrix
    constexpr vec3 apply_vector_3d(const mat3& m, const vec3& v)
    {
        return vec3(m * mat3x1(v));
    }

    // transform a 3D vector using a 4x4 homogeneous matrix
    constexpr vec3 apply_vector_3d_h(const mat4& m, const vec3& v)
    {
        return vec3(m.sub<3>() * mat3x1(v));
    }

    // transform a 2D normal vector using a 2x2 matrix
    // * you need to input the inverted version of your transformation matrix.
    //   use the inverse() function if you only have the original transformation
    //   matrix.
    constexpr vec2 apply_normal_2d(const mat2& minv, const vec2& n)
    {
        return vec2(transpose(minv) * mat2x1(n));
    }

    // transform a 2D normal vector using a 3x3 homogeneous matrix
    // * you need to input the inverted version of your transformation matrix.
    //   use the inverse() function if you only have the original transformation
    //   matrix.
    constexpr vec2 apply_normal_2d_h(const mat3& minv, const vec2& n)
    {
        return vec2(transpose(minv.sub<2>()) * mat2x1(n));
    }

    // transform a 3D normal vector using a 3x3 matrix
    // * you need to input the inverted version of your transformation matrix.
    //   use the inverse() function if you only have the original transformation
    //   matrix.
    constexpr vec3 apply_normal_3d(const mat3& minv, const vec3& n)
    {
        return vec3(transpose(minv) * mat3x1(n));
    }

    // transform a 3D normal vector using a 4x4 homogeneous matrix
    // * you need to input the inverted version of your transformation matrix.
    //   use the inverse() function if you only have the original transformation
    //   matrix.
    constexpr vec3 apply_normal_3d_h(const mat4& minv, const vec3& n)
    {
        return vec3(transpose(minv.sub<3>()) * mat3x1(n));
    }

    // transform a 3D ray using a 3x3 matrix
    constexpr ray_t apply_ray_3d(const mat3& m, ray_t r)
    {
        r.o = apply_point_3d(m, r.o);
        r.d = apply_vector_3d(m, r.d);
        return r;
    }

    // transform a 3D ray using a 4x4 homogeneous matrix
    constexpr ray_t apply_ray_3d_h(const mat4& m, ray_t r)
    {
        r.o = apply_point_3d_h(m, r.o);
        r.d = apply_vector_3d_h(m, r.d);
        return r;
    }

    // transform a 2D AABB using a 2x2 matrix
    constexpr bounds2 apply_bounds_2d(const mat2& m, const bounds2& b)
    {
        bounds2 r(apply_point_2d(m, b.pmin));
        r = union_(r, apply_point_2d(m, vec2(b.pmin.x, b.pmax.y)));
        r = union_(r, apply_point_2d(m, vec2(b.pmax.x, b.pmin.y)));
        r = union_(r, apply_point_2d(m, b.pmax));
        return r;
    }

    // transform a 2D AABB using a 3x3 homogeneous matrix
    constexpr bounds2 apply_bounds_2d_h(const mat3& m, const bounds2& b)
    {
        bounds2 r(apply_point_2d_h(m, b.pmin));
        r = union_(r, apply_point_2d_h(m, vec2(b.pmin.x, b.pmax.y)));
        r = union_(r, apply_point_2d_h(m, vec2(b.pmax.x, b.pmin.y)));
        r = union_(r, apply_point_2d_h(m, b.pmax));
        return r;
    }

    // transform a 3D AABB using a 3x3 matrix
    constexpr bounds3 apply_bounds_3d(const mat3& m, const bounds3& b)
    {
        bounds3 r(apply_point_3d(m, b.pmin));
        r = union_(r, apply_point_3d(m, vec3(b.pmax.x, b.pmin.y, b.pmin.z)));
        r = union_(r, apply_point_3d(m, vec3(b.pmin.x, b.pmax.y, b.pmin.z)));
        r = union_(r, apply_point_3d(m, vec3(b.pmin.x, b.pmin.y, b.pmax.z)));
        r = union_(r, apply_point_3d(m, vec3(b.pmin.x, b.pmax.y, b.pmax.z)));
        r = union_(r, apply_point_3d(m, vec3(b.pmax.x, b.pmax.y, b.pmin.z)));
        r = union_(r, apply_point_3d(m, vec3(b.pmax.x, b.pmin.y, b.pmax.z)));
        r = union_(r, apply_point_3d(m, b.pmax));
        return r;
    }

    // transform a 3D AABB using a 4x4 homogeneous matrix
    constexpr bounds3 apply_bounds_3d_h(const mat4& m, const bounds3& b)
    {
        bounds3 r(apply_point_3d_h(m, b.pmin));
        r = union_(r, apply_point_3d_h(m, vec3(b.pmax.x, b.pmin.y, b.pmin.z)));
        r = union_(r, apply_point_3d_h(m, vec3(b.pmin.x, b.pmax.y, b.pmin.z)));
        r = union_(r, apply_point_3d_h(m, vec3(b.pmin.x, b.pmin.y, b.pmax.z)));
        r = union_(r, apply_point_3d_h(m, vec3(b.pmin.x, b.pmax.y, b.pmax.z)));
        r = union_(r, apply_point_3d_h(m, vec3(b.pmax.x, b.pmax.y, b.pmin.z)));
        r = union_(r, apply_point_3d_h(m, vec3(b.pmax.x, b.pmin.y, b.pmax.z)));
        r = union_(r, apply_point_3d_h(m, b.pmax));
        return r;
    }

    // check if a 2D transformation matrix has a scaling term in it
    constexpr bool has_scale_2d(const mat2& m)
    {
        f32 la2 = length_squared(apply_vector_2d(m, vec2(1, 0)));
        f32 lb2 = length_squared(apply_vector_2d(m, vec2(0, 1)));
        return
            la2 < .9999f || la2 > 1.0001f ||
            lb2 < .9999f || lb2 > 1.0001f;
    }

    // check if a 2D homogeneous transformation matrix has a scaling term in it
    constexpr bool has_scale_2d_h(const mat3& m)
    {
        f32 la2 = length_squared(apply_vector_2d_h(m, vec2(1, 0)));
        f32 lb2 = length_squared(apply_vector_2d_h(m, vec2(0, 1)));
        return
            la2 < .9999f || la2 > 1.0001f ||
            lb2 < .9999f || lb2 > 1.0001f;
    }

    // check if a 3D transformation matrix has a scaling term in it
    constexpr bool has_scale_3d(const mat3& m)
    {
        f32 la2 = length_squared(apply_vector_3d(m, vec3(1, 0, 0)));
        f32 lb2 = length_squared(apply_vector_3d(m, vec3(0, 1, 0)));
        f32 lc2 = length_squared(apply_vector_3d(m, vec3(0, 0, 1)));
        return
            la2 < .9999f || la2 > 1.0001f ||
            lb2 < .9999f || lb2 > 1.0001f ||
            lc2 < .9999f || lc2 > 1.0001f;
    }

    // check if a 3D homogeneous transformation matrix has a scaling term in it
    constexpr bool has_scale_3d_h(const mat4& m)
    {
        f32 la2 = length_squared(apply_vector_3d_h(m, vec3(1, 0, 0)));
        f32 lb2 = length_squared(apply_vector_3d_h(m, vec3(0, 1, 0)));
        f32 lc2 = length_squared(apply_vector_3d_h(m, vec3(0, 0, 1)));
        return
            la2 < .9999f || la2 > 1.0001f ||
            lb2 < .9999f || lb2 > 1.0001f ||
            lc2 < .9999f || lc2 > 1.0001f;
    }

    // check if handedness is changed by a 3D transformation matrix
    constexpr bool swaps_handedness_3d(const mat3& m)
    {
        return determinant(m) < 0;
    }

    // check if handedness is changed by a 3D homogeneous transformation matrix
    constexpr bool swaps_handedness_3d_h(const mat4& m)
    {
        return determinant(m.sub<3>()) < 0;
    }

};
