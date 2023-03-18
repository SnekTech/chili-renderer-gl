//
// Created by syf on 2023/3/9.
//

#ifndef CHILI_RENDERER_GL_GRAPHICS_H
#define CHILI_RENDERER_GL_GRAPHICS_H

#include "GLFW/glfw3.h"

#include "Surface.h"
#include "DataStructures/Vec2.h"
#include "DataStructures/TexVertex.h"

class Graphics
{
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

    ~Graphics();

    void DrawTriangle(Vec2 v0, Vec2 v1, Vec2 v2, Color color);

    void DrawTriangleTex(const TexVertex& v0, const TexVertex& v1, const TexVertex& v2, const Surface& tex);

    void PutPixel(int x, int y, int r, int g, int b)
    {
        PutPixel(x, y, { (unsigned char)r, (unsigned char)g, (unsigned char)b });
    }

    void PutPixel(int x, int y, Color color)
    {
        frameBuffer.PutPixel(x, y, color);
    }

    void DrawLine(const Vec2& p1, const Vec2& p2, Color color)
    {
        DrawLine(p1.x, p1.y, p2.x, p2.y, color);
    }

    void DrawLine(float x1, float y1, float x2, float y2, Color color);

private:
    void DrawFlatTopTriangle(Vec2 v0, Vec2 v1, Vec2 v2, Color color);

    void DrawFlatBottomTriangle(Vec2 v0, Vec2 v1, Vec2 v2, Color color);

    void DrawFlatTopTriangleTex(const TexVertex& v0, const TexVertex& v1, const TexVertex& v2, const Surface& tex);

    void DrawFlatBottomTriangleTex(const TexVertex& v0, const TexVertex& v1, const TexVertex& v2, const Surface& tex);

    void DrawFlagTriangleTex(const TexVertex& v0, const TexVertex& v1, const TexVertex& v2, const Surface& tex,
        const TexVertex& dv0, const TexVertex& dv1, TexVertex& itEdge1);

private:
    unsigned int VBO, VAO, EBO;
    Surface frameBuffer;
public:
    static constexpr unsigned int ScreenWidth = 640u;
    static constexpr unsigned int ScreenHeight = 640u;
};


#endif //CHILI_RENDERER_GL_GRAPHICS_H
