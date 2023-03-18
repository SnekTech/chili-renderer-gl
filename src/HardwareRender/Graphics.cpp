//
// Created by syf on 2023/3/9.
//

#include "glad/gl.h"
#include <iostream>

#include "Graphics.h"
#include "../Widgets/Shader.h"

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
        { 1.0f,  1.0f,  0.0f, 1.0f, 1.0f }, // top right
        { 1.0f,  -1.0f, 0.0f, 1.0f, 0.0f }, // bottom right
        { -1.0f, -1.0f, 0.0f, 0.0f, 0.0f }, // bottom left
        { -1.0f, 1.0f,  0.0f, 0.0f, 1.0f }  // top left
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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0); // NOLINT(modernize-use-nullptr)
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
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
    frameBuffer.Clear(Colors::Black);
}

void Graphics::DrawLine(float x1, float y1, float x2, float y2, Color c)
{
    const float dx = x2 - x1;
    const float dy = y2 - y1;

    if (dy == 0.0f && dx == 0.0f)
    {
        PutPixel(int(x1), int(y1), c);
        return;
    }

    if (abs(dy) > abs(dx))
    {
        if (dy < 0.0f)
        {
            std::swap(x1, x2);
            std::swap(y1, y2);
        }

        const float m = dx / dy;
        float y = y1;
        int lastIntY;
        for (float x = x1; y < y2; y += 1.0f, x += m)
        {
            lastIntY = int(y);
            PutPixel(int(x), lastIntY, c);
        }
        if (int(y2) > lastIntY)
        {
            PutPixel(int(x2), int(y2), c);
        }
    }
    else
    {
        if (dx < 0.0f)
        {
            std::swap(x1, x2);
            std::swap(y1, y2);
        }

        const float m = dy / dx;
        float x = x1;
        int lastIntX;
        for (float y = y1; x < x2; x += 1.0f, y += m)
        {
            lastIntX = int(x);
            PutPixel(lastIntX, int(y), c);
        }
        if (int(x2) > lastIntX)
        {
            PutPixel(int(x2), int(y2), c);
        }
    }
}

void Graphics::DrawTriangle(Vec2 v0, Vec2 v1, Vec2 v2, Color color)
{
    // we calculate & split the triangle vertically,
    // sort vertices vertically, let v0.y <= v1.y <= v2.y, aka bottom to top
    using std::swap;
    if (v1.y < v0.y) swap(v0, v1);
    if (v2.y < v1.y) swap(v1, v2);
    if (v1.y < v0.y) swap(v0, v1);

    if (v0.y == v1.y) // natural flat bottom
    {
        if (v1.x < v0.x) swap(v0, v1);
        DrawFlatBottomTriangle(v0, v1, v2, color);
        return;
    }

    if (v1.y == v2.y) // natural flat top
    {
        if (v2.x < v1.x) swap(v1, v2);
        DrawFlatTopTriangle(v0, v1, v2, color);
        return;
    }

    // now it's a general triangle,
    // we need to split it into flat triangles

    const float alpha = (v1.y - v0.y) / (v2.y - v0.y);
    const Vec2 vi = v0 + (v2 - v0) * alpha;

    if (v1.x < vi.x) // major right
    {
        DrawFlatTopTriangle(v0, v1, vi, color);
        DrawFlatBottomTriangle(v1, vi, v2, color);
    }
    else // major left
    {
        DrawFlatTopTriangle(v0, vi, v1, color);
        DrawFlatBottomTriangle(vi, v1, v2, color);
    }

}

void Graphics::DrawTriangleTex(const TexVertex& v0, const TexVertex& v1, const TexVertex& v2, const Surface& tex)
{
    auto pv0 = &v0;
    auto pv1 = &v1;
    auto pv2 = &v2;
    using std::swap;

    if (pv1->pos.y < pv0->pos.y) swap(pv0, pv1);
    if (pv2->pos.y < pv1->pos.y) swap(pv1, pv2);
    if (pv1->pos.y < pv0->pos.y) swap(pv0, pv1);

    if (pv0->pos.y == pv1->pos.y) // natural flat bottom
    {
        if (pv1->pos.x < pv0->pos.x) swap(pv0, pv1);
        DrawFlatBottomTriangleTex(*pv0, *pv1, *pv2, tex);
        return;
    }

    if (pv1->pos.y == pv2->pos.y) // natural flag top
    {
        if (pv2->pos.x < pv1->pos.x) swap(pv1, pv2);
        DrawFlatTopTriangleTex(*pv0, *pv1, *pv2, tex);
        return;
    }

    const float alphaSplit = ((pv1->pos.y - pv0->pos.y) / (pv2->pos.y - pv0->pos.y));
    TexVertex vi = pv0->InterpolateTo(*pv2, alphaSplit);

    if (pv1->pos.x < vi.pos.x) // major right
    {
        DrawFlatTopTriangleTex(*pv0, *pv1, vi, tex);
        DrawFlatBottomTriangleTex(*pv1, vi, *pv2, tex);
    }
    else // major left
    {
        DrawFlatTopTriangleTex(*pv0, vi, *pv1, tex);
        DrawFlatBottomTriangleTex(vi, *pv1, *pv2, tex);
    }

}

void Graphics::DrawFlatTopTriangle(Vec2 v0, Vec2 v1, Vec2 v2, Color color)
{
    // calculate slopes
    const float m0 = (v1.x - v0.x) / (v1.y - v0.y);
    const float m1 = (v2.x - v0.x) / (v2.y - v0.y);

    // calculate start & end line
    // applying the 'bottom' rule
    const int yStart = (int)ceilf(v0.y - 0.5f);
    const int yEnd = (int)ceilf(v2.y - 0.5f);

    for (int y = yStart; y < yEnd; y++)
    {
        // calculate scanline range in float
        const float pixelY = (float)y + 0.5f;
        const float px0 = m0 * (pixelY - v0.y) + v0.x;
        const float px1 = m1 * (pixelY - v0.y) + v0.x;

        // calculate scanline start & end pixel
        const int xStart = (int)ceilf(px0 - 0.5f);
        const int xEnd = (int)ceilf(px1 - 0.5f);

        for (int x = xStart; x < xEnd; x++)
        {
            PutPixel(x, y, color);
        }
    }
}

void Graphics::DrawFlatBottomTriangle(Vec2 v0, Vec2 v1, Vec2 v2, Color color)
{
    const float m0 = (v2.x - v0.x) / (v2.y - v0.y);
    const float m1 = (v2.x - v1.x) / (v2.y - v1.y);

    const int yStart = (int)ceilf(v0.y - 0.5f);
    const int yEnd = (int)ceilf(v2.y - 0.5f);

    for (int y = yStart; y < yEnd; y++)
    {
        const float pixelY = (float)y + 0.5f;
        const float x0 = m0 * (pixelY - v0.y) + v0.x;
        const float x1 = m1 * (pixelY - v1.y) + v1.x;

        const int xStart = (int)ceilf(x0 - 0.5f);
        const int xEnd = (int)ceilf(x1 - 0.5f);

        for (int x = xStart; x < xEnd; x++)
        {
            PutPixel(x, y, color);
        }
    }
}

void Graphics::DrawFlatTopTriangleTex(const TexVertex& v0, const TexVertex& v1, const TexVertex& v2, const Surface& tex)
{
    // calculate dVertex / dy
    const float deltaY = v2.pos.y - v0.pos.y;
    const TexVertex dv0 = (v1 - v0) / deltaY;
    const TexVertex dv1 = (v2 - v0) / deltaY;

    // create right edge interpolant
    TexVertex itEdge1 = v0;

    DrawFlagTriangleTex(v0, v1, v2, tex, dv0, dv1, itEdge1);
}

void
Graphics::DrawFlatBottomTriangleTex(const TexVertex& v0, const TexVertex& v1, const TexVertex& v2, const Surface& tex)
{
    // calculate dVertex / dy
    const float deltaY = v2.pos.y - v0.pos.y;
    const TexVertex dv0 = (v2 - v0) / deltaY;
    const TexVertex dv1 = (v2 - v1) / deltaY;

    // create right edge interpolant
    TexVertex itEdge1 = v1;

    DrawFlagTriangleTex(v0, v1, v2, tex, dv0, dv1, itEdge1);
}

void Graphics::DrawFlagTriangleTex(const TexVertex& v0, const TexVertex& v1, const TexVertex& v2, const Surface& tex,
    const TexVertex& dv0, const TexVertex& dv1, TexVertex& itEdge1)
{
    // create edge interpolant for left edge (always v0)
    TexVertex itEdge0 = v0;

    // calculate start and end scanlines
    const int yStart = (int)ceilf(v0.pos.y - 0.5f);
    const int yEnd = (int)ceilf(v2.pos.y - 0.5f);

    // do interpolant preStep
    itEdge0 += dv0 * (float(yStart) + 0.5f - v0.pos.y);
    itEdge1 += dv1 * (float(yStart) + 0.5f - v0.pos.y);

    // init tex width/height and clamp values
    const auto texWidth = (float)tex.GetWidth();
    const auto texHeight = (float)tex.GetHeight();
    const float texClampX = texWidth - 1.0f;
    const float texClampY = texHeight - 1.0f;

    for (int y = yStart; y < yEnd; y++, itEdge0 += dv0, itEdge1 += dv1)
    {
        // calculate start and end pixel
        const int xStart = (int)ceilf(itEdge0.pos.x - 0.5f);
        const int xEnd = (int)ceilf(itEdge1.pos.x - 0.5f);

        // calculate scanline dTexCoord / dx
        const Vec2 dTcLine = (itEdge1.tc - itEdge0.tc) / (itEdge1.pos.x - itEdge0.pos.x);

        // create scanline tex coord interpolant and preStep
        Vec2 iTcLine = itEdge0.tc + dTcLine * (float(xStart) + 0.5f - itEdge0.pos.x);

        for (int x = xStart; x < xEnd; x++, iTcLine += dTcLine)
        {
            const auto xTex = (int)std::clamp(iTcLine.x * texWidth, 0.0f, texClampX);
            const auto yTex = (int)std::clamp(iTcLine.y * texHeight, 0.0f, texClampY);
            if (x > Graphics::ScreenWidth)
            {
                std::cout << x << std::endl;
            }
            PutPixel(x, y, tex.GetPixel(xTex, yTex));
        }
    }
}
