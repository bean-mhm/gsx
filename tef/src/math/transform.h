#pragma once

#include "vec3.h"
#include "mat4.h"
#include "ray.h"
#include "bounds3.h"
#include "utils.h"

namespace tef::math
{

    class transform_t
    {
    public:
        mat4 m, minv;

        // Constructors
        transform_t();
        transform_t(const float mat[4][4]);
        transform_t(const mat4& m);
        transform_t(const mat4& m, const mat4& minv);

        // String
        operator std::string() const;

        // Print
        friend std::ostream& operator<<(std::ostream& os, const transform_t& t);

        // this * other transform
        transform_t operator*(const transform_t& t2) const;

        // this == other transform
        bool operator==(const transform_t& t) const;

        // this != other transform
        bool operator!=(const transform_t& t) const;

        // this < other transform
        bool operator<(const transform_t& t) const;

        // Check if m is the identity matrix
        bool is_identity() const;

        // Check if the transformation has a scaling term in it
        bool has_scale() const;

        // Check if handedness is changed by this transformation
        bool swaps_handedness() const;

        // Transform a 3D point
        vec3 apply_point(const vec3& p) const;

        // Transform a 3D vector
        vec3 apply_vector(const vec3& v) const;

        // Transform a 3D normal vector
        vec3 apply_normal(const vec3& n) const;

        // Transform a ray
        ray_t apply_ray(const ray_t& r) const;

        // Transform a 3D AABB
        bounds3 apply_bounds(const bounds3& b) const;

        // Generate a translation matrix
        static transform_t translate(const vec3& delta);

        // Generate a scaling matrix
        static transform_t scale(const vec3& fac);

        // Generate a rotation matrix around the X axis
        static transform_t rotate_x(float theta);

        // Generate a rotation matrix around the Y axis
        static transform_t rotate_y(float theta);

        // Generate a rotation matrix around the Z axis
        static transform_t rotate_z(float theta);

        // Generate a rotation matrix around an arbitrary axis
        static transform_t rotate(float theta, const vec3& axis);

        // A transformation from a left-handed viewing coordinate system where the camera is at
        // the origin looking down the +z axis, and the +y axis is along the up direction.
        static transform_t look_at(const vec3& pos, const vec3& look, const vec3& up);

    };

    // Transposed copy of a transform
    transform_t transpose(const transform_t& t);

    // Inverted copy of a transform
    transform_t inverse(const transform_t& t);

}
