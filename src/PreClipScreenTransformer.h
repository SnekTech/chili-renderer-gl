//
// Created by syf on 2023/3/10.
//

#ifndef CHILI_RENDERER_GL_PRECLIPSCREENTRANSFORMER_H
#define CHILI_RENDERER_GL_PRECLIPSCREENTRANSFORMER_H

#include "DataStructures/Vec3.h"
#include "HardwareRender/Graphics.h"

class PreClipScreenTransformer
{
public:
	PreClipScreenTransformer()
		:
		xFactor(Graphics::ScreenWidth / 2.0f),
		yFactor(Graphics::ScreenHeight / 2.0f)
	{}
	Vec3& Transform(Vec3& v) const
	{
        float zInv =  1.0f / v.z;
		v.x = (v.x * zInv + 1.0f) * xFactor;
		v.y = (v.y * zInv + 1.0f) * yFactor;
		return v;
	}
	[[nodiscard]]
	Vec3 GetTransformed(const Vec3& v) const
	{
		Vec3 result(v);
		Transform(result);
		return result;
	}

private:
	float xFactor;
	float yFactor;
};

#endif //CHILI_RENDERER_GL_PRECLIPSCREENTRANSFORMER_H
