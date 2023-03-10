//
// Created by syf on 2023/3/9.
//

#include <glad/gl.h>
#include <iostream>

#include "Graphics.h"
#include "Shader.h"

Graphics::Graphics()
	:
	VBO(0), VAO(0), EBO(0),
	frameBuffer(ScreenWidth, ScreenHeight)
{
	// glad: load all OpenGL function pointers
	// ---------------------------------------
	int version = gladLoadGL(glfwGetProcAddress);
	if (!version)
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		throw std::exception();
	}
	printf("GL %d.%d\n", GLAD_VERSION_MAJOR(version), GLAD_VERSION_MINOR(version));

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	struct VertexData vertices[] = {
		// positions             // texture coords
		{1.0f,  1.0f, 0.0f, 1.0f, 1.0f}, // top right
		{1.0f, -1.0f, 0.0f, 1.0f, 0.0f}, // bottom right
		{-1.0f, -1.0f, 0.0f, 0.0f, 0.0f}, // bottom left
		{-1.0f,  1.0f, 0.0f, 0.0f, 1.0f}  // top left
	};
	unsigned int indices[] = {
		0, 1, 3,
		1, 2, 3
	};
	GLsizei stride = sizeof(VertexData);

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*) 0); // NOLINT(modernize-use-nullptr)
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*) (3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// create texture
	unsigned int texture1;
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);
	// allocation
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, ScreenWidth, ScreenHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
	// Set up the texture, so it won't be black
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	Shader ourShader(R"(.\..\src\shaders\vs.glsl)", R"(.\..\src\shaders\fs.glsl)");
	ourShader.Use();
}

Graphics::~Graphics()
{
	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
}

void Graphics::EndFrame()
{
	// set pixels
	auto pixels = (GLvoid*)frameBuffer.GetBufferPtrConst();
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, ScreenWidth, ScreenHeight, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (const void*)0); // NOLINT(modernize-use-nullptr)
}

void Graphics::BeginFrame()
{
	frameBuffer.Clear(Colors::Magenta);
}

void Graphics::DrawLine( float x1,float y1,float x2,float y2,Color c )
{
	const float dx = x2 - x1;
	const float dy = y2 - y1;

	if( dy == 0.0f && dx == 0.0f )
	{
		PutPixel( int( x1 ),int( y1 ),c );
	}
	else if( abs( dy ) > abs( dx ) )
	{
		if( dy < 0.0f )
		{
			std::swap( x1,x2 );
			std::swap( y1,y2 );
		}

		const float m = dx / dy;
		float y = y1;
		int lastIntY;
		for( float x = x1; y < y2; y += 1.0f, x += m )
		{
			lastIntY = int( y );
			PutPixel( int( x ),lastIntY,c );
		}
		if( int( y2 ) > lastIntY )
		{
			PutPixel( int( x2 ),int( y2 ),c );
		}
	}
	else
	{
		if( dx < 0.0f )
		{
			std::swap( x1,x2 );
			std::swap( y1,y2 );
		}

		const float m = dy / dx;
		float x = x1;
		int lastIntX;
		for( float y = y1; x < x2; x += 1.0f,y += m )
		{
			lastIntX = int( x );
			PutPixel( lastIntX,int( y ),c );
		}
		if( int( x2 ) > lastIntX )
		{
			PutPixel( int( x2 ),int( y2 ),c );
		}
	}
}