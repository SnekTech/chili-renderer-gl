//
// Created by syf on 2023/3/10.
//

#ifndef CHILI_RENDERER_GL_PRECLIPSCREENTRANSFORMER_H
#define CHILI_RENDERER_GL_PRECLIPSCREENTRANSFORMER_H

#include "DataStructures/Vec3.h"
#include "Render/Graphics.h"

class PreClipScreenTransformer
{
public:
	PreClipScreenTransformer()
		:
		xFactor(Graphics::ScreenWidth / 2.0f),
		yFactor(Graphics::ScreenHeight / 2.0f)
	{}

	template<class Vertex>
	Vertex& Transform(Vertex& v) const
	{
        float zInv =  1.0f / v.pos.z;
		v *= zInv;
		v.pos.x = (v.pos.x + 1.0f) * xFactor;
		v.pos.y = (v.pos.y + 1.0f) * yFactor;
		v.pos.z = zInv;

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

#endif //CHILI_RENDERER_GL_PRECLIPSCREENTRANSFORMER_H
