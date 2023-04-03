//
// Created by syf on 2023/3/10.
//

#ifndef CHILI_RENDERER_GL_MAT_H
#define CHILI_RENDERER_GL_MAT_H

#include <string>
#include <array>
#include <stdexcept>

#include "Vec3.h"
#include "Vec4.h"

template <typename T, unsigned int Size>
class Matrix
{
private:
	using MatElements = std::array<std::array<T, Size>, Size>;
public:
    Matrix() = default;
	explicit Matrix(const MatElements& elements)
	{
		int i = 0;
		for (auto& row : elements)
		{
			std::copy(row.begin(), row.end(), Elements[i].begin());
			i++;
		}
	}
	Matrix(const Matrix& other): Matrix(other.Elements)
	{
	}

	void swap(Matrix& rhs) noexcept
	{
		using std::swap;
		swap(Elements, rhs.Elements);
	}
	Matrix& operator=(Matrix rhs)
	{
		rhs.swap(*this);
		return *this;
	}
	Matrix& operator*=(T rhs)
	{
		for (auto& row : Elements)
		{
			for (auto& e : row)
			{
				e *= rhs;
			}
		}
		return *this;
	}
	Matrix operator*(T rhs) const
	{
		Matrix result = *this;
		return result *= rhs;
	}
	Matrix& operator*=(const Matrix& rhs)
	{
		return *this = *this * rhs;
	}
	Matrix operator*(const Matrix& rhs) const
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

		return Matrix(resultElements);
	}
	Matrix operator!()
	{
		Matrix xp;
		for (int i = 0; i < Size; i++)
		{
			for (int j = 0; j < Size; j++)
			{
				xp.Elements[i][j] = Elements[j][i];
			}
		}
		return xp;
	}
	static Matrix Identity()
	{
		MatElements identity;

		if constexpr(Size == 2)
		{
			identity = {
				1, 0,
				0, 1
			};
            return Matrix(identity);
        }
        if constexpr (Size == 3)
        {
            identity =
                {
                    1.0f, 0.0, 0.0,
                    0.0f, 1.0f, 0.0f,
                    0.0f, 0.0f, 1.0f
                };
            return Matrix(identity);
        }
        if constexpr (Size == 4)
        {
            identity = {
                1, 0, 0, 0,
                0, 1, 0, 0,
                0, 0, 1, 0,
                0, 0, 0, 1
            };
            return Matrix(identity);
        }

        throw std::runtime_error("bad matrix dimensionality");
	}
	static Matrix Scaling(T factor)
	{
		return Identity() * factor;
	}
	static Matrix RotationZ(T theta)
	{
		const T sinTheta = sin(theta);
		const T cosTheta = cos(theta);

		if constexpr (Size == 3)
		{
			MatElements raw =
			{
				cosTheta, sinTheta, 0,
				-sinTheta, cosTheta, 0,
				0, 0, 1
			};
			return Matrix(raw);
		}
		if constexpr (Size == 4)
		{
			MatElements  raw = {
				cosTheta, sinTheta, 0, 0,
               -sinTheta, sinTheta, 0, 0,
				0, 			0, 		1, 0,
				0, 			0, 		0, 1
			};
            return raw;
		}

        throw std::runtime_error("bad matrix dimensionality");
	}
    static Matrix RotationY(T theta)
    {
        const T sinTheta = sin(theta);
        const T cosTheta = cos(theta);

        if constexpr (Size == 3)
        {
            MatElements raw =
                {
                    cosTheta, 0, -sinTheta,
                    0,        1, 0,
                    sinTheta, 0, cosTheta
                };
            return Matrix(raw);
        }
        if constexpr (Size == 4)
        {
            MatElements  raw =
                {
                    cosTheta, 0, -sinTheta, 0,
                    0,        1, 0,         0,
                    sinTheta, 0, cosTheta,  0,
                    0,        0, 0,         0
                };
            return raw;
        }

        throw std::runtime_error("bad matrix dimensionality");
    }
    static Matrix RotationX(T theta)
    {
        const T sinTheta = sin(theta);
        const T cosTheta = cos(theta);

        if constexpr (Size == 3)
        {
            MatElements raw =
            {
                1, 0,           0,
                0, cosTheta, sinTheta,
                0, -sinTheta, cosTheta,
            };
            return Matrix(raw);
        }
        if constexpr (Size == 4)
        {
            MatElements raw =
            {
                1, 0,        0,         0,
                0, cosTheta, sinTheta,  0,
                0, -sinTheta, cosTheta, 0,
                0, 0,        0,         1
            };
            return Matrix(raw);
        }

        throw std::runtime_error("bad matrix dimensionality");
    }
public:
	MatElements Elements;
};

template<typename T>
Vector3<T>& operator *= (Vector3<T>& lhs, const Matrix<T, 3>& rhs)
{
	return lhs = lhs * rhs;
}

template<typename T>
Vector3<T> operator*(const Vector3<T>& lhs, const Matrix<T, 3>& rhs)
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
Vector3<T>& operator *= (Vector4<T>& lhs, const Matrix<T, 4>& rhs)
{
	return lhs = lhs * rhs;
}

template<typename T>
Vector3<T> operator*(const Vector4<T>& lhs, const Matrix<T, 4>& rhs)
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

using Mat2 = Matrix<float, 2>;
using Mad2 = Matrix<double, 2>;

using Mat3 = Matrix<float, 3>;
using Mad3 = Matrix<double, 3>;

using Mat4 = Matrix<float, 4>;
using Mad4 = Matrix<double, 4>;


#endif //CHILI_RENDERER_GL_MAT_H
