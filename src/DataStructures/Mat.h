//
// Created by syf on 2023/3/10.
//
#pragma once

#include <string>
#include <array>
#include <stdexcept>

#include "Vec3.h"
#include "Vec4.h"

template<typename T, unsigned int Size>
class MatrixImpl
{
private:
    using MatElements = std::array<std::array<T, Size>, Size>;
public:
    MatrixImpl() = default;

    explicit MatrixImpl(const MatElements& elements)
    {
        int i = 0;
        for (auto& row: elements)
        {
            std::copy(row.begin(), row.end(), Elements[i].begin());
            i++;
        }
    }

    MatrixImpl& operator*=(T rhs)
    {
        for (auto& row: Elements)
        {
            for (auto& e: row)
            {
                e *= rhs;
            }
        }
        return *this;
    }

    MatrixImpl operator*(T rhs) const
    {
        MatrixImpl result = *this;
        return result *= rhs;
    }

    MatrixImpl& operator*=(const MatrixImpl& rhs)
    {
        return *this = *this * rhs;
    }

    MatrixImpl operator*(const MatrixImpl& rhs) const
    {
        MatElements resultElements;

        for (int i = 0; i < Size; i++)
        {
            for (int j = 0; j < Size; j++)
            {
                T sum = 0;
                for (int k = 0; k < Size; k++)
                {
                    sum += Elements[i][k] * rhs.Elements[k][j];
                }
                resultElements[i][j] = sum;
            }
        }

        return MatrixImpl(resultElements);
    }

    MatrixImpl operator!()
    {
        MatrixImpl xp;
        for (int i = 0; i < Size; i++)
        {
            for (int j = 0; j < Size; j++)
            {
                xp.Elements[i][j] = Elements[j][i];
            }
        }
        return xp;
    }

    constexpr static MatrixImpl Identity()
    {
        MatElements identity;

        if constexpr (Size == 3)
        {
            identity =
                {
                    1.0f, 0.0f, 0.0f,
                    0.0f, 1.0f, 0.0f,
                    0.0f, 0.0f, 1.0f,
                };
            return MatrixImpl(identity);
        }
        if constexpr (Size == 4)
        {
            identity = {
                1, 0, 0, 0,
                0, 1, 0, 0,
                0, 0, 1, 0,
                0, 0, 0, 1,
            };
            return MatrixImpl(identity);
        }

        throw std::runtime_error("bad matrix dimensionality");
    }

    constexpr static MatrixImpl Scaling(T factor)
    {
        MatElements identity;

        if constexpr (Size == 3)
        {
            identity =
                {
                    factor, 0.0f, 0.0f,
                    0.0f, factor, 0.0f,
                    0.0f, 0.0f, factor,
                };
            return MatrixImpl(identity);
        }
        if constexpr (Size == 4)
        {
            identity = {
                factor, 0.0f, 0.0f, 0.0f,
                0.0f, factor, 0.0f, 0.0f,
                0.0f, 0.0f, factor, 0.0f,
                0.0f, 0.0f, 0.0f, 1.0f,
            };
            return MatrixImpl(identity);
        }

        throw std::runtime_error("bad matrix dimensionality");
    }

    static MatrixImpl RotationZ(T theta)
    {
        const T sinTheta = sin(theta);
        const T cosTheta = cos(theta);

        if constexpr (Size == 3)
        {
            MatElements raw =
                {
                    cosTheta, sinTheta, 0.0f,
                    -sinTheta, cosTheta, 0.0f,
                    0.0f, 0.0f, 1.0f,
                };
            return MatrixImpl(raw);
        }
        if constexpr (Size == 4)
        {
            MatElements raw = {
                cosTheta, sinTheta, 0.0f, 0.0f,
                -sinTheta, cosTheta, 0.0f, 0.0f,
                0.0f, 0.0f, 1.0f, 0,
                0.0f, 0.0f, 0.0f, 1.0f,
            };
            return MatrixImpl(raw);
        }

        throw std::runtime_error("bad matrix dimensionality");
    }

    static MatrixImpl RotationY(T theta)
    {
        const T sinTheta = sin(theta);
        const T cosTheta = cos(theta);

        if constexpr (Size == 3)
        {
            MatElements raw =
                {
                    cosTheta, 0.0f, -sinTheta,
                    0.0f, 1.0f, 0.0f,
                    sinTheta, 0.0f, cosTheta
                };
            return MatrixImpl(raw);
        }
        if constexpr (Size == 4)
        {
            MatElements raw =
                {
                    cosTheta, 0.0f, -sinTheta, 0.0f,
                    0.0f, 1.0f, 0.0f, 0.0f,
                    sinTheta, 0.0f, cosTheta, 0.0f,
                    0.0f, 0.0f, 0.0f, 1.0f,
                };
            return MatrixImpl(raw);
        }

        throw std::runtime_error("bad matrix dimensionality");
    }

    static MatrixImpl RotationX(T theta)
    {
        const T sinTheta = sin(theta);
        const T cosTheta = cos(theta);

        if constexpr (Size == 3)
        {
            MatElements raw =
                {
                    1.0f, 0.0f, 0.0f,
                    0.0f, cosTheta, sinTheta,
                    0.0f, -sinTheta, cosTheta,
                };
            return MatrixImpl(raw);
        }
        if constexpr (Size == 4)
        {
            MatElements raw =
                {
                    1.0f, 0.0f, 0.0f, 0.0f,
                    0.0f, cosTheta, sinTheta, 0.0f,
                    0.0f, -sinTheta, cosTheta, 0.0f,
                    0.0f, 0.0f, 0.0f, 1.0f,
                };
            return MatrixImpl(raw);
        }

        throw std::runtime_error("bad matrix dimensionality");
    }

    template<class V>
    constexpr static MatrixImpl Translation(const V& translation)
    {
        return Translation(translation.x, translation.y, translation.z);
    }

    constexpr static MatrixImpl Translation(T x, T y, T z)
    {
        if constexpr (Size == 4)
        {
            MatElements raw = {
                1.0f, 0.0f, 0.0f, 0.0f,
                0.0f, 1.0f, 0.0f, 0.0f,
                0.0f, 0.0f, 1.0f, 0.0f,
                x, y, z, 1.0f,
            };
            return MatrixImpl(raw);
        }

        throw std::runtime_error("bad matrix dimensionality");
    }

    constexpr static MatrixImpl Projection(T w, T h, T n, T f)
    {
        if constexpr (Size == 4)
        {
            MatElements raw = {
                2.0f * n / w, 0.0f, 0.0f, 0.0f,
                0.0f, 2.0f * n / h, 0.0f, 0.0f,
                0.0f, 0.0f, f / (f - n), 1.0f,
                0.0f, 0.0f, -n * f / (f - n), 0.0f,
            };
            return MatrixImpl(raw);
        }

        throw std::runtime_error("bad matrix dimensionality");
    }

    constexpr static MatrixImpl ProjectionHFOV(T fov, T aspectRatio, T n, T f)
    {
        if constexpr (Size == 4)
        {
            const auto fov_rad = fov * PI / 180.0f;
            const auto w = 1.0f / std::tan(fov_rad / 2.0f);
            const auto h = w * aspectRatio;

            MatElements raw = {
                w, 0.0f, 0.0f, 0.0f,
                0.0f, h, 0.0f, 0.0f,
                0.0f, 0.0f, f / (f - n), 1.0f,
                0.0f, 0.0f, -n * f / (f - n), 0.0f,
            };
            return MatrixImpl(raw);
        }

        throw std::runtime_error("bad matrix dimensionality");
    }

public:
    MatElements Elements;
};

template<typename T>
Vector3<T>& operator*=(Vector3<T>& lhs, const MatrixImpl<T, 3>& rhs)
{
    return lhs = lhs * rhs;
}

template<typename T>
Vector3<T> operator*(const Vector3<T>& lhs, const MatrixImpl<T, 3>& rhs)
{
    const auto& matrix = rhs.Elements;
    return
        {
            lhs.x * matrix[0][0] + lhs.y * matrix[1][0] + lhs.z * matrix[2][0],
            lhs.x * matrix[0][1] + lhs.y * matrix[1][1] + lhs.z * matrix[2][1],
            lhs.x * matrix[0][2] + lhs.y * matrix[1][2] + lhs.z * matrix[2][2],
        };
}

template<typename T>
Vector4<T>& operator*=(Vector4<T>& lhs, const MatrixImpl<T, 4>& rhs)
{
    return lhs = lhs * rhs;
}

template<typename T>
Vector4<T> operator*(const Vector4<T>& lhs, const MatrixImpl<T, 4>& rhs)
{
    const auto& matrix = rhs.Elements;
    return
        {
            lhs.x * matrix[0][0] + lhs.y * matrix[1][0] + lhs.z * matrix[2][0] + lhs.w * matrix[3][0],
            lhs.x * matrix[0][1] + lhs.y * matrix[1][1] + lhs.z * matrix[2][1] + lhs.w * matrix[3][1],
            lhs.x * matrix[0][2] + lhs.y * matrix[1][2] + lhs.z * matrix[2][2] + lhs.w * matrix[3][2],
            lhs.x * matrix[0][3] + lhs.y * matrix[1][3] + lhs.z * matrix[2][3] + lhs.w * matrix[3][3],
        };
}

using Mat2 = MatrixImpl<float, 2>;
using Mad2 = MatrixImpl<double, 2>;

using Mat3 = MatrixImpl<float, 3>;
using Mad3 = MatrixImpl<double, 3>;

using Mat4 = MatrixImpl<float, 4>;
using Mad4 = MatrixImpl<double, 4>;
