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

        constexpr mat4()
        {
            m[0][0] = m[1][1] = m[2][2] = m[3][3] = 1.f;
            m[0][1] = m[0][2] = m[0][3] =
                m[1][0] = m[1][2] = m[1][3] =
                m[2][0] = m[2][1] = m[2][3] =
                m[3][0] = m[3][1] = m[3][2] = 0.f;
        }

        constexpr mat4(float mat[4][4])
        {
            std::copy(&mat[0][0], &mat[0][0] + 16, &m[0][0]);
        }

        constexpr mat4(
            float m00, float m01, float m02, float m03,
            float m10, float m11, float m12, float m13,
            float m20, float m21, float m22, float m23,
            float m30, float m31, float m32, float m33
        )
        {
            m[0][0] = m00;
            m[0][1] = m01;
            m[0][2] = m02;
            m[0][3] = m03;

            m[1][0] = m10;
            m[1][1] = m11;
            m[1][2] = m12;
            m[1][3] = m13;

            m[2][0] = m20;
            m[2][1] = m21;
            m[2][2] = m22;
            m[2][3] = m23;

            m[3][0] = m30;
            m[3][1] = m31;
            m[3][2] = m32;
            m[3][3] = m33;
        }

        // String
        std::string to_string() const;

        // Print
        friend std::ostream& operator<<(std::ostream& os, const mat4& m);

        // this * other matrix
        constexpr mat4 operator*(const mat4& m2) const
        {
            mat4 r;
            for (int i = 0; i < 4; ++i)
            {
                for (int j = 0; j < 4; ++j)
                {
                    r.m[i][j] =
                        m[i][0] * m2.m[0][j] +
                        m[i][1] * m2.m[1][j] +
                        m[i][2] * m2.m[2][j] +
                        m[i][3] * m2.m[3][j];
                }
            }
            return r;
        }

        // this == other matrix
        constexpr bool operator==(const mat4& m2) const
        {
            for (int i = 0; i < 4; ++i)
                for (int j = 0; j < 4; ++j)
                    if (m[i][j] != m2.m[i][j])
                        return false;
            return true;
        }

        // this != other matrix
        constexpr bool operator!=(const mat4& m2) const
        {
            for (int i = 0; i < 4; ++i)
                for (int j = 0; j < 4; ++j)
                    if (m[i][j] != m2.m[i][j])
                        return true;
            return false;
        }

    };

    // Inverted copy of a matrix
    constexpr mat4 inverse(const mat4& m)
    {
        int indxc[4], indxr[4];
        int ipiv[4] = { 0, 0, 0, 0 };
        float minv[4][4];
        std::copy(&m.m[0][0], &m.m[0][0] + 16, &minv[0][0]);
        for (int i = 0; i < 4; i++)
        {
            int irow = 0, icol = 0;
            float big = 0.f;
            // Choose pivot
            for (int j = 0; j < 4; j++)
            {
                if (ipiv[j] != 1)
                {
                    for (int k = 0; k < 4; k++)
                    {
                        if (ipiv[k] == 0)
                        {
                            if (std::abs(minv[j][k]) >= big)
                            {
                                big = std::abs(minv[j][k]);
                                irow = j;
                                icol = k;
                            }
                        }
                        else if (ipiv[k] > 1)
                        {
                            // Singular matrix
                            // TODO: Log this error
                            return mat4();
                        }
                    }
                }
            }
            ++ipiv[icol];
            // Swap rows _irow_ and _icol_ for pivot
            if (irow != icol)
            {
                for (int k = 0; k < 4; ++k) std::swap(minv[irow][k], minv[icol][k]);
            }
            indxr[i] = irow;
            indxc[i] = icol;
            if (minv[icol][icol] == 0.f)
            {
                // Singular matrix
                // TODO: Log this error
                return mat4();
            }

            // Set $m[icol][icol]$ to one by scaling row _icol_ appropriately
            float pivinv = 1.f / minv[icol][icol];
            minv[icol][icol] = 1.f;
            for (int j = 0; j < 4; j++) minv[icol][j] *= pivinv;

            // Subtract this row from others to zero out their columns
            for (int j = 0; j < 4; j++)
            {
                if (j != icol)
                {
                    float save = minv[j][icol];
                    minv[j][icol] = 0;
                    for (int k = 0; k < 4; k++) minv[j][k] -= minv[icol][k] * save;
                }
            }
        }
    }

    // Transposed copy of a matrix
    constexpr mat4 transpose(const mat4& m)
    {
        return mat4(
            m.m[0][0], m.m[1][0], m.m[2][0], m.m[3][0],
            m.m[0][1], m.m[1][1], m.m[2][1], m.m[3][1],
            m.m[0][2], m.m[1][2], m.m[2][2], m.m[3][2],
            m.m[0][3], m.m[1][3], m.m[2][3], m.m[3][3]
        );
    }

}
