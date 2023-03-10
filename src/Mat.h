//
// Created by syf on 2023/3/10.
//

#ifndef CHILI_RENDERER_GL_MAT_H
#define CHILI_RENDERER_GL_MAT_H

#include <string>
#include <array>
#include <stdexcept>

#include "Vec3.h"

template <typename T, unsigned int Size>
class Matrix
{
private:
	using MatElements = std::array<std::array<T, Size>, Size>;
public:
	Matrix(const MatElements& elements)
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

		switch (Size)
		{
		case 2:
			identity = {
				1, 0,
				0, 1
			};
			break;
		case 3:
			identity =
			{
				1, 0, 0,
				0, 1, 0,
				0, 0, 1
			};
			break;
		case 4:
			identity = {
				1, 0, 0, 0,
				0, 1, 0, 0,
				0, 0, 1, 0,
				0, 0, 0, 1
			};
			break;
		default:
			throw std::runtime_error("bad matrix dimensionality");
		}


		return Matrix(identity);
	}
	static Matrix Scaling(T factor)
	{
		return Identity() * factor;
	}
public:
	MatElements Elements;
};

template<typename T, unsigned int Size>
_Vec3<T>& operator *= (_Vec3<T>& lhs, const Matrix<T, Size>& rhs)
{
	return lhs = lhs * rhs;
}

template<typename T, unsigned int Size>
_Vec3<T> operator*(const _Vec3<T>& lhs, const Matrix<T, Size>& rhs)
{
	const auto& matrix = rhs.Elements;
	return
	{
		lhs.x * matrix[0][0] + lhs.y * matrix[1][0] + lhs.z * matrix[2][0],
		lhs.x * matrix[0][1] + lhs.y * matrix[1][1] + lhs.z * matrix[2][1],
		lhs.x * matrix[0][2] + lhs.y * matrix[1][2] + lhs.z * matrix[2][2],
	};
}

using Mat3 = Matrix<float, 3>;
using Mad3 = Matrix<double, 3>;


#endif //CHILI_RENDERER_GL_MAT_H
