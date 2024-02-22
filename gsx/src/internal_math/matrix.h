#pragma once

#include <array>
#include <cstdint>

#include "utils.h"
#include "../internal_common/all.h"
#include "../internal_str/all.h"

namespace gsx::math
{

    // row-major matrix
    template<typename T, i32 n_row, i32 n_col>
    class base_mat
    {
    public:
        constexpr base_mat()
        {
            for (i32 row = 0; row < n_row; row++)
            {
                for (i32 col = 0; col < n_col; col++)
                {
                    m[row][col] = (row == col) ? (T)1 : (T)0;
                }
            }
        }

        constexpr base_mat(const T mat[n_row][n_col])
        {
            std::copy(&mat[0][0], &mat[0][0] + (n_row * n_col), &m[0][0]);
        }

        constexpr base_mat(const T* mat)
        {
            std::copy(mat, mat + (n_row * n_col), &m[0][0]);
        }

        constexpr base_mat(const std::array<T, n_row* n_col>& mat)
        {
            std::copy(mat.data(), mat.data() + (n_row * n_col), &m[0][0]);
        }

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

        friend std::ostream& operator<<(std::ostream& os, const base_mat& m)
        {
            os << m.to_string();
            return os;
        }

        constexpr base_mat operator*(T s) const
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

        constexpr base_mat& operator*=(T s)
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

        constexpr base_mat operator/(T s) const
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

        constexpr base_mat& operator/=(T s)
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

        template<i32 n2>
        constexpr base_mat<T, n_row, n2> operator*(
            const base_mat<T, n_col, n2>& m
            ) const
        {
            base_mat<T, n_row, n2> r;
            for (i32 row = 0; row < n_row; row++)
            {
                for (i32 col = 0; col < n2; col++)
                {
                    T dot = 0;
                    for (i32 i = 0; i < n_col; i++)
                    {
                        dot += (*this)(row, i) * m(i, col);
                    }
                    r(row, col) = dot;
                }
            }
            return r;
        }

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

        constexpr T operator()(i32 index) const
        {
            return (&m[0][0])[index];
        }

        constexpr T& operator()(i32 index)
        {
            return (&m[0][0])[index];
        }

        constexpr T operator()(i32 row, i32 col) const
        {
            return m[row][col];
        }

        constexpr T& operator()(i32 row, i32 col)
        {
            return m[row][col];
        }

        // sub-matrix
        // * the indices are inclusive. for example, if the start row and column
        //   indices are both 0, and the end row and column indices are both 2,
        //   this function will return the upper-left 3x3 portion.
        // * the value of end_row must not be smaller than the value of
        //   start_row, and the same goes for start_col and end_col.
        template<i32 start_row, i32 start_col, i32 end_row, i32 end_col>
        constexpr base_mat<T, end_row - start_row + 1, end_col - start_col + 1>
            sub() const
        {
            base_mat<T, end_row - start_row + 1, end_col - start_col + 1> r;
            for (i32 row = start_row; row <= end_row; row++)
            {
                for (i32 col = start_col; col <= end_col; col++)
                {
                    r(row - start_row, col - start_col) = (*this)(row, col);
                }
            }
        }

        // upper-left n x m sub-matrix
        // * n must be smaller than or equal to n_row.
        // * m must be smaller than or equal to n_col.
        template<i32 n, i32 m>
        constexpr base_mat<T, n, m> sub() const
        {
            base_mat<T, n, m> r;
            for (i32 row = 0; row < n; row++)
            {
                for (i32 col = 0; col < m; col++)
                {
                    r(row, col) = (*this)(row, col);
                }
            }
            return r;
        }

        static constexpr i32 n_rows()
        {
            return n_row;
        }

        static constexpr i32 n_cols()
        {
            return n_col;
        }

        static constexpr i32 n_elements()
        {
            return n_row * n_col;
        }

    private:
        T m[n_row][n_col];

    };

    template<typename T, i32 n_row, i32 n_col>
    constexpr base_mat<T, n_row, n_col> operator*(
        T s,
        const base_mat<T, n_row, n_col>& m
        )
    {
        return m * s;
    }

    template<typename T, i32 n>
    constexpr bool is_identity(const base_mat<T, n, n>& m)
    {
        for (i32 row = 0; row < n; row++)
        {
            for (i32 col = 0; col < n; col++)
            {
                T expected = (row == col) ? (T)1 : (T)0;
                if (m(row, col) != expected)
                    return false;
            }
        }
        return true;
    }

    // cofactor of m[p][q]
    // https://www.geeksforgeeks.org/adjoint-inverse-matrix/
    template<typename T, i32 n>
    constexpr base_mat<T, n - 1, n - 1> cofactor(
        const base_mat<T, n, n>& m,
        i32 p,
        i32 q
    )
    {
        i32 i = 0, j = 0;
        base_mat<T, n - 1, n - 1> r;
        for (i32 row = 0; row < n; row++)
        {
            for (i32 col = 0; col < n; col++)
            {
                // copy into the result matrix only those elements which are not
                // in the current row and column
                if (row != p && col != q)
                {
                    r(i, j++) = m(row, col);

                    // row is filled, so increase the row index and reset the
                    // col index
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

    // determinant of a square matrix
    // https://www.geeksforgeeks.org/determinant-of-a-matrix/
    template<std::floating_point T, i32 n>
    constexpr T determinant(base_mat<T, n, n> m)
    {
        if constexpr (n == 1)
            return m(0, 0);

        T det = 1;
        T total = 1;

        // temporary array for storing row
        T temp[n + 1]{};

        // traverse the diagonal elements
        for (i32 i = 0; i < n; i++)
        {
            i32 index = i;

            // find the index which has non zero value
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
                // swap the diagonal element row and index row
                for (i32 j = 0; j < n; j++)
                {
                    std::swap(m(index, j), m(i, j));
                }

                // the determinant sign changes when we shift rows
                det *= (index - i) % 2 == 0 ? (T)1 : (T)(-1);
            }

            // store the diagonal row elements
            for (i32 j = 0; j < n; j++)
            {
                temp[j] = m(i, j);
            }

            // traverse every row below the diagonal element
            for (i32 j = i + 1; j < n; j++)
            {
                // value of diagonal element
                T num1 = temp[i];

                // value of next row element
                T num2 = m(j, i);

                // traverse every column of row and multiply to every row
                for (i32 k = 0; k < n; k++)
                {
                    // multiply to make the diagonal element and next row
                    // element equal
                    m(j, k) = (num1 * m(j, k)) - (num2 * temp[k]);
                }

                // Det(kA)=kDet(A);
                total *= num1;
            }
        }

        // multiply the diagonal elements to get the determinant
        for (i32 i = 0; i < n; i++)
        {
            det *= m(i, i);
        }

        // Det(kA)/k=Det(A);
        return (det / total);
    }

    // adjoint of a square matrix
    // https://www.geeksforgeeks.org/adjoint-inverse-matrix/
    template<std::floating_point T, i32 n>
    constexpr base_mat<T, n, n> adjoint(const base_mat<T, n, n>& m)
    {
        if constexpr (n == 1)
            return base_mat<T, n, n>();

        base_mat<T, n, n> r;
        for (i32 i = 0; i < n; i++)
        {
            for (i32 j = 0; j < n; j++)
            {
                // get cofactor of m[i][j]
                base_mat<T, n - 1, n - 1> cf = cofactor(m, i, j);

                // sign of adj[j][i] positive if the sum of the row and column
                // indices is even.
                T sign = ((i + j) % 2 == 0) ? (T)1 : (T)(-1);

                // interchanging rows and columns to get the transpose of the
                // cofactor matrix
                r(j, i) = sign * determinant(cf);
            }
        }
        return r;
    }

    // inverted copy of a square matrix
    template<std::floating_point T, i32 n>
    constexpr base_mat<T, n, n> inverse(
        const base_mat<T, n, n>& m,
        bool* out_invertible = nullptr
    )
    {
        // determinant
        T det = determinant(m);
        if (det == 0)
        {
            if (out_invertible)
                *out_invertible = false;
            return base_mat<T, n, n>();
        }

        // adjoint
        base_mat<T, n, n> adj = adjoint(m);

        // inverse(A) = adj(A)/det(A)
        if (out_invertible)
            *out_invertible = true;
        return adj / det;
    }

    // transposed copy of a matrix
    template<typename T, i32 n_row, i32 n_col>
    constexpr base_mat<T, n_col, n_row> transpose(
        const base_mat<T, n_row, n_col>& m
    )
    {
        base_mat<T, n_col, n_row> r;
        for (i32 row = 0; row < n_row; row++)
        {
            for (i32 col = 0; col < n_col; col++)
            {
                r(col, row) = m(row, col);
            }
        }
        return r;
    }

    using mat1x2 = base_mat<f32, 1, 2>;
    using dmat1x2 = base_mat<f64, 1, 2>;
    using imat1x2 = base_mat<i32, 1, 2>;
    using umat1x2 = base_mat<u32, 1, 2>;

    using mat2x1 = base_mat<f32, 2, 1>;
    using dmat2x1 = base_mat<f64, 2, 1>;
    using imat2x1 = base_mat<i32, 2, 1>;
    using umat2x1 = base_mat<u32, 2, 1>;

    using mat1x3 = base_mat<f32, 1, 3>;
    using dmat1x3 = base_mat<f64, 1, 3>;
    using imat1x3 = base_mat<i32, 1, 3>;
    using umat1x3 = base_mat<u32, 1, 3>;

    using mat3x1 = base_mat<f32, 3, 1>;
    using dmat3x1 = base_mat<f64, 3, 1>;
    using imat3x1 = base_mat<i32, 3, 1>;
    using umat3x1 = base_mat<u32, 3, 1>;

    using mat1x4 = base_mat<f32, 1, 4>;
    using dmat1x4 = base_mat<f64, 1, 4>;
    using imat1x4 = base_mat<i32, 1, 4>;
    using umat1x4 = base_mat<u32, 1, 4>;

    using mat4x1 = base_mat<f32, 4, 1>;
    using dmat4x1 = base_mat<f64, 4, 1>;
    using imat4x1 = base_mat<i32, 4, 1>;
    using umat4x1 = base_mat<u32, 4, 1>;

    using mat2 = base_mat<f32, 2, 2>;
    using dmat2 = base_mat<f64, 2, 2>;
    using imat2 = base_mat<i32, 2, 2>;
    using umat2 = base_mat<u32, 2, 2>;

    using mat2x2 = base_mat<f32, 2, 2>;
    using dmat2x2 = base_mat<f64, 2, 2>;
    using imat2x2 = base_mat<i32, 2, 2>;
    using umat2x2 = base_mat<u32, 2, 2>;

    using mat2x3 = base_mat<f32, 2, 3>;
    using dmat2x3 = base_mat<f64, 2, 3>;
    using imat2x3 = base_mat<i32, 2, 3>;
    using umat2x3 = base_mat<u32, 2, 3>;

    using mat3x2 = base_mat<f32, 3, 2>;
    using dmat3x2 = base_mat<f64, 3, 2>;
    using imat3x2 = base_mat<i32, 3, 2>;
    using umat3x2 = base_mat<u32, 3, 2>;

    using mat2x4 = base_mat<f32, 2, 4>;
    using dmat2x4 = base_mat<f64, 2, 4>;
    using imat2x4 = base_mat<i32, 2, 4>;
    using umat2x4 = base_mat<u32, 2, 4>;

    using mat4x2 = base_mat<f32, 4, 2>;
    using dmat4x2 = base_mat<f64, 4, 2>;
    using imat4x2 = base_mat<i32, 4, 2>;
    using umat4x2 = base_mat<u32, 4, 2>;

    using mat3 = base_mat<f32, 3, 3>;
    using dmat3 = base_mat<f64, 3, 3>;
    using imat3 = base_mat<i32, 3, 3>;
    using umat3 = base_mat<u32, 3, 3>;

    using mat3x3 = base_mat<f32, 3, 3>;
    using dmat3x3 = base_mat<f64, 3, 3>;
    using imat3x3 = base_mat<i32, 3, 3>;
    using umat3x3 = base_mat<u32, 3, 3>;

    using mat3x4 = base_mat<f32, 3, 4>;
    using dmat3x4 = base_mat<f64, 3, 4>;
    using imat3x4 = base_mat<i32, 3, 4>;
    using umat3x4 = base_mat<u32, 3, 4>;

    using mat4x3 = base_mat<f32, 4, 3>;
    using dmat4x3 = base_mat<f64, 4, 3>;
    using imat4x3 = base_mat<i32, 4, 3>;
    using umat4x3 = base_mat<u32, 4, 3>;

    using mat4 = base_mat<f32, 4, 4>;
    using dmat4 = base_mat<f64, 4, 4>;
    using imat4 = base_mat<i32, 4, 4>;
    using umat4 = base_mat<u32, 4, 4>;

    using mat4x4 = base_mat<f32, 4, 4>;
    using dmat4x4 = base_mat<f64, 4, 4>;
    using imat4x4 = base_mat<i32, 4, 4>;
    using umat4x4 = base_mat<u32, 4, 4>;

}
