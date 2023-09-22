#pragma once

#include "utils.h"

#include "../str/utils.h"

namespace tef::math
{

    class mat4
    {
    public:
        float m[4][4];

        // Constructors
        mat4();
        mat4(float mat[4][4]);
        mat4(float m00, float m01, float m02, float m03,
            float m10, float m11, float m12, float m13,
            float m20, float m21, float m22, float m23,
            float m30, float m31, float m32, float m33);

        // String
        operator std::string() const;

        // Print
        friend std::ostream& operator<<(std::ostream& os, const mat4& m);

        // this * other matrix
        mat4 operator*(const mat4& m2) const;

        // this == other matrix
        bool operator==(const mat4& m2) const;

        // this != other matrix
        bool operator!=(const mat4& m2) const;

    };

    // Inverted copy of a matrix
    mat4 inverse(const mat4& m);

    // Transposed copy of a matrix
    mat4 transpose(const mat4& m);

}
