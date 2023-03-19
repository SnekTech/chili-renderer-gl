/******************************************************************************************
*	Chili DirectX Framework Version 16.10.01											  *
*	Vec2.h																				  *
*	Copyright 2016 PlanetChili <http://www.planetchili.net>								  *
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
#include "Vec2.h"

template <typename T>
class Vector3
{
public:
	Vector3() = default;
	Vector3( T x,T y,T z )
		:
		x(x), y(y), z( z )
	{}
	template <typename T2>
	explicit operator Vector3<T2>() const
	{
		return{ (T2)x,(T2)y,(T2)z };
	}
	operator Vector2<T>() const
	{
		return {x,y};
	}
	T		LenSq() const
	{
		return sq( *this );
	}
	T		Len() const
	{
		return sqrt( LenSq() );
	}
	Vector3&	Normalize()
	{
		const T length = Len();
		x /= length;
		y /= length;
		z /= length;
		return *this;
	}
	Vector3	GetNormalized() const
	{
		Vector3 norm = *this;
		norm.Normalize();
		return norm;
	}
	Vector3	operator-() const
	{
		return Vector3( -x,-y,-z );
	}
	Vector3&	operator=( const Vector3 &rhs )
	{
		this->x = rhs.x;
		this->y = rhs.y;
		z = rhs.z;
		return *this;
	}
	Vector3&	operator+=( const Vector3 &rhs )
	{
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;
		return *this;
	}
	Vector3&	operator-=( const Vector3 &rhs )
	{
		x -= rhs.x;
		y -= rhs.y;
		z -= rhs.z;
		return *this;
	}
	T		operator*( const Vector3 &rhs ) const
	{
		return x * rhs.x + y * rhs.y + z * rhs.z;
	}
    Vector3 Cross(const Vector3<T> &rhs) const
    {
        return Vector3(
            y * rhs.z - z * rhs.y,
            z * rhs.x - x * rhs.z,
            x * rhs.y - y * rhs.x );
    }
	Vector3	operator+( const Vector3 &rhs ) const
	{
		return Vector3( *this ) += rhs;
	}
	Vector3	operator-( const Vector3 &rhs ) const
	{
		return Vector3( *this ) -= rhs;
	}
	Vector3&	operator*=( const T &rhs )
	{
        x *= rhs;
        y *= rhs;
		z *= rhs;
		return *this;
	}
	Vector3	operator*( const T &rhs ) const
	{
		return Vector3( *this ) *= rhs;
	}
	Vector3	operator%( const Vector3& rhs ) const
	{
		return Vector3(
			x * rhs.z - z * rhs.y,
			z * rhs.x - x * rhs.z,
			x * rhs.y - y * rhs.x );
	}
	Vector3&	operator/=( const T &rhs )
	{
		this->x /= rhs;
		this->y /= rhs;
		z /= rhs;
		return *this;
	}
	Vector3	operator/( const T &rhs ) const
	{
		return Vector3( *this ) /= rhs;
	}
	bool	operator==( const Vector3 &rhs ) const
	{
		return x == rhs.x && y == rhs.y && rhs.z == z;
	}
	bool	operator!=( const Vector3 &rhs ) const
	{
        return !operator==(rhs);
	}
	// clamp to between 0.0 ~ 1.0
	Vector3&	Saturate()
	{
		x = std::min( 1.0f,std::max( 0.0f,x ) );
		y = std::min( 1.0f,std::max( 0.0f,y ) );
		z = std::min( 1.0f,std::max( 0.0f,z ) );
		return *this;
	}
	// clamp to between 0.0 ~ 1.0
	Vector3	GetSaturated() const
	{
		Vector3 temp( *this );
		temp.Saturate();
		return temp;
	}
	// x3 = x1 * x2 etc.
	Vector3&  Hadamard( const Vector3& rhs )
	{
		x *= rhs.x;
		y *= rhs.y;
		z *= rhs.z;
		return *this;
	}
	// x3 = x1 * x2 etc.
	Vector3	GetHadamard( const Vector3& rhs ) const
	{
		Vector3 temp( *this );
		temp.Hadamard( rhs );
		return temp;
	}
    Vector3 InterpolateTo(const Vector3& dest, float alpha) const
    {
        return *this + (dest - *this) * alpha;
    }
public:
    T x;
    T y;
	T z;
};

typedef Vector3<float> Vec3;
typedef Vector3<double> Ved3;
typedef Vector3<int> Vei3;