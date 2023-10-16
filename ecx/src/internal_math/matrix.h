#pragma once

// STD
#include <array>
#include <cstdint>

// Internal
#include "utils.h"
#include "../internal_common/all.h"
#include "../internal_str/all.h"

namespace ecx::math
{

    // Row-major matrix
    template<i32 n_row, i32 n_col>
    class base_mat
    {
    public:
        // Constructors

        constexpr base_mat()
        {
            for (i32 row = 0; row < n_row; row++)
            {
                for (i32 col = 0; col < n_col; col++)
                {
                    m[row][col] = (row == col) ? 1.f : 0.f;
                }
            }
        }

        constexpr base_mat(const f32 mat[n_row][n_col])
        {
            std::copy(&mat[0][0], &mat[0][0] + (n_row * n_col), &m[0][0]);
        }

        constexpr base_mat(const f32* mat)
        {
            std::copy(mat, mat + (n_row * n_col), &m[0][0]);
        }

        constexpr base_mat(const std::array<f32, n_row* n_col>& mat)
        {
            std::copy(mat.data(), mat.data() + (n_row * n_col), &m[0][0]);
        }

        // String
        std::string to_string() const
        {
            std::string s("[");
            for (i32 row = 0; row < n_row; row++)
            {
                s += (row > 0 ? " [ " : "[ ");
                for (i32 col = 0; col < n_col; col++)
                {
                    s += str::from_number(m[row][col]);
                    if (col != n_col - 1)
                        s += "  ";
                }
                s += (row != n_row - 1 ? " ]\n" : " ]]");
            }
            return s;
        }

        // Print
        friend std::ostream& operator<<(std::ostream& os, const base_mat& m)
        {
            os << m.to_string();
            return os;
        }

        // this * scalar
        constexpr base_mat operator*(f32 s) const
        {
            base_mat r;
            for (i32 row = 0; row < n_row; row++)
            {
                for (i32 col = 0; col < n_col; col++)
                {
                    r(row, col) = (*this)(row, col) * s;
                }
            }
            return r;
        }

        // this *= scalar
        constexpr base_mat& operator*=(f32 s)
        {
            for (i32 row = 0; row < n_row; row++)
            {
                for (i32 col = 0; col < n_col; col++)
                {
                    (*this)(row, col) *= s;
                }
            }
            return *this;
        }

        // this / scalar
        constexpr base_mat operator/(f32 s) const
        {
            base_mat r;
            for (i32 row = 0; row < n_row; row++)
            {
                for (i32 col = 0; col < n_col; col++)
                {
                    r(row, col) = (*this)(row, col) / s;
                }
            }
            return r;
        }

        // this /= scalar
        constexpr base_mat& operator/=(f32 s)
        {
            for (i32 row = 0; row < n_row; row++)
            {
                for (i32 col = 0; col < n_col; col++)
                {
                    (*this)(row, col) /= s;
                }
            }
            return *this;
        }

        // this + matrix
        constexpr base_mat operator+(const base_mat& m) const
        {
            base_mat r;
            for (i32 row = 0; row < n_row; row++)
            {
                for (i32 col = 0; col < n_col; col++)
                {
                    r(row, col) = (*this)(row, col) + m(row, col);
                }
            }
            return r;
        }

        // this += matrix
        constexpr base_mat& operator+=(const base_mat& m)
        {
            for (i32 row = 0; row < n_row; row++)
            {
                for (i32 col = 0; col < n_col; col++)
                {
                    (*this)(row, col) += m(row, col);
                }
            }
            return *this;
        }

        // this - matrix
        constexpr base_mat operator-(const base_mat& m) const
        {
            base_mat r;
            for (i32 row = 0; row < n_row; row++)
            {
                for (i32 col = 0; col < n_col; col++)
                {
                    r(row, col) = (*this)(row, col) - m(row, col);
                }
            }
            return r;
        }

        // this -= matrix
        constexpr base_mat& operator-=(const base_mat& m)
        {
            for (i32 row = 0; row < n_row; row++)
            {
                for (i32 col = 0; col < n_col; col++)
                {
                    (*this)(row, col) -= m(row, col);
                }
            }
            return *this;
        }

        // this * matrix
        template<i32 n2>
        constexpr base_mat<n_row, n2> operator*(const base_mat<n_col, n2>& m) const
        {
            base_mat<n_row, n2> r;
            for (i32 row = 0; row < n_row; row++)
            {
                for (i32 col = 0; col < n2; col++)
                {
                    f32 dot = 0.f;
                    for (i32 i = 0; i < n_col; i++)
                    {
                        dot += (*this)(row, i) * m(i, col);
                    }
                    r(row, col) = dot;
                }
            }
            return r;
        }

        // this == other matrix
        constexpr bool operator==(const base_mat& m2) const
        {
            for (i32 row = 0; row < n_row; row++)
            {
                for (i32 col = 0; col < n_col; col++)
                {
                    if (m[row][col] != m2.m[row][col])
                        return false;
                }
            }
            return true;
        }

        // this != other matrix
        constexpr bool operator!=(const base_mat& m2) const
        {
            for (i32 row = 0; row < n_row; row++)
            {
                for (i32 col = 0; col < n_col; col++)
                {
                    if (m[row][col] != m2.m[row][col])
                        return true;
                }
            }
            return false;
        }

        // Access by 1D index (copy)
        constexpr f32 operator()(i32 index) const
        {
            return (&m[0][0])[index];
        }

        // Access by 1D index (reference)
        constexpr f32& operator()(i32 index)
        {
            return (&m[0][0])[index];
        }

        // Access by indices (copy)
        constexpr f32 operator()(i32 row, i32 col) const
        {
            return m[row][col];
        }

        // Access by indices (reference)
        constexpr f32& operator()(i32 row, i32 col)
        {
            return m[row][col];
        }

        // Sub-matrix
        // Note: The indices are inclusive. For example, if the start row and column indices are
        // both 0, and the end row and column indices are both 2, this function will return the
        // upper-left 3x3 portion.
        // Note: The value of end_row must not be smaller than the value of start_row, and the
        // same goes for start_col and end_col.
        template<i32 start_row, i32 start_col, i32 end_row, i32 end_col>
        constexpr base_mat<end_row - start_row + 1, end_col - start_col + 1> sub() const
        {
            base_mat<end_row - start_row + 1, end_col - start_col + 1> r;
            for (i32 row = start_row; row <= end_row; row++)
            {
                for (i32 col = start_col; col <= end_col; col++)
                {
                    r(row - start_row, col - start_col) = (*this)(row, col);
                }
            }
        }

        // Upper-left n x m sub-matrix
        // Note: n must be smaller than or equal to n_row.
        // Note: m must be smaller than or equal to n_col.
        template<i32 n, i32 m>
        constexpr base_mat<n, m> sub() const
        {
            base_mat<n, m> r;
            for (i32 row = 0; row < n; row++)
            {
                for (i32 col = 0; col < m; col++)
                {
                    r(row, col) = (*this)(row, col);
                }
            }
        }

        // Upper-left n x n sub-matrix
        // Note: n must be smaller than n_row and n_col.
        template<i32 n>
        constexpr base_mat<n, n> sub() const
        {
            base_mat<n, n> r;
            for (i32 row = 0; row < n; row++)
            {
                for (i32 col = 0; col < n; col++)
                {
                    r(row, col) = (*this)(row, col);
                }
            }
        }

        // Number of rows
        constexpr i32 n_rows()
        {
            return n_row;
        }

        // Number of columns
        constexpr i32 n_cols()
        {
            return n_col;
        }

        // Number of elements (n_row * n_col)
        constexpr i32 n_elements() const
        {
            return n_row * n_col;
        }

    private:
        f32 m[n_row][n_col];

    };

    // Scalar * matrix
    template<i32 n_row, i32 n_col>
    constexpr base_mat<n_row, n_col> operator*(f32 s, const base_mat<n_row, n_col>& m)
    {
        return m * s;
    }

    template<i32 n>
    constexpr bool is_identity(const base_mat<n, n>& m)
    {
        for (i32 row = 0; row < n; row++)
        {
            for (i32 col = 0; col < n; col++)
            {
                f32 expected = (row == col) ? 1.f : 0.f;
                if (m(row, col) != expected)
                    return false;
            }
        }
        return true;
    }

    // Cofactor of m[p][q]
    // https://www.geeksforgeeks.org/adjoint-inverse-matrix/
    template<i32 n>
    constexpr base_mat<n - 1, n - 1> cofactor(const base_mat<n, n>& m, i32 p, i32 q)
    {
        i32 i = 0, j = 0;
        base_mat<n - 1, n - 1> r;
        for (i32 row = 0; row < n; row++)
        {
            for (i32 col = 0; col < n; col++)
            {
                // Copy into the result matrix only those elements which are not in the current
                // row and column
                if (row != p && col != q)
                {
                    r(i, j++) = m(row, col);

                    // Row is filled, so increase the row index and reset the col index
                    if (j == n - 1)
                    {
                        j = 0;
                        i++;
                    }
                }
            }
        }
        return r;
    }

    // Determinant of a square matrix
    // https://www.geeksforgeeks.org/determinant-of-a-matrix/
    template<i32 n>
    constexpr f32 determinant(base_mat<n, n> m)
    {
        if constexpr (n == 1)
            return m(0, 0);

        f32 det = 1;
        f32 total = 1;

        // Temporary array for storing row
        f32 temp[n + 1]{};

        // Traverse the diagonal elements
        for (i32 i = 0; i < n; i++)
        {
            i32 index = i;

            // Find the index which has non zero value
            while (index < n && m(index, i) == 0)
            {
                index++;
            }

            if (index == n)
            {
                continue;
            }

            if (index != i)
            {
                // Swap the diagonal element row and index row
                for (i32 j = 0; j < n; j++)
                {
                    std::swap(m(index, j), m(i, j));
                }

                // The determinant sign changes when we shift rows
                det *= (index - i) % 2 == 0 ? 1.f : -1.f;
            }

            // Store the diagonal row elements
            for (i32 j = 0; j < n; j++)
            {
                temp[j] = m(i, j);
            }

            // Traverse every row below the diagonal element
            for (i32 j = i + 1; j < n; j++)
            {
                // Value of diagonal element
                f32 num1 = temp[i];

                // Value of next row element
                f32 num2 = m(j, i);

                // Traverse every column of row and multiply to every row
                for (i32 k = 0; k < n; k++)
                {
                    // Multiply to make the diagonal element and next row element equal
                    m(j, k) = (num1 * m(j, k)) - (num2 * temp[k]);
                }

                // Det(kA)=kDet(A);
                total *= num1;
            }
        }

        // Multiply the diagonal elements to get the determinant
        for (i32 i = 0; i < n; i++)
        {
            det *= m(i, i);
        }

        // Det(kA)/k=Det(A);
        return (det / total);
    }

    // Adjoint of a square matrix
    // https://www.geeksforgeeks.org/adjoint-inverse-matrix/
    template<i32 n>
    constexpr base_mat<n, n> adjoint(const base_mat<n, n>& m)
    {
        if constexpr (n == 1)
            return base_mat<n, n>();

        base_mat<n, n> r;
        for (i32 i = 0; i < n; i++)
        {
            for (i32 j = 0; j < n; j++)
            {
                // Get cofactor of m[i][j]
                base_mat<n - 1, n - 1> cf = cofactor(m, i, j);

                // Sign of adj[j][i] positive if the sum of the row and column indices is even.
                f32 sign = ((i + j) % 2 == 0) ? 1.f : -1.f;

                // Interchanging rows and columns to get the transpose of the cofactor matrix
                r(j, i) = sign * determinant(cf);
            }
        }
        return r;
    }

    // Inverted copy of a square matrix
    template<i32 n>
    constexpr base_mat<n, n> inverse(const base_mat<n, n>& m, bool* out_invertible = nullptr)
    {
        // Determinant
        f32 det = determinant(m);
        if (det == 0)
        {
            if (out_invertible)
                *out_invertible = false;
            return base_mat<n, n>();
        }

        // Adjoint
        base_mat<n, n> adj = adjoint(m);

        // inverse(A) = adj(A)/det(A)
        if (out_invertible)
            *out_invertible = true;
        return adj / det;
    }

    // Transposed copy of a matrix
    template<i32 n_row, i32 n_col>
    constexpr base_mat<n_col, n_row> transpose(const base_mat<n_row, n_col>& m)
    {
        base_mat<n_col, n_row> r;
        for (i32 row = 0; row < n_row; row++)
        {
            for (i32 col = 0; col < n_col; col++)
            {
                r(col, row) = m(row, col);
            }
        }
        return r;
    }

    // Type definitions

    using mat1x2 = base_mat<1, 2>;
    using mat2x1 = base_mat<2, 1>;

    using mat1x3 = base_mat<1, 3>;
    using mat3x1 = base_mat<3, 1>;

    using mat1x4 = base_mat<1, 4>;
    using mat4x1 = base_mat<4, 1>;

    using mat2 = base_mat<2, 2>;
    using mat2x2 = base_mat<2, 2>;

    using mat2x3 = base_mat<2, 3>;
    using mat3x2 = base_mat<3, 2>;

    using mat2x4 = base_mat<2, 4>;
    using mat4x2 = base_mat<4, 2>;

    using mat3 = base_mat<3, 3>;
    using mat3x3 = base_mat<3, 3>;

    using mat3x4 = base_mat<3, 4>;
    using mat4x3 = base_mat<4, 3>;

    using mat4 = base_mat<4, 4>;
    using mat4x4 = base_mat<4, 4>;

}
