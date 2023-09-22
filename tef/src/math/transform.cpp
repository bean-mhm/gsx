#include "transform.h"

namespace tef::math
{

    transform_t::transform_t()
        : m(), minv()
    {}

    transform_t::transform_t(const float mat[4][4])
    {
        m = mat4(
            mat[0][0], mat[0][1], mat[0][2], mat[0][3],
            mat[1][0], mat[1][1], mat[1][2], mat[1][3],
            mat[2][0], mat[2][1], mat[2][2], mat[2][3],
            mat[3][0], mat[3][1], mat[3][2], mat[3][3]
        );
        minv = inverse(m);
    }

    transform_t::transform_t(const mat4& m)
        : m(m), minv(inverse(m))
    {}

    transform_t::transform_t(const mat4& m, const mat4& minv)
        : m(m), minv(minv)
    {}

    transform_t::operator std::string() const
    {
        return std::string(m);
    }

    std::ostream& operator<<(std::ostream& os, const transform_t& t)
    {
        os << std::string(t);
        return os;
    }

    transform_t transform_t::operator*(const transform_t& t2) const
    {
        return transform_t(m * t2.m, t2.minv * minv);
    }

    bool transform_t::operator==(const transform_t& t) const
    {
        return t.m == m && t.minv == minv;
    }

    bool transform_t::operator!=(const transform_t& t) const
    {
        return t.m != m || t.minv != minv;
    }

    bool transform_t::operator<(const transform_t& t) const
    {
        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 4; ++j)
            {
                if (m.m[i][j] < t.m.m[i][j]) return true;
                if (m.m[i][j] > t.m.m[i][j]) return false;
            }
        return false;
    }

    bool transform_t::is_identity() const
    {
        return
            m.m[0][0] == 1.f && m.m[0][1] == 0.f && m.m[0][2] == 0.f && m.m[0][3] == 0.f &&
            m.m[1][0] == 0.f && m.m[1][1] == 1.f && m.m[1][2] == 0.f && m.m[1][3] == 0.f &&
            m.m[2][0] == 0.f && m.m[2][1] == 0.f && m.m[2][2] == 1.f && m.m[2][3] == 0.f &&
            m.m[3][0] == 0.f && m.m[3][1] == 0.f && m.m[3][2] == 0.f && m.m[3][3] == 1.f;
    }

    bool transform_t::has_scale() const
    {
        float la2 = length_squared(apply_vector(vec3(1, 0, 0)));
        float lb2 = length_squared(apply_vector(vec3(0, 1, 0)));
        float lc2 = length_squared(apply_vector(vec3(0, 0, 1)));
        return
            (la2) < .999f || (la2) > 1.001f ||
            (lb2) < .999f || (lb2) > 1.001f ||
            (lc2) < .999f || (lc2) > 1.001f;
    }

    bool transform_t::swaps_handedness() const
    {
        // Determinant of the transformation's upper-left 3x3 submatrix
        float det =
            m.m[0][0] * (m.m[1][1] * m.m[2][2] - m.m[1][2] * m.m[2][1]) -
            m.m[0][1] * (m.m[1][0] * m.m[2][2] - m.m[1][2] * m.m[2][0]) +
            m.m[0][2] * (m.m[1][0] * m.m[2][1] - m.m[1][1] * m.m[2][0]);
        return det < 0;
    }

    vec3 transform_t::apply_point(const vec3& p) const
    {
        float xp = m.m[0][0] * p.x + m.m[0][1] * p.y + m.m[0][2] * p.z + m.m[0][3];
        float yp = m.m[1][0] * p.x + m.m[1][1] * p.y + m.m[1][2] * p.z + m.m[1][3];
        float zp = m.m[2][0] * p.x + m.m[2][1] * p.y + m.m[2][2] * p.z + m.m[2][3];
        float wp = m.m[3][0] * p.x + m.m[3][1] * p.y + m.m[3][2] * p.z + m.m[3][3];
        if (wp == 1)
            return vec3(xp, yp, zp);
        else
            return vec3(xp, yp, zp) / wp;
    }

    vec3 transform_t::apply_vector(const vec3& v) const
    {
        return vec3(
            m.m[0][0] * v.x + m.m[0][1] * v.y + m.m[0][2] * v.z,
            m.m[1][0] * v.x + m.m[1][1] * v.y + m.m[1][2] * v.z,
            m.m[2][0] * v.x + m.m[2][1] * v.y + m.m[2][2] * v.z
        );
    }

    vec3 transform_t::apply_normal(const vec3& n) const
    {
        return vec3(
            minv.m[0][0] * n.x + minv.m[1][0] * n.y + minv.m[2][0] * n.z,
            minv.m[0][1] * n.x + minv.m[1][1] * n.y + minv.m[2][1] * n.z,
            minv.m[0][2] * n.x + minv.m[1][2] * n.y + minv.m[2][2] * n.z
        );
    }

    ray_t transform_t::apply_ray(const ray_t& r) const
    {
        ray_t ret = r;
        ret.o = apply_point(ret.o);
        ret.d = apply_vector(ret.d);
        return ret;
    }

    bounds3 transform_t::apply_bounds(const bounds3& b) const
    {
        bounds3 ret(apply_point(b.pmin));
        ret = union_(ret, apply_point(vec3(b.pmax.x, b.pmin.y, b.pmin.z)));
        ret = union_(ret, apply_point(vec3(b.pmin.x, b.pmax.y, b.pmin.z)));
        ret = union_(ret, apply_point(vec3(b.pmin.x, b.pmin.y, b.pmax.z)));
        ret = union_(ret, apply_point(vec3(b.pmin.x, b.pmax.y, b.pmax.z)));
        ret = union_(ret, apply_point(vec3(b.pmax.x, b.pmax.y, b.pmin.z)));
        ret = union_(ret, apply_point(vec3(b.pmax.x, b.pmin.y, b.pmax.z)));
        ret = union_(ret, apply_point(vec3(b.pmax.x, b.pmax.y, b.pmax.z)));
        return ret;
    }

    transform_t transform_t::translate(const vec3& delta)
    {
        mat4 m(
            1, 0, 0, delta.x,
            0, 1, 0, delta.y,
            0, 0, 1, delta.z,
            0, 0, 0, 1);
        mat4 minv(
            1, 0, 0, -delta.x,
            0, 1, 0, -delta.y,
            0, 0, 1, -delta.z,
            0, 0, 0, 1);
        return transform_t(m, minv);
    }

    transform_t transform_t::scale(const vec3& fac)
    {
        mat4 m(
            fac.x, 0, 0, 0,
            0, fac.y, 0, 0,
            0, 0, fac.z, 0,
            0, 0, 0, 1);
        mat4 minv(
            1 / fac.x, 0, 0, 0,
            0, 1 / fac.y, 0, 0,
            0, 0, 1 / fac.z, 0,
            0, 0, 0, 1);
        return transform_t(m, minv);
    }

    transform_t transform_t::rotate_x(float theta)
    {
        theta *= deg2rad;
        float sin_theta = std::sin(theta);
        float cos_theta = std::cos(theta);
        mat4 m(
            1, 0, 0, 0,
            0, cos_theta, -sin_theta, 0,
            0, sin_theta, cos_theta, 0,
            0, 0, 0, 1
        );
        return transform_t(m, transpose(m));
    }

    transform_t transform_t::rotate_y(float theta)
    {
        theta *= deg2rad;
        float sin_theta = std::sin(theta);
        float cos_theta = std::cos(theta);
        mat4 m(
            cos_theta, 0, sin_theta, 0,
            0, 1, 0, 0,
            -sin_theta, 0, cos_theta, 0,
            0, 0, 0, 1
        );
        return transform_t(m, transpose(m));
    }

    transform_t transform_t::rotate_z(float theta)
    {
        theta *= deg2rad;
        float sin_theta = std::sin(theta);
        float cos_theta = std::cos(theta);
        mat4 m(
            cos_theta, -sin_theta, 0, 0,
            sin_theta, cos_theta, 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1
        );
        return transform_t(m, transpose(m));
    }

    transform_t transform_t::rotate(float theta, const vec3& axis)
    {
        theta *= deg2rad;
        vec3 a = normalize(axis);
        float sin_theta = std::sin(theta);
        float cos_theta = std::cos(theta);

        mat4 m;

        // Compute rotation of first basis vector
        m.m[0][0] = a.x * a.x + (1 - a.x * a.x) * cos_theta;
        m.m[0][1] = a.x * a.y * (1 - cos_theta) - a.z * sin_theta;
        m.m[0][2] = a.x * a.z * (1 - cos_theta) + a.y * sin_theta;
        m.m[0][3] = 0;

        // Second basis vector
        m.m[1][0] = a.x * a.y * (1 - cos_theta) + a.z * sin_theta;
        m.m[1][1] = a.y * a.y + (1 - a.y * a.y) * cos_theta;
        m.m[1][2] = a.y * a.z * (1 - cos_theta) - a.x * sin_theta;
        m.m[1][3] = 0;

        // Third basis vector
        m.m[2][0] = a.x * a.z * (1 - cos_theta) - a.y * sin_theta;
        m.m[2][1] = a.y * a.z * (1 - cos_theta) + a.x * sin_theta;
        m.m[2][2] = a.z * a.z + (1 - a.z * a.z) * cos_theta;
        m.m[2][3] = 0;

        return transform_t(m, transpose(m));
    }

    transform_t transform_t::look_at(const vec3& pos, const vec3& look, const vec3& up)
    {
        mat4 cam_to_world;

        // Initialize fourth column of viewing matrix
        cam_to_world.m[0][3] = pos.x;
        cam_to_world.m[1][3] = pos.y;
        cam_to_world.m[2][3] = pos.z;
        cam_to_world.m[3][3] = 1;

        // Initialize first three columns of viewing matrix
        vec3 dir = normalize(look - pos);
        vec3 right = normalize(cross(normalize(up), dir));
        vec3 new_up = cross(dir, right);
        cam_to_world.m[0][0] = right.x;
        cam_to_world.m[1][0] = right.y;
        cam_to_world.m[2][0] = right.z;
        cam_to_world.m[3][0] = 0.;
        cam_to_world.m[0][1] = new_up.x;
        cam_to_world.m[1][1] = new_up.y;
        cam_to_world.m[2][1] = new_up.z;
        cam_to_world.m[3][1] = 0.;
        cam_to_world.m[0][2] = dir.x;
        cam_to_world.m[1][2] = dir.y;
        cam_to_world.m[2][2] = dir.z;
        cam_to_world.m[3][2] = 0.;

        return transform_t(inverse(cam_to_world), cam_to_world);
    }

    transform_t transpose(const transform_t& t)
    {
        return transform_t(transpose(t.m), transpose(t.minv));
    }

    transform_t inverse(const transform_t& t)
    {
        return transform_t(t.minv, t.m);
    }

}
