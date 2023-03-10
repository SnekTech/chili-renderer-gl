//
// Created by syf on 2023/3/9.
//

#include "Graphics.h"

Graphics::Graphics()
	:
	frameBuffer(ScreenWidth, ScreenHeight)
{
	// opengl init goes here
}

Graphics::~Graphics()
{
	// gl clean
}

void Graphics::EndFrame()
{

}

void Graphics::BeginFrame()
{
	frameBuffer.Clear(Colors::Magenta);
}