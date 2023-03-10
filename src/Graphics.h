//
// Created by syf on 2023/3/9.
//

#ifndef CHILI_RENDERER_GL_GRAPHICS_H
#define CHILI_RENDERER_GL_GRAPHICS_H

#include "Surface.h"

class Graphics {
private:
	struct VertexData
	{
		float x, y, z;
		float u, v;
	};
public:
	Graphics();
	Graphics(const Graphics&) = delete;
	Graphics& operator=(const Graphics&) = delete;
	void EndFrame();
	void BeginFrame();
	void PutPixel(int x, int y, int r, int g, int b)
	{
		PutPixel(x, y, {(unsigned char)r, (unsigned char) g, (unsigned char) b});
	}
	void PutPixel(int x, int y, Color color)
	{
		frameBuffer.PutPixel(x, y, color);
	}
	~Graphics();

private:
    Surface frameBuffer;
public:
    static constexpr unsigned int ScreenWidth = 640u;
    static constexpr unsigned int ScreenHeight = 480u;
};


#endif //CHILI_RENDERER_GL_GRAPHICS_H
