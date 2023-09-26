#pragma once

#include <array>

#include "utils.h"

#include "../str/utils.h"

namespace tef::math
{

    // Row-major matrix
    template <size_t n_row, size_t n_col>
    class base_mat
    {
    public:
        // Constructors

        constexpr base_mat()
        {
            for (size_t row = 0; row < n_row; row++)
            {
                for (size_t col = 0; col < n_col; col++)
                {
                    m[row][col] = (row == col) ? 1.f : 0.f;
                }
            }
        }

        constexpr base_mat(const float mat[n_row][n_col])
        {
            std::copy(&mat[0][0], &mat[0][0] + (n_row * n_col), &m[0][0]);
        }

        constexpr base_mat(const float* mat)
        {
            std::copy(mat, mat + (n_row * n_col), &m[0][0]);
        }

        constexpr base_mat(const std::array<float, n_row* n_col>& mat)
        {
            std::copy(mat.data(), mat.data() + (n_row * n_col), &m[0][0]);
        }

        // String
        std::string to_string() const
        {
            std::string s("[");
            for (size_t row = 0; row < n_row; row++)
            {
                s += (row > 0 ? " [ " : "[ ");
                for (size_t col = 0; col < n_col; col++)
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
        constexpr base_mat operator*(float s) const
        {
            base_mat r;
            for (size_t row = 0; row < n_row; row++)
            {
                for (size_t col = 0; col < n_col; col++)
                {
                    r(row, col) = (*this)(row, col) * s;
                }
            }
            return r;
        }

        // this *= scalar
        constexpr base_mat& operator*=(float s)
        {
            for (size_t row = 0; row < n_row; row++)
            {
                for (size_t col = 0; col < n_col; col++)
                {
                    (*this)(row, col) *= s;
                }
            }
            return *this;
        }

        // this / scalar
        constexpr base_mat operator/(float s) const
        {
            base_mat r;
            for (size_t row = 0; row < n_row; row++)
            {
                for (size_t col = 0; col < n_col; col++)
                {
                    r(row, col) = (*this)(row, col) / s;
                }
            }
            return r;
        }

        // this /= scalar
        constexpr base_mat& operator/=(float s)
        {
            for (size_t row = 0; row < n_row; row++)
            {
                for (size_t col = 0; col < n_col; col++)
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
            for (size_t row = 0; row < n_row; row++)
            {
                for (size_t col = 0; col < n_col; col++)
                {
                    r(row, col) = (*this)(row, col) + m(row, col);
                }
            }
            return r;
        }

        // this += matrix
        constexpr base_mat& operator+=(const base_mat& m)
        {
            for (size_t row = 0; row < n_row; row++)
            {
                for (size_t col = 0; col < n_col; col++)
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
            for (size_t row = 0; row < n_row; row++)
            {
                for (size_t col = 0; col < n_col; col++)
                {
                    r(row, col) = (*this)(row, col) - m(row, col);
                }
            }
            return r;
        }

        // this -= matrix
        constexpr base_mat& operator-=(const base_mat& m)
        {
            for (size_t row = 0; row < n_row; row++)
            {
                for (size_t col = 0; col < n_col; col++)
                {
                    (*this)(row, col) -= m(row, col);
                }
            }
            return *this;
        }

        // this * matrix
        template <size_t n2>
        constexpr base_mat<n_row, n2> operator*(const base_mat<n_col, n2>& m) const
        {
            base_mat<n_row, n2> r;
            for (size_t row = 0; row < n_row; row++)
            {
                for (size_t col = 0; col < n2; col++)
                {
                    float dot = 0.f;
                    for (size_t i = 0; i < n_col; i++)
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
            for (size_t row = 0; row < n_row; row++)
            {
                for (size_t col = 0; col < n_col; col++)
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
            for (size_t row = 0; row < n_row; row++)
            {
                for (size_t col = 0; col < n_col; col++)
                {
                    if (m[row][col] != m2.m[row][col])
                        return true;
                }
            }
            return false;
        }

        // Access by 1D index (copy)
        constexpr float operator()(size_t index) const
        {
            return (&m[0][0])[index];
        }

        // Access by 1D index (reference)
        constexpr float& operator()(size_t index)
        {
            return (&m[0][0])[index];
        }

        // Access by indices (copy)
        constexpr float operator()(size_t row, size_t col) const
        {
            return m[row][col];
        }

        // Access by indices (reference)
        constexpr float& operator()(size_t row, size_t col)
        {
            return m[row][col];
        }

        // Sub-matrix
        // Note: The indices are inclusive. For example, if the start row and column indices are
        // both 0, and the end row and column indices are both 2, this function will return the
        // upper-left 3x3 portion.
        // Note: The value of end_row must not be smaller than the value of start_row, and the
        // same goes for start_col and end_col.
        template <size_t start_row, size_t start_col, size_t end_row, size_t end_col>
        constexpr base_mat<end_row - start_row + 1, end_col - start_col + 1> sub() const
        {
            base_mat<end_row - start_row + 1, end_col - start_col + 1> r;
            for (size_t row = start_row; row <= end_row; row++)
            {
                for (size_t col = start_col; col <= end_col; col++)
                {
                    r(row - start_row, col - start_col) = (*this)(row, col);
                }
            }
        }

        // Upper-left n x m sub-matrix
        // Note: n must be smaller than or equal to n_row.
        // Note: m must be smaller than or equal to n_col.
        template <size_t n, size_t m>
        constexpr base_mat<n, m> sub() const
        {
            base_mat<n, m> r;
            for (size_t row = 0; row < n; row++)
            {
                for (size_t col = 0; col < m; col++)
                {
                    r(row, col) = (*this)(row, col);
                }
            }
        }

        // Upper-left n x n sub-matrix
        // Note: n must be smaller than n_row and n_col.
        template <size_t n>
        constexpr base_mat<n, n> sub() const
        {
            base_mat<n, n> r;
            for (size_t row = 0; row < n; row++)
            {
                for (size_t col = 0; col < n; col++)
                {
                    r(row, col) = (*this)(row, col);
                }
            }
        }

        // Total number of elements (n_rows * n_col)
        constexpr size_t n_total() const
        {
            return n_row * n_col;
        }

    private:
        float m[n_row][n_col];

    };

    // Scalar * matrix
    template <size_t n_row, size_t n_col>
    constexpr base_mat<n_row, n_col> operator*(float s, const base_mat<n_row, n_col>& m)
    {
        return m * s;
    }

    template <size_t n>
    constexpr bool is_identity(const base_mat<n, n>& m)
    {
        for (size_t row = 0; row < n; row++)
        {
            for (size_t col = 0; col < n; col++)
            {
                float expected = (row == col) ? 1.f : 0.f;
                if (m(row, col) != expected)
                    return false;
            }
        }
        return true;
    }

    // Cofactor of m[p][q]
    // https://www.geeksforgeeks.org/adjoint-inverse-matrix/
    template <size_t n>
    constexpr base_mat<n - 1, n - 1> cofactor(const base_mat<n, n>& m, size_t p, size_t q)
    {
        size_t i = 0, j = 0;
        base_mat<n - 1, n - 1> r;
        for (size_t row = 0; row < n; row++)
        {
            for (size_t col = 0; col < n; col++)
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
    template <size_t n>
    constexpr float determinant(base_mat<n, n> m)
    {
        if constexpr (n == 1)
            return m(0, 0);

        float det = 1;
        float total = 1;

        // Temporary array for storing row
        float temp[n + 1]{};

        // Traverse the diagonal elements
        for (size_t i = 0; i < n; i++)
        {
            size_t index = i;

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
                for (size_t j = 0; j < n; j++)
                {
                    std::swap(m(index, j), m(i, j));
                }

                // The determinant sign changes when we shift rows
                det *= (index - i) % 2 == 0 ? 1.f : -1.f;
            }

            // Store the diagonal row elements
            for (size_t j = 0; j < n; j++)
            {
                temp[j] = m(i, j);
            }

            // Traverse every row below the diagonal element
            for (size_t j = i + 1; j < n; j++)
            {
                // Value of diagonal element
                float num1 = temp[i];

                // Value of next row element
                float num2 = m(j, i);

                // Traverse every column of row and multiply to every row
                for (size_t k = 0; k < n; k++)
                {
                    // Multiply to make the diagonal element and next row element equal
                    m(j, k) = (num1 * m(j, k)) - (num2 * temp[k]);
                }

                // Det(kA)=kDet(A);
                total *= num1;
            }
        }

        // Multiply the diagonal elements to get the determinant
        for (size_t i = 0; i < n; i++)
        {
            det *= m(i, i);
        }

        // Det(kA)/k=Det(A);
        return (det / total);
    }

    // Adjoint of a square matrix
    // https://www.geeksforgeeks.org/adjoint-inverse-matrix/
    template <size_t n>
    constexpr base_mat<n, n> adjoint(const base_mat<n, n>& m)
    {
        if constexpr (n == 1)
            return base_mat<n, n>();

        base_mat<n, n> r;
        for (size_t i = 0; i < n; i++)
        {
            for (size_t j = 0; j < n; j++)
            {
                // Get cofactor of m[i][j]
                base_mat<n - 1, n - 1> cf = cofactor(m, i, j);

                // Sign of adj[j][i] positive if the sum of the row and column indices is even.
                float sign = ((i + j) % 2 == 0) ? 1.f : -1.f;

                // Interchanging rows and columns to get the transpose of the cofactor matrix
                r(j, i) = sign * determinant(cf);
            }
        }
        return r;
    }

    // Inverted copy of a square matrix
    template <size_t n>
    constexpr base_mat<n, n> inverse(const base_mat<n, n>& m, bool* out_invertible = nullptr)
    {
        // Determinant
        float det = determinant(m);
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
    template <size_t n_row, size_t n_col>
    constexpr base_mat<n_col, n_row> transpose(const base_mat<n_row, n_col>& m)
    {
        base_mat<n_col, n_row> r;
        for (size_t row = 0; row < n_row; row++)
        {
            for (size_t col = 0; col < n_col; col++)
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
