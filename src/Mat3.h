//
// Created by syf on 2023/3/10.
//

#ifndef CHILI_RENDERER_GL_MAT3_H
#define CHILI_RENDERER_GL_MAT3_H

#include "Vec3.h"
#include <string>
#include <memory>

template <typename T>
class Matrix3
{
private:
	using MatElements = T[3][3];
public:
	Matrix3(const MatElements& elements) : pElements(std::make_unique<MatElements>())
	{
		memcpy(pElements.get(), elements, sizeof(MatElements));
	}
	Matrix3(const Matrix3& other): pElements(other.pElements ? std::make_unique<MatElements>() : nullptr)
	{
		memcpy(pElements.get(), other.pElements.get(), sizeof(MatElements));
//		for (int i = 0; i < 3; i++)
//		{
//			for (int j = 0; j < 3; j++)
//			{
//				(*pElements)[i][j] = (*other.pElements)[i][j];
//			}
//		}
	}

	void swap(Matrix3& rhs) noexcept
	{
		using std::swap;
		swap(pElements, rhs.pElements);
	}
	Matrix3& operator=(Matrix3 rhs)
	{
		rhs.swap(*this);
		return *this;
	}
	Matrix3& operator*=(T rhs)
	{
		for (auto& row : *pElements)
		{
			for (T& e : row)
			{
				e *= rhs;
			}
		}
		return *this;
	}
	Matrix3 operator*(T rhs) const
	{
		Matrix3 result = *this;
		return result *= rhs;
	}
	Matrix3 operator*(const Matrix3& rhs) const
	{
		MatElements resultElements;
		auto& otherElements = *rhs.pElements;

		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				T sum = 0;
				for (int k = 0; k < 3; k++)
				{
					sum += *pElements[i][k] * otherElements[k][j];
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
	std::unique_ptr<MatElements> pElements;
};

template<typename T>
_Vec3<T>& operator *= (_Vec3<T>& lhs, const Matrix3<T>& rhs)
{
	return lhs = lhs * rhs;
}

template<typename T>
_Vec3<T> operator*(const _Vec3<T>& lhs, const Matrix3<T>& rhs)
{
	const auto& matrixElements = *rhs.pElements;
	return
	{
		lhs.x * matrixElements[0][0] + lhs.y * matrixElements[1][0] + lhs.z * matrixElements[2][0],
		lhs.x * matrixElements[0][1] + lhs.y * matrixElements[1][1] + lhs.z * matrixElements[2][1],
		lhs.x * matrixElements[0][2] + lhs.y * matrixElements[1][2] + lhs.z * matrixElements[2][2],
	};
}

using Mat3 = Matrix3<float>;
using Mad3 = Matrix3<double>;


#endif //CHILI_RENDERER_GL_MAT3_H
