//
// Created by syf on 2023/3/10.
//
#pragma once

#include "DataStructures/Vec3.h"
#include "Render/Graphics.h"

class NDCScreenTransformer
{
public:
	NDCScreenTransformer()
		:
		xFactor(Graphics::ScreenWidth / 2.0f),
		yFactor(Graphics::ScreenHeight / 2.0f)
	{}

	template<class Vertex>
	Vertex& Transform(Vertex& v) const
	{
        // homo-space -> ndc on xyz
        // perspective divide on all attributes
        float wInv = 1.0f / v.pos.w;
        v *= wInv;

        // ndc -> screen 2d
		v.pos.x = (v.pos.x + 1.0f) * xFactor;
		v.pos.y = (v.pos.y + 1.0f) * yFactor;

        // for recovering the attributes after interpolation
		v.pos.w = wInv;

		return v;
	}

	template<class Vertex>
	Vertex GetTransformed(const Vertex& v) const
	{
		return Transform(Vertex(v));
	}

private:
	float xFactor;
	float yFactor;
};
