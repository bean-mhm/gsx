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

    // Note: In the functions below, the overwrite argument means whether the output matrices
    // should be overwritten by the newly generated matrices, or just multiplied. If overwrite is
    // false, the output inverse matrix is updated according to the fact that
    // inv(A * B) = inv(B) * inv(A).

    // Note: The goofy syntax of the functions below is because of the fact that for most of these
    // transformations, we can calculate their inverse easily and way faster than the general
    // inverse() function ever could, so if the caller also wants the inverse, they can get it in
    // a computationally cheaper way.

    // 2D translation matrix (3x3)
    void translate2D(
        const vec2& delta,
        mat3* out_m,
        mat3* out_minv = nullptr,
        const bool overwrite = false
    );

    // Shortened version of translate2D()
    mat3 translate2D(const vec2& delta);

    // 3D translation matrix (4x4)
    void translate3D(
        const vec3& delta,
        mat4* out_m,
        mat4* out_minv = nullptr,
        const bool overwrite = false
    );

    // Shortened version of translate3D()
    mat4 translate3D(const vec3& delta);

    // 2D scaling matrix (2x2)
    void scale2D(
        const vec2& fac,
        mat2* out_m,
        mat2* out_minv = nullptr,
        const bool overwrite = false
    );

    // 2D scaling matrix (3x3)
    void scale2D(
        const vec2& fac,
        mat3* out_m,
        mat3* out_minv = nullptr,
        const bool overwrite = false
    );

    // Shortened versions of scale2D()
    mat2 scale2D_2x2(const vec2& fac);
    mat3 scale2D_3x3(const vec2& fac);

    // 3D scaling matrix (3x3)
    void scale3D(
        const vec3& fac,
        mat3* out_m,
        mat3* out_minv = nullptr,
        const bool overwrite = false
    );

    // 3D scaling matrix (4x4)
    void scale3D(
        const vec3& fac,
        mat4* out_m,
        mat4* out_minv = nullptr,
        const bool overwrite = false
    );

    // Shortened versions of scale3D()
    mat3 scale3D_3x3(const vec3& fac);
    mat4 scale3D_4x4(const vec3& fac);

    // 2D rotation matrix (2x2)
    void rotate2D(
        float theta,
        mat2* out_m,
        mat2* out_minv = nullptr,
        const bool overwrite = false
    );

    // 2D rotation matrix (3x3)
    void rotate2D(
        float theta,
        mat3* out_m,
        mat3* out_minv = nullptr,
        const bool overwrite = false
    );

    // Shortened versions of rotate2D()
    mat2 rotate2D_2x2(float theta);
    mat3 rotate2D_3x3(float theta);

    // 3D rotation matrix around the X axis (left-handed) (3x3)
    void rotate3D_x(
        float theta,
        mat3* out_m,
        mat3* out_minv = nullptr,
        const bool overwrite = false
    );

    // 3D rotation matrix around the X axis (left-handed) (4x4)
    void rotate3D_x(
        float theta,
        mat4* out_m,
        mat4* out_minv = nullptr,
        const bool overwrite = false
    );

    // Shortened versions of rotate3D_x()
    mat3 rotate3D_x_3x3(float theta);
    mat4 rotate3D_x_4x4(float theta);

    // 3D rotation matrix around the Y axis (left-handed) (3x3)
    void rotate3D_y(
        float theta,
        mat3* out_m,
        mat3* out_minv = nullptr,
        const bool overwrite = false
    );

    // 3D rotation matrix around the Y axis (left-handed) (4x4)
    void rotate3D_y(
        float theta,
        mat4* out_m,
        mat4* out_minv = nullptr,
        const bool overwrite = false
    );

    // Shortened versions of rotate3D_y()
    mat3 rotate3D_y_3x3(float theta);
    mat4 rotate3D_y_4x4(float theta);

    // 3D rotation matrix around the Z axis (left-handed) (3x3)
    void rotate3D_z(
        float theta,
        mat3* out_m,
        mat3* out_minv = nullptr,
        const bool overwrite = false
    );

    // 3D rotation matrix around the Z axis (left-handed) (4x4)
    void rotate3D_z(
        float theta,
        mat4* out_m,
        mat4* out_minv = nullptr,
        const bool overwrite = false
    );

    // Shortened versions of rotate3D_z()
    mat3 rotate3D_z_3x3(float theta);
    mat4 rotate3D_z_4x4(float theta);

    // 3D rotation matrix around an arbitrary axis (left-handed) (3x3)
    void rotate3D(
        float theta,
        const vec3& axis,
        mat3* out_m,
        mat3* out_minv = nullptr,
        const bool overwrite = false
    );

    // 3D rotation matrix around an arbitrary axis (left-handed) (4x4)
    void rotate3D(
        float theta,
        const vec3& axis,
        mat4* out_m,
        mat4* out_minv = nullptr,
        const bool overwrite = false
    );

    // Shortened versions of rotate3D()
    mat3 rotate3D_3x3(float theta, const vec3& axis);
    mat4 rotate3D_4x4(float theta, const vec3& axis);

    // 3D transformation from a left-handed viewing coordinate system where the camera is at the
    // origin looking along the +z axis, where the +y axis is along the up direction (4x4)
    void lookat3D(
        const vec3& pos,
        const vec3& look,
        const vec3& up,
        mat4* out_m,
        mat4* out_minv = nullptr,
        const bool overwrite = false
    );

    // Shortened version of lookat3D()
    mat4 lookat3D(const vec3& pos, const vec3& look, const vec3& up);

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
        return
            la2 < .999f || la2 > 1.001f ||
            lb2 < .999f || lb2 > 1.001f;
    }

    // Check if a 2D transformation matrix has a scaling term in it. (3x3)
    constexpr bool has_scale2D(const mat3& m)
    {
        float la2 = length_squared(apply_vector2D(m, vec2(1, 0)));
        float lb2 = length_squared(apply_vector2D(m, vec2(0, 1)));
        return
            la2 < .999f || la2 > 1.001f ||
            lb2 < .999f || lb2 > 1.001f;
    }

    // Check if a 3D transformation matrix has a scaling term in it. (3x3)
    constexpr bool has_scale3D(const mat3& m)
    {
        float la2 = length_squared(apply_vector3D(m, vec3(1, 0, 0)));
        float lb2 = length_squared(apply_vector3D(m, vec3(0, 1, 0)));
        float lc2 = length_squared(apply_vector3D(m, vec3(0, 0, 1)));
        return
            la2 < .999f || la2 > 1.001f ||
            lb2 < .999f || lb2 > 1.001f ||
            lc2 < .999f || lc2 > 1.001f;
    }

    // Check if a 3D transformation matrix has a scaling term in it. (4x4)
    constexpr bool has_scale3D(const mat4& m)
    {
        float la2 = length_squared(apply_vector3D(m, vec3(1, 0, 0)));
        float lb2 = length_squared(apply_vector3D(m, vec3(0, 1, 0)));
        float lc2 = length_squared(apply_vector3D(m, vec3(0, 0, 1)));
        return
            la2 < .999f || la2 > 1.001f ||
            lb2 < .999f || lb2 > 1.001f ||
            lc2 < .999f || lc2 > 1.001f;
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
