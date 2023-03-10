//
// Created by syf on 2023/3/10.
//

#ifndef CHILI_RENDERER_GL_MAT3_H
#define CHILI_RENDERER_GL_MAT3_H

#include "Vec3.h"
#include <string>
#include <array>

template <typename T>
class Matrix3
{
private:
	using MatElements = std::array<std::array<T, 3>, 3>;
public:
	Matrix3(const MatElements& elements)
	{
		int i = 0;
		for (auto& row : elements)
		{
			std::copy(row.begin(), row.end(), matRows[i].begin());
			i++;
		}
	}
	Matrix3(const Matrix3& other): Matrix3(other.matRows)
	{
	}

	void swap(Matrix3& rhs) noexcept
	{
		using std::swap;
		swap(matRows, rhs.matRows);
	}
	Matrix3& operator=(Matrix3 rhs)
	{
		rhs.swap(*this);
		return *this;
	}
	Matrix3& operator*=(T rhs)
	{
		for (auto& row : matRows)
		{
			for (auto& e : row)
			{
				e *= rhs;
			}
		}
		return *this;
	}
	Matrix3 operator*(T rhs) const
	{
		Matrix3 result = *this;
		result *= rhs;
		return result;
	}
	Matrix3 operator*(const Matrix3& rhs) const
	{
		MatElements resultElements;

		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				T sum = 0;
				for (int k = 0; k < 3; k++)
				{
					sum += matRows[i][k] * rhs.matRows[k][j];
				}
				resultElements[i][j] = sum;
			}
		}

		return Matrix3(resultElements);
	}
	static Matrix3 Identity()
	{
		MatElements identity =
			{
				1, 0, 0,
				0, 1, 0,
				0, 0, 1
			};

		return Matrix3(identity);
	}
	static Matrix3 Scaling(T factor)
	{
		return Identity() * factor;
	}
public:
	MatElements matRows;
};

template<typename T>
_Vec3<T>& operator *= (_Vec3<T>& lhs, const Matrix3<T>& rhs)
{
	return lhs = lhs * rhs;
}

template<typename T>
_Vec3<T> operator*(const _Vec3<T>& lhs, const Matrix3<T>& rhs)
{
	const auto& matrix = rhs.matRows;
	return
	{
		lhs.x * matrix[0][0] + lhs.y * matrix[1][0] + lhs.z * matrix[2][0],
		lhs.x * matrix[0][1] + lhs.y * matrix[1][1] + lhs.z * matrix[2][1],
		lhs.x * matrix[0][2] + lhs.y * matrix[1][2] + lhs.z * matrix[2][2],
	};
}

using Mat3 = Matrix3<float>;
using Mad3 = Matrix3<double>;


#endif //CHILI_RENDERER_GL_MAT3_H
