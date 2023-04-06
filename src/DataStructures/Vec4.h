/******************************************************************************************
*	Chili DirectX Framework Version 16.10.01											  *
*	Vec2.h																				  *
*	Copyright 2018 PlanetChili <http://www.planetchili.net>								  *
*																						  *
*	This file is part of The Chili DirectX Framework.									  *
*																						  *
*	The Chili DirectX Framework is free software: you can redistribute it and/or modify	  *
*	it under the terms of the GNU General Public License as published by				  *
*	the Free Software Foundation, either version 3 of the License, or					  *
*	(at your option) any later version.													  *
*																						  *
*	The Chili DirectX Framework is distributed in the hope that it will be useful,		  *
*	but WITHOUT ANY WARRANTY; without even the implied warranty of						  *
*	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the						  *
*	GNU General Public License for more details.										  *
*																						  *
*	You should have received a copy of the GNU General Public License					  *
*	along with The Chili DirectX Framework.  If not, see <http://www.gnu.org/licenses/>.  *
******************************************************************************************/
#pragma once

#include <algorithm>
#include "../Widgets/ChiliMath.h"
#include "Vec3.h"

template<typename T>
class Vector4
{
public:
    Vector4() = default;

    Vector4(T x, T y, T z, T w)
        :
        x(x), y(y), z(z), w(w)
    {
    }

    explicit Vector4(const Vector3<T>& v3, float w = 1.0f)
        :
        x(v3.x), y(v3.y), z(v3.z),
        w(w)
    {
    }

    template<typename T2>
    explicit operator Vector4<T2>() const
    {
        return { (T2)x, (T2)y, (T2)z, (T2)w };
    }

    //T		LenSq() const
    //{
    //	return sq( *this );
    //}
    //T		Len() const
    //{
    //	return sqrt( LenSq() );
    //}
    //_Vec3&	Normalize()
    //{
    //	const T length = Len();
    //	x /= length;
    //	y /= length;
    //	z /= length;
    //	return *this;
    //}
    //_Vec3	GetNormalized() const
    //{
    //	_Vec3 norm = *this;
    //	norm.Normalize();
    //	return norm;
    //}
    Vector4 operator-() const
    {
        return Vector4(-x, -y, -z, -w);
    }

    Vector4& operator=(const Vector4& rhs)
    {
        x = rhs.x;
        y = rhs.y;
        z = rhs.z;
        w = rhs.w;
        return *this;
    }

    Vector4& operator+=(const Vector4& rhs)
    {
        x += rhs.x;
        y += rhs.y;
        z += rhs.z;
        w += rhs.w;
        return *this;
    }

    Vector4& operator-=(const Vector4& rhs)
    {
        x -= rhs.x;
        y -= rhs.y;
        z -= rhs.z;
        w -= rhs.w;
        return *this;
    }

    //T		operator*( const _Vec4 &rhs ) const
    //{
    //	return x * rhs.x + y * rhs.y + z * rhs.z;
    //}
    Vector4 operator+(const Vector4& rhs) const
    {
        return Vector4(*this) += rhs;
    }

    Vector4 operator-(const Vector4& rhs) const
    {
        return Vector4(*this) -= rhs;
    }

    Vector4& operator*=(const T& rhs)
    {
        x *= rhs;
        y *= rhs;
        z *= rhs;
        w *= rhs;
        return *this;
    }

    Vector4 operator*(const T& rhs) const
    {
        return Vector4(*this) *= rhs;
    }

    //_Vec4	operator%( const _Vec4& rhs ) const
    //{
    //	return _Vec4(
    //		y * rhs.z - z * rhs.y,
    //		z * rhs.x - x * rhs.z,
    //		x * rhs.y - y * rhs.x );
    //}
    Vector4& operator/=(const T& rhs)
    {
        x /= rhs;
        y /= rhs;
        z /= rhs;
        w /= rhs;
        return *this;
    }

    Vector4 operator/(const T& rhs) const
    {
        return Vector4(*this) /= rhs;
    }

    bool operator==(const Vector4& rhs) const
    {
        return x == rhs.x && y == rhs.y && rhs.z == z && rhs.w == w;
    }

    bool operator!=(const Vector4& rhs) const
    {
        return !(*this == rhs);
    }

    // clamp to between 0.0 ~ 1.0
    Vector4& Saturate()
    {
        x = std::min(1.0f, std::max(0.0f, x));
        y = std::min(1.0f, std::max(0.0f, y));
        z = std::min(1.0f, std::max(0.0f, z));
        w = std::min(1.0f, std::max(0.0f, w));
        return *this;
    }

    // clamp to between 0.0 ~ 1.0
    Vector4 GetSaturated() const
    {
        Vector4 temp(*this);
        temp.Saturate();
        return temp;
    }

    // x3 = x1 * x2 etc.
    Vector4& Hadamard(const Vector4& rhs)
    {
        x *= rhs.x;
        y *= rhs.y;
        z *= rhs.z;
        w *= rhs.w;
        return *this;
    }

    // x3 = x1 * x2 etc.
    Vector4 GetHadamard(const Vector4& rhs) const
    {
        Vector4 temp(*this);
        temp.Hadamard(rhs);
        return temp;
    }

public:
    T x;
    T y;
    T z;
    T w;
};

typedef Vector4<float> Vec4;
typedef Vector4<double> Ved4;
typedef Vector4<int> Vei4;