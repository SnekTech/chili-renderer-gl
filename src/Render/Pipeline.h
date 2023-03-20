//
// Created by syf on 2023/3/19.
//

#ifndef CHILI_RENDERER_GL_PIPELINE_H
#define CHILI_RENDERER_GL_PIPELINE_H

#include <algorithm>
#include "Graphics.h"
#include "DataStructures/IndexedList.h"
#include "DataStructures/Mat.h"
#include "DataStructures/Triangle.h"
#include "PreClipScreenTransformer.h"
#include "ZBuffer.h"

template<class Effect>
class Pipeline
{
public:
    typedef typename Effect::Vertex Vertex;
public:
    Pipeline(Graphics& gfx)
        : gfx(gfx), zb(Graphics::ScreenWidth, Graphics::ScreenHeight)
    {
    }

    void Draw(IndexedTriangleList<Vertex>& triList)
    {
        ProcessVertices(triList.vertices, triList.indices);
    }

    void BindRotation(const Mat3& rotationIn)
    {
        rotation = rotationIn;
    }

    void BindTranslation(const Vec3 translationIn)
    {
        translation = translationIn;
    }

    void BeginFrame()
    {
        zb.Clear();
    }

private:
    void ProcessVertices(const std::vector<Vertex>& vertices, const std::vector<size_t>& indices)
    {
        std::vector<Vertex> verticesOut;

        for (const auto& v: vertices)
        {
            verticesOut.emplace_back(v.pos * rotation + translation, v);
        }

        AssembleTriangles(verticesOut, indices);
    }

    void AssembleTriangles(const std::vector<Vertex>& vertices, const std::vector<size_t>& indices)
    {
        for (size_t i = 0, end = indices.size() / 3; i < end; i++)
        {
            const auto& v0 = vertices[indices[i * 3]];
            const auto& v1 = vertices[indices[i * 3 + 1]];
            const auto& v2 = vertices[indices[i * 3 + 2]];

            bool isBackfaceCulled = (v1.pos - v0.pos).Cross(v2.pos - v0.pos) * v0.pos > 0.0f;
            if (!isBackfaceCulled)
            {
                ProcessTriangle(v0, v1, v2);
            }
        }
    }

    void ProcessTriangle(const Vertex& v0, const Vertex& v1, const Vertex& v2)
    {
        // geometry shader here
        PostProcessTriangleVertices(Triangle<Vertex>{ v0, v1, v2 });
    }

    void PostProcessTriangleVertices(Triangle<Vertex> triangle)
    {
        pst.Transform(triangle.v0);
        pst.Transform(triangle.v1);
        pst.Transform(triangle.v2);

        DrawTriangle(triangle);
    }

    void DrawTriangle(const Triangle<Vertex>& triangle)
    {
        const Vertex* pv0 = &triangle.v0;
        const Vertex* pv1 = &triangle.v1;
        const Vertex* pv2 = &triangle.v2;

        using std::swap;
        if (pv1->pos.y < pv0->pos.y) swap(pv0, pv1);
        if (pv2->pos.y < pv1->pos.y) swap(pv1, pv2);
        if (pv1->pos.y < pv0->pos.y) swap(pv0, pv1);

        if (pv0->pos.y == pv1->pos.y) // natural flat bottom
        {
            if (pv1->pos.x < pv2->pos.x) swap(pv0, pv1);

            DrawFlatBottomTriangle(*pv0, *pv1, *pv2);
            return;
        }

        if (pv1->pos.y == pv2->pos.y) // natural flat top
        {
            if (pv2->pos.x < pv1->pos.x) swap(pv1, pv2);

            DrawFlatTopTriangle(*pv0, *pv1, *pv2);
            return;
        }

        // general triangle
        const float alphaSplit = ((pv1->pos.y - pv0->pos.y) / (pv2->pos.y - pv0->pos.y));
        const auto vi = interpolate(*pv0, *pv2, alphaSplit);

        if (pv1->pos.x < vi.pos.x) // major right
        {
            DrawFlatTopTriangle(*pv0, *pv1, vi);
            DrawFlatBottomTriangle(*pv1, vi, *pv2);
        }
        else // major left
        {
            DrawFlatTopTriangle(*pv0, vi, *pv1);
            DrawFlatBottomTriangle(vi, *pv1, *pv2);
        }

    }

    void DrawFlatTopTriangle(const Vertex& v0, const Vertex& v1, const Vertex& v2)
    {
        const float deltaY = v2.pos.y - v0.pos.y;
        const auto dv0 = (v1 - v0) / deltaY;
        const auto dv1 = (v2 - v0) / deltaY;

        auto itEdge1 = v0;

        DrawFlatTriangle(v0, v1, v2, dv0, dv1, itEdge1);
    }

    void DrawFlatBottomTriangle(const Vertex& v0, const Vertex& v1, const Vertex& v2)
    {
        const float deltaY = v2.pos.y - v0.pos.y;
        const auto dv0 = (v2 - v0) / deltaY;
        const auto dv1 = (v2 - v1) / deltaY;

        auto itEdge1 = v1;

        DrawFlatTriangle(v0, v1, v2, dv0, dv1, itEdge1);
    }

    void DrawFlatTriangle(
        const Vertex& v0,
        const Vertex& v1,
        const Vertex& v2,
        const Vertex& dv0,
        const Vertex& dv1,
        Vertex itEdge1
    )
    {
        // create edge interpolant for left edge (always v0)
        auto itEdge0 = v0;

        // calculate start and end scanlines
        const int yStart = (int)ceilf(v0.pos.y - 0.5f);
        const int yEnd = (int)ceilf(v2.pos.y - 0.5f);

        // do interpolant preStep
        itEdge0 += dv0 * (float(yStart) + 0.5f - v0.pos.y);
        itEdge1 += dv1 * (float(yStart) + 0.5f - v0.pos.y);

        for (int y = yStart; y < yEnd; y++, itEdge0 += dv0, itEdge1 += dv1)
        {
            // calculate start and end pixel
            const int xStart = (int)ceilf(itEdge0.pos.x - 0.5f);
            const int xEnd = (int)ceilf(itEdge1.pos.x - 0.5f);

            auto iLine = itEdge0;
            const float dx = itEdge1.pos.x - itEdge0.pos.x;
            const auto diLine = (itEdge1 - iLine) / dx;

            // create scanline tex coord interpolant and preStep
            iLine += diLine * (float(xStart) + 0.5f - itEdge0.pos.x);

            for (int x = xStart; x < xEnd; x++, iLine += diLine)
            {
                const float z = 1.0f / iLine.pos.z;
                if (zb.TestAndSet(x, y, z))
                {
                    const auto attr = iLine * z;
                    gfx.PutPixel(x, y, effect.ps(attr));
                }
            }
        }

    }

public:
    Effect effect;
private:
    Graphics& gfx;
    PreClipScreenTransformer pst;
    ZBuffer zb;
    Mat3 rotation = Mat3::Identity();
    Vec3 translation = { 0, 0, 0 };
};

#endif //CHILI_RENDERER_GL_PIPELINE_H
