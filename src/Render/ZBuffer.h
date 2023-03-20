//
// Created by syf on 2023/3/20.
//

#ifndef CHILI_RENDERER_GL_ZBUFFER_H
#define CHILI_RENDERER_GL_ZBUFFER_H

#include <limits>
#include <memory>

class ZBuffer
{
public:
	ZBuffer(int width, int height)
		: width(width), height(height), pBuffer(std::make_unique<float[]>(width * height))
	{
	}

	ZBuffer(const ZBuffer&) = delete;

	ZBuffer& operator=(const ZBuffer& rhs) = delete;

	void Clear()
	{
		const int length = width * height;
		for (int i = 0; i < length; i++)
		{
			pBuffer[i] = std::numeric_limits<float>::infinity();
		}
	}

	float& At(int x, int y)
	{
		return pBuffer[y * width + x];
	}

	[[nodiscard]] const float& At(int x, int y) const
	{
		return const_cast<ZBuffer*>(this)->At(x, y);
	}

	bool TestAndSet(int x, int y, float depth)
	{
		float& depthInBuffer = At(x, y);
		if (depth < depthInBuffer)
		{
			depthInBuffer = depth;
			return true;
		}

		return false;
	}

private:
	int width;
	int height;
	std::unique_ptr<float[]> pBuffer;
};

#endif //CHILI_RENDERER_GL_ZBUFFER_H
