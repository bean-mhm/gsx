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
    template<std::floating_point T>
    base_mat<T, 3, 3> translate_2d_h(
        const base_vec2<T>& delta,
        base_mat<T, 3, 3>* out_minv = nullptr
    )
    {
        if (out_minv)
        {
            *out_minv = base_mat<T, 3, 3>({
                1, 0, -delta.x,
                0, 1, -delta.y,
                0, 0, 1 }
            );
        }

        return base_mat<T, 3, 3>({
            1, 0, delta.x,
            0, 1, delta.y,
            0, 0, 1 }
        );
    }

    // 3D translation matrix (homogeneous)
    template<std::floating_point T>
    base_mat<T, 4, 4> translate_3d_h(
        const base_vec3<T>& delta,
        base_mat<T, 4, 4>* out_minv = nullptr
    )
    {
        if (out_minv)
        {
            *out_minv = base_mat<T, 4, 4>({
                1, 0, 0, -delta.x,
                0, 1, 0, -delta.y,
                0, 0, 1, -delta.z,
                0, 0, 0, 1 }
            );
        }

        return base_mat<T, 4, 4>({
            1, 0, 0, delta.x,
            0, 1, 0, delta.y,
            0, 0, 1, delta.z,
            0, 0, 0, 1 }
        );
    }

    // 2D scaling matrix
    template<std::floating_point T>
    base_mat<T, 2, 2> scale_2d(
        const base_vec2<T>& fac,
        base_mat<T, 2, 2>* out_minv = nullptr
    )
    {
        if (out_minv)
        {
            *out_minv = base_mat<T, 2, 2>({
                1 / fac.x, 0,
                0, 1 / fac.y }
            );
        }

        return base_mat<T, 2, 2>({
            fac.x, 0,
            0, fac.y }
        );
    }

    // 2D scaling matrix (homogeneous)
    template<std::floating_point T>
    base_mat<T, 3, 3> scale_2d_h(
        const base_vec2<T>& fac,
        base_mat<T, 3, 3>* out_minv = nullptr
    )
    {
        if (out_minv)
        {
            *out_minv = base_mat<T, 3, 3>({
                1 / fac.x, 0, 0,
                0, 1 / fac.y, 0,
                0, 0, 1 }
            );
        }

        return base_mat<T, 3, 3>({
            fac.x, 0, 0,
            0, fac.y, 0,
            0, 0, 1 }
        );
    }

    // 3D scaling matrix
    template<std::floating_point T>
    base_mat<T, 3, 3> scale_3d(
        const base_vec3<T>& fac,
        base_mat<T, 3, 3>* out_minv = nullptr
    )
    {
        if (out_minv)
        {
            *out_minv = base_mat<T, 3, 3>({
                1 / fac.x, 0, 0,
                0, 1 / fac.y, 0,
                0, 0, 1 / fac.z }
            );
        }

        return base_mat<T, 3, 3>({
            fac.x, 0, 0,
            0, fac.y, 0,
            0, 0, fac.z }
        );
    }

    // 3D scaling matrix (homogeneous)
    template<std::floating_point T>
    base_mat<T, 4, 4> scale_3d_h(
        const base_vec3<T>& fac,
        base_mat<T, 4, 4>* out_minv = nullptr
    )
    {
        if (out_minv)
        {
            *out_minv = base_mat<T, 4, 4>({
                1 / fac.x, 0, 0, 0,
                0, 1 / fac.y, 0, 0,
                0, 0, 1 / fac.z, 0,
                0, 0, 0, 1 }
            );
        }

        return base_mat<T, 4, 4>({
            fac.x, 0, 0, 0,
            0, fac.y, 0, 0,
            0, 0, fac.z, 0,
            0, 0, 0, 1 }
        );
    }

    // 2D rotation matrix
    template<std::floating_point T>
    base_mat<T, 2, 2> rotate_2d(
        T angle,
        base_mat<T, 2, 2>* out_minv = nullptr
    )
    {
        T s = sin(angle);
        T c = cos(angle);

        if (out_minv)
        {
            *out_minv = base_mat<T, 2, 2>({
                c, s,
                -s, c }
            );
        }

        return base_mat<T, 2, 2>({
            c, -s,
            s, c }
        );
    }

    // 2D rotation matrix (homogeneous)
    template<std::floating_point T>
    base_mat<T, 3, 3> rotate_2d_h(
        T angle,
        base_mat<T, 3, 3>* out_minv = nullptr
    )
    {
        T s = sin(angle);
        T c = cos(angle);

        if (out_minv)
        {
            *out_minv = base_mat<T, 3, 3>({
                c, s, 0,
                -s, c, 0,
                0, 0, 1 }
            );
        }

        return base_mat<T, 3, 3>({
            c, -s, 0,
            s, c, 0,
            0, 0, 1 }
        );
    }

    // 3D rotation matrix around the X axis (left-handed)
    template<std::floating_point T>
    base_mat<T, 3, 3> rotate_3d_x(
        T angle,
        base_mat<T, 3, 3>* out_minv = nullptr
    )
    {
        T s = sin(angle);
        T c = cos(angle);

        base_mat<T, 3, 3> r({
            1, 0, 0,
            0, c, -s,
            0, s, c }
        );

        if (out_minv)
            *out_minv = transpose(r);

        return r;
    }

    // 3D rotation matrix around the X axis (left-handed) (homogeneous)
    template<std::floating_point T>
    base_mat<T, 4, 4> rotate_3d_x_h(
        T angle,
        base_mat<T, 4, 4>* out_minv = nullptr
    )
    {
        T s = sin(angle);
        T c = cos(angle);

        base_mat<T, 4, 4> r({
            1, 0, 0, 0,
            0, c, -s, 0,
            0, s, c, 0,
            0, 0, 0, 1 }
        );

        if (out_minv)
            *out_minv = transpose(r);

        return r;
    }

    // 3D rotation matrix around the Y axis (left-handed)
    template<std::floating_point T>
    base_mat<T, 3, 3> rotate_3d_y(
        T angle,
        base_mat<T, 3, 3>* out_minv = nullptr
    )
    {
        T s = sin(angle);
        T c = cos(angle);

        base_mat<T, 3, 3> r({
            c, 0, s,
            0, 1, 0,
            -s, 0, c }
        );

        if (out_minv)
            *out_minv = transpose(r);

        return r;
    }

    // 3D rotation matrix around the Y axis (left-handed) (homogeneous)
    template<std::floating_point T>
    base_mat<T, 4, 4> rotate_3d_y_h(
        T angle,
        base_mat<T, 4, 4>* out_minv = nullptr
    )
    {
        T s = sin(angle);
        T c = cos(angle);

        base_mat<T, 4, 4> r({
            c, 0, s, 0,
            0, 1, 0, 0,
            -s, 0, c, 0,
            0, 0, 0, 1 }
        );

        if (out_minv)
            *out_minv = transpose(r);

        return r;
    }

    // 3D rotation matrix around the Z axis (left-handed)
    template<std::floating_point T>
    base_mat<T, 3, 3> rotate_3d_z(
        T angle,
        base_mat<T, 3, 3>* out_minv = nullptr
    )
    {
        T s = sin(angle);
        T c = cos(angle);

        base_mat<T, 3, 3> r({
            c, -s, 0,
            s, c, 0,
            0, 0, 1 }
        );

        if (out_minv)
            *out_minv = transpose(r);

        return r;
    }

    // 3D rotation matrix around the Z axis (left-handed) (homogeneous)
    template<std::floating_point T>
    base_mat<T, 4, 4> rotate_3d_z_h(
        T angle,
        base_mat<T, 4, 4>* out_minv = nullptr
    )
    {
        T s = sin(angle);
        T c = cos(angle);

        base_mat<T, 4, 4> r({
            c, -s, 0, 0,
            s, c, 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1 }
        );

        if (out_minv)
            *out_minv = transpose(r);

        return r;
    }

    // 3D rotation matrix around an arbitrary axis (left-handed)
    template<std::floating_point T>
    base_mat<T, 3, 3> rotate_3d(
        T angle,
        const base_vec3<T>& axis,
        base_mat<T, 3, 3>* out_minv = nullptr
    )
    {
        base_vec3<T> a = normalize(axis);
        T s = sin(angle);
        T c = cos(angle);

        base_mat<T, 3, 3> r;

        // compute rotation of first basis vector
        r(0, 0) = a.x * a.x + (1 - a.x * a.x) * c;
        r(0, 1) = a.x * a.y * (1 - c) - a.z * s;
        r(0, 2) = a.x * a.z * (1 - c) + a.y * s;

        // second basis vector
        r(1, 0) = a.x * a.y * (1 - c) + a.z * s;
        r(1, 1) = a.y * a.y + (1 - a.y * a.y) * c;
        r(1, 2) = a.y * a.z * (1 - c) - a.x * s;

        // third basis vector
        r(2, 0) = a.x * a.z * (1 - c) - a.y * s;
        r(2, 1) = a.y * a.z * (1 - c) + a.x * s;
        r(2, 2) = a.z * a.z + (1 - a.z * a.z) * c;

        if (out_minv)
            *out_minv = transpose(r);

        return r;
    }

    // 3D rotation matrix around an arbitrary axis (left-handed) (homogeneous)
    template<std::floating_point T>
    base_mat<T, 4, 4> rotate_3d_h(
        T angle,
        const base_vec3<T>& axis,
        base_mat<T, 4, 4>* out_minv = nullptr
    )
    {
        base_vec3<T> a = normalize(axis);
        T s = sin(angle);
        T c = cos(angle);

        base_mat<T, 4, 4> r;

        // compute rotation of first basis vector
        r(0, 0) = a.x * a.x + (1 - a.x * a.x) * c;
        r(0, 1) = a.x * a.y * (1 - c) - a.z * s;
        r(0, 2) = a.x * a.z * (1 - c) + a.y * s;
        r(0, 3) = 0;

        // second basis vector
        r(1, 0) = a.x * a.y * (1 - c) + a.z * s;
        r(1, 1) = a.y * a.y + (1 - a.y * a.y) * c;
        r(1, 2) = a.y * a.z * (1 - c) - a.x * s;
        r(1, 3) = 0;

        // third basis vector
        r(2, 0) = a.x * a.z * (1 - c) - a.y * s;
        r(2, 1) = a.y * a.z * (1 - c) + a.x * s;
        r(2, 2) = a.z * a.z + (1 - a.z * a.z) * c;
        r(2, 3) = 0;

        if (out_minv)
            *out_minv = transpose(r);

        return r;
    }

    // 3D homogeneous transformation from a left-handed viewing coordinate
    // system where the camera is at the origin looking along the +z axis, where
    // the +y axis is along the up direction
    template<std::floating_point T>
    base_mat<T, 4, 4> lookat_3d_h(
        const base_vec3<T>& pos,
        const base_vec3<T>& look,
        const base_vec3<T>& up,
        base_mat<T, 4, 4>* out_minv = nullptr
    )
    {
        base_mat<T, 4, 4> cam_to_world;

        // initialize fourth column of viewing matrix
        cam_to_world(0, 3) = pos.x;
        cam_to_world(1, 3) = pos.y;
        cam_to_world(2, 3) = pos.z;
        cam_to_world(3, 3) = 1;

        // initialize first three columns of viewing matrix
        base_vec3<T> dir = normalize(look - pos);
        base_vec3<T> right = normalize(cross(normalize(up), dir));
        base_vec3<T> new_up = cross(dir, right);
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

    // transform a 2D point using a 2x2 matrix
    template<std::floating_point T>
    constexpr base_vec2<T> apply_point_2d(
        const base_mat<T, 2, 2>& m,
        const base_vec2<T>& p
    )
    {
        return base_vec2<T>(m * base_mat<T, 2, 1>(p));
    }

    // transform a 2D point using a 3x3 homogeneous matrix
    template<std::floating_point T>
    constexpr base_vec2<T> apply_point_2d_h(
        const base_mat<T, 3, 3>& m,
        const base_vec2<T>& p
    )
    {
        base_vec3<T> r(m * base_mat<T, 3, 1>(base_vec3<T>(p, 1)));
        if (r.z == 0)
            return r.permute(0, 1);
        else
            return r.permute(0, 1) / r.z;
    }

    // transform a 3D point using a 3x3 matrix
    template<std::floating_point T>
    constexpr base_vec3<T> apply_point_3d(
        const base_mat<T, 3, 3>& m,
        const base_vec3<T>& p
    )
    {
        return base_vec3<T>(m * base_mat<T, 3, 1>(p));
    }

    // transform a 3D point using a 4x4 homogeneous matrix
    template<std::floating_point T>
    constexpr base_vec3<T> apply_point_3d_h(
        const base_mat<T, 4, 4>& m,
        const base_vec3<T>& p
    )
    {
        base_vec4<T> r(m * base_mat<T, 4, 1>(base_vec4<T>(p, 1)));
        if (r.w == 0)
            return r.permute(0, 1, 2);
        else
            return r.permute(0, 1, 2) / r.w;
    }

    // transform a 2D vector using a 2x2 matrix
    template<std::floating_point T>
    constexpr base_vec2<T> apply_vector_2d(
        const base_mat<T, 2, 2>& m,
        const base_vec2<T>& v
    )
    {
        return base_vec2<T>(m * base_mat<T, 2, 1>(v));
    }

    // transform a 2D vector using a 3x3 homogeneous matrix
    template<std::floating_point T>
    constexpr base_vec2<T> apply_vector_2d_h(
        const base_mat<T, 3, 3>& m,
        const base_vec2<T>& v
    )
    {
        return base_vec2<T>(m.sub<2>() * base_mat<T, 2, 1>(v));
    }

    // transform a 3D vector using a 3x3 matrix
    template<std::floating_point T>
    constexpr base_vec3<T> apply_vector_3d(
        const base_mat<T, 3, 3>& m,
        const base_vec3<T>& v
    )
    {
        return base_vec3<T>(m * base_mat<T, 3, 1>(v));
    }

    // transform a 3D vector using a 4x4 homogeneous matrix
    template<std::floating_point T>
    constexpr base_vec3<T> apply_vector_3d_h(
        const base_mat<T, 4, 4>& m,
        const base_vec3<T>& v
    )
    {
        return base_vec3<T>(m.sub<3>() * base_mat<T, 3, 1>(v));
    }

    // transform a 2D normal vector using a 2x2 matrix
    // * you need to input the inverted version of your transformation matrix.
    //   use the inverse() function if you only have the original transformation
    //   matrix.
    template<std::floating_point T>
    constexpr base_vec2<T> apply_normal_2d(
        const base_mat<T, 2, 2>& minv,
        const base_vec2<T>& n
    )
    {
        return base_vec2<T>(transpose(minv) * base_mat<T, 2, 1>(n));
    }

    // transform a 2D normal vector using a 3x3 homogeneous matrix
    // * you need to input the inverted version of your transformation matrix.
    //   use the inverse() function if you only have the original transformation
    //   matrix.
    template<std::floating_point T>
    constexpr base_vec2<T> apply_normal_2d_h(
        const base_mat<T, 3, 3>& minv,
        const base_vec2<T>& n
    )
    {
        return base_vec2<T>(transpose(minv.sub<2>()) * base_mat<T, 2, 1>(n));
    }

    // transform a 3D normal vector using a 3x3 matrix
    // * you need to input the inverted version of your transformation matrix.
    //   use the inverse() function if you only have the original transformation
    //   matrix.
    template<std::floating_point T>
    constexpr base_vec3<T> apply_normal_3d(
        const base_mat<T, 3, 3>& minv,
        const base_vec3<T>& n
    )
    {
        return base_vec3<T>(transpose(minv) * base_mat<T, 3, 1>(n));
    }

    // transform a 3D normal vector using a 4x4 homogeneous matrix
    // * you need to input the inverted version of your transformation matrix.
    //   use the inverse() function if you only have the original transformation
    //   matrix.
    template<std::floating_point T>
    constexpr base_vec3<T> apply_normal_3d_h(
        const base_mat<T, 4, 4>& minv,
        const base_vec3<T>& n
    )
    {
        return base_vec3<T>(transpose(minv.sub<3>()) * base_mat<T, 3, 1>(n));
    }

    // transform a 3D ray using a 3x3 matrix
    template<std::floating_point T>
    constexpr base_ray_t<T> apply_ray_3d(
        const base_mat<T, 3, 3>& m,
        base_ray_t<T> r
    )
    {
        r.o = apply_point_3d(m, r.o);
        r.d = apply_vector_3d(m, r.d);
        return r;
    }

    // transform a 3D ray using a 4x4 homogeneous matrix
    template<std::floating_point T>
    constexpr base_ray_t<T> apply_ray_3d_h(
        const base_mat<T, 4, 4>& m,
        base_ray_t<T> r
    )
    {
        r.o = apply_point_3d_h(m, r.o);
        r.d = apply_vector_3d_h(m, r.d);
        return r;
    }

    // transform a 2D AABB using a 2x2 matrix
    template<std::floating_point T>
    constexpr base_bounds2<T> apply_bounds_2d(
        const base_mat<T, 2, 2>& m,
        const base_bounds2<T>& b
    )
    {
        base_bounds2<T> r(apply_point_2d(m, b.pmin));
        r = union_(r, apply_point_2d(m, base_vec2<T>(b.pmin.x, b.pmax.y)));
        r = union_(r, apply_point_2d(m, base_vec2<T>(b.pmax.x, b.pmin.y)));
        r = union_(r, apply_point_2d(m, b.pmax));
        return r;
    }

    // transform a 2D AABB using a 3x3 homogeneous matrix
    template<std::floating_point T>
    constexpr base_bounds2<T> apply_bounds_2d_h(
        const base_mat<T, 3, 3>& m,
        const base_bounds2<T>& b
    )
    {
        base_bounds2<T> r(apply_point_2d_h(m, b.pmin));
        r = union_(r, apply_point_2d_h(m, base_vec2<T>(b.pmin.x, b.pmax.y)));
        r = union_(r, apply_point_2d_h(m, base_vec2<T>(b.pmax.x, b.pmin.y)));
        r = union_(r, apply_point_2d_h(m, b.pmax));
        return r;
    }

    // transform a 3D AABB using a 3x3 matrix
    template<std::floating_point T>
    constexpr base_bounds3<T> apply_bounds_3d(
        const base_mat<T, 3, 3>& m,
        const base_bounds3<T>& b
    )
    {
        base_bounds3<T> r(apply_point_3d(m, b.pmin));
        r = union_(r, apply_point_3d(
            m, base_vec3<T>(b.pmax.x, b.pmin.y, b.pmin.z)
        ));
        r = union_(r, apply_point_3d(
            m, base_vec3<T>(b.pmin.x, b.pmax.y, b.pmin.z)
        ));
        r = union_(r, apply_point_3d(
            m, base_vec3<T>(b.pmin.x, b.pmin.y, b.pmax.z)
        ));
        r = union_(r, apply_point_3d(
            m, base_vec3<T>(b.pmin.x, b.pmax.y, b.pmax.z)
        ));
        r = union_(r, apply_point_3d(
            m, base_vec3<T>(b.pmax.x, b.pmax.y, b.pmin.z)
        ));
        r = union_(r, apply_point_3d(
            m, base_vec3<T>(b.pmax.x, b.pmin.y, b.pmax.z)
        ));
        r = union_(r, apply_point_3d(m, b.pmax));
        return r;
    }

    // transform a 3D AABB using a 4x4 homogeneous matrix
    template<std::floating_point T>
    constexpr base_bounds3<T> apply_bounds_3d_h(
        const base_mat<T, 4, 4>& m,
        const base_bounds3<T>& b
    )
    {
        base_bounds3<T> r(apply_point_3d_h(m, b.pmin));
        r = union_(r, apply_point_3d_h(
            m, base_vec3<T>(b.pmax.x, b.pmin.y, b.pmin.z)
        ));
        r = union_(r, apply_point_3d_h(
            m, base_vec3<T>(b.pmin.x, b.pmax.y, b.pmin.z)
        ));
        r = union_(r, apply_point_3d_h(
            m, base_vec3<T>(b.pmin.x, b.pmin.y, b.pmax.z)
        ));
        r = union_(r, apply_point_3d_h(
            m, base_vec3<T>(b.pmin.x, b.pmax.y, b.pmax.z)
        ));
        r = union_(r, apply_point_3d_h(
            m, base_vec3<T>(b.pmax.x, b.pmax.y, b.pmin.z)
        ));
        r = union_(r, apply_point_3d_h(
            m, base_vec3<T>(b.pmax.x, b.pmin.y, b.pmax.z)
        ));
        r = union_(r, apply_point_3d_h(m, b.pmax));
        return r;
    }

    // check if a 2D transformation matrix has a scaling term in it
    template<std::floating_point T>
    constexpr bool has_scale_2d(const base_mat<T, 2, 2>& m)
    {
        T la2 = length_squared(apply_vector_2d(m, base_vec2<T>(1, 0)));
        T lb2 = length_squared(apply_vector_2d(m, base_vec2<T>(0, 1)));
        return
            la2 < (T).9999 || la2 >(T)1.0001 ||
            lb2 < (T).9999 || lb2 >(T)1.0001;
    }

    // check if a 2D homogeneous transformation matrix has a scaling term in it
    template<std::floating_point T>
    constexpr bool has_scale_2d_h(const base_mat<T, 3, 3>& m)
    {
        T la2 = length_squared(apply_vector_2d_h(m, base_vec2<T>(1, 0)));
        T lb2 = length_squared(apply_vector_2d_h(m, base_vec2<T>(0, 1)));
        return
            la2 < (T).9999 || la2 >(T)1.0001 ||
            lb2 < (T).9999 || lb2 >(T)1.0001;
    }

    // check if a 3D transformation matrix has a scaling term in it
    template<std::floating_point T>
    constexpr bool has_scale_3d(const base_mat<T, 3, 3>& m)
    {
        T la2 = length_squared(apply_vector_3d(m, base_vec3<T>(1, 0, 0)));
        T lb2 = length_squared(apply_vector_3d(m, base_vec3<T>(0, 1, 0)));
        T lc2 = length_squared(apply_vector_3d(m, base_vec3<T>(0, 0, 1)));
        return
            la2 < (T).9999 || la2 >(T)1.0001 ||
            lb2 < (T).9999 || lb2 >(T)1.0001 ||
            lc2 < (T).9999 || lc2 >(T)1.0001;
    }

    // check if a 3D homogeneous transformation matrix has a scaling term in it
    template<std::floating_point T>
    constexpr bool has_scale_3d_h(const base_mat<T, 4, 4>& m)
    {
        T la2 = length_squared(apply_vector_3d_h(m, base_vec3<T>(1, 0, 0)));
        T lb2 = length_squared(apply_vector_3d_h(m, base_vec3<T>(0, 1, 0)));
        T lc2 = length_squared(apply_vector_3d_h(m, base_vec3<T>(0, 0, 1)));
        return
            la2 < (T).9999 || la2 >(T)1.0001 ||
            lb2 < (T).9999 || lb2 >(T)1.0001 ||
            lc2 < (T).9999 || lc2 >(T)1.0001;
    }

    // check if handedness is changed by a 3D transformation matrix
    template<std::floating_point T>
    constexpr bool swaps_handedness_3d(const base_mat<T, 3, 3>& m)
    {
        return determinant(m) < 0;
    }

    // check if handedness is changed by a 3D homogeneous transformation matrix
    template<std::floating_point T>
    constexpr bool swaps_handedness_3d_h(const base_mat<T, 4, 4>& m)
    {
        return determinant(m.sub<3>()) < 0;
    }

};
