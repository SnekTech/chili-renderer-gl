//
// Created by syf on 2023/3/19.
//

#pragma once

#include <algorithm>
#include <memory>

#include "Graphics.h"
#include "DataStructures/IndexedList.h"
#include "DataStructures/Mat.h"
#include "DataStructures/Triangle.h"
#include "NDCScreenTransformer.h"
#include "ZBuffer.h"

template<class Effect>
class Pipeline
{
public:
    typedef typename Effect::Vertex Vertex;
    typedef typename Effect::VertexShader::Output VSOut;
    typedef typename Effect::GeometryShader::Output GSOut;
public:
    explicit Pipeline(Graphics& gfx)
        : Pipeline(gfx, std::make_shared<ZBuffer>(Graphics::ScreenWidth, Graphics::ScreenHeight))
    {
    }

    Pipeline(Graphics& gfx, std::shared_ptr<ZBuffer> pZb)
        : gfx(gfx), pZb(std::move(pZb))
    {
        assert(this->pZb->GetHeight() == gfx.ScreenHeight && this->pZb->GetWidth() == gfx.ScreenWidth);
    }

    void Draw(IndexedTriangleList<Vertex>& triList)
    {
        ProcessVertices(triList.vertices, triList.indices);
    }

    void BeginFrame()
    {
        pZb->Clear();
    }

private:
    void ProcessVertices(const std::vector<Vertex>& vertices, const std::vector<size_t>& indices)
    {
        std::vector<VSOut> verticesOut(vertices.size());

        std::transform(vertices.begin(), vertices.end(), verticesOut.begin(), effect.vs);

        AssembleTriangles(verticesOut, indices);
    }

    void AssembleTriangles(const std::vector<VSOut>& vertices, const std::vector<size_t>& indices)
    {
        const auto eyePos = Vec4{ 0.0f, 0.0f, 0.0f, 1.0f } * effect.vs.GetProj();
        for (size_t i = 0, end = indices.size() / 3; i < end; i++)
        {
            const auto& v0 = vertices[indices[i * 3]];
            const auto& v1 = vertices[indices[i * 3 + 1]];
            const auto& v2 = vertices[indices[i * 3 + 2]];

            Vec3 pos0 = v0.pos;
            Vec3 pos1 = v1.pos;
            Vec3 pos2 = v2.pos;
            bool isBackfaceCulled = (pos1 - pos0).Cross(pos2 - pos0) * Vec3(pos0 - eyePos) > 0.0f;
            if (!isBackfaceCulled)
            {
                ProcessTriangle(v0, v1, v2, i);
            }
        }
    }

    void ProcessTriangle(const VSOut& v0, const VSOut& v1, const VSOut& v2, size_t triangle_index)
    {
        // geometry shader here
        ClipCullTriangle(effect.gs(v0, v1, v2, triangle_index));
    }

    void ClipCullTriangle(const Triangle<GSOut>& t)
    {
        if (abs(t.v0.pos.x) > t.v0.pos.w &&
            abs(t.v1.pos.x) > t.v1.pos.w &&
            abs(t.v2.pos.x) > t.v2.pos.w)
            return;

        if (abs(t.v0.pos.y) > t.v0.pos.w &&
            abs(t.v1.pos.y) > t.v1.pos.w &&
            abs(t.v2.pos.y) > t.v2.pos.w)
            return;

        if (t.v0.pos.z > t.v0.pos.w &&
            t.v1.pos.z > t.v1.pos.w &&
            t.v2.pos.z > t.v2.pos.w)
            return;

        if (t.v0.pos.z < 0.0f &&
            t.v1.pos.z < 0.0f &&
            t.v2.pos.z < 0.0f)
            return;

        const auto Clip1 = [this](GSOut v0, GSOut v1, GSOut v2)
        {
            const float alphaA = (-v0.pos.z) / (v1.pos.z - v0.pos.z);
            const float alphaB = (-v0.pos.z) / (v2.pos.z - v0.pos.z);

            const auto v0a = interpolate(v0, v1, alphaA);
            const auto v0b = interpolate(v0, v2, alphaB);

            PostProcessTriangleVertices(Triangle<GSOut>{ v0a, v1, v2 });
            PostProcessTriangleVertices(Triangle<GSOut>{ v0b, v0a, v2 });
        };

        const auto Clip2 = [this](GSOut v0, GSOut v1, GSOut v2)
        {
            const float alpha0 = (-v0.pos.z) / (v2.pos.z - v0.pos.z);
            const float alpha1 = (-v1.pos.z) / (v2.pos.z - v1.pos.z);

            v0 = interpolate(v0, v2, alpha0);
            v1 = interpolate(v1, v2, alpha1);

            PostProcessTriangleVertices(Triangle<GSOut>{ v0, v1, v2 });
        };

        if (t.v0.pos.z < 0.0f)
        {
            if (t.v1.pos.z < 0.0f)
                Clip2(t.v0, t.v1, t.v2);
            else if (t.v2.pos.z < 0.0f)
                Clip2(t.v0, t.v2, t.v1);
            else
                Clip1(t.v0, t.v1, t.v2);
        }
        else if (t.v1.pos.z < 0.0f)
        {
            if (t.v2.pos.z < 0.0f)
                Clip2(t.v1, t.v2, t.v0);
            else
                Clip1(t.v1, t.v0, t.v2);
        }
        else if (t.v2.pos.z < 0.0f)
            Clip1(t.v2, t.v0, t.v1);
        else
            // no near clipping necessary
            PostProcessTriangleVertices(t);
    }

    void PostProcessTriangleVertices(Triangle<GSOut> triangle)
    {
        pst.Transform(triangle.v0);
        pst.Transform(triangle.v1);
        pst.Transform(triangle.v2);

        DrawTriangle(triangle);
    }

    void DrawTriangle(const Triangle<GSOut>& triangle)
    {
        const GSOut* pv0 = &triangle.v0;
        const GSOut* pv1 = &triangle.v1;
        const GSOut* pv2 = &triangle.v2;

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

    void DrawFlatTopTriangle(const GSOut& v0, const GSOut& v1, const GSOut& v2)
    {
        const float deltaY = v2.pos.y - v0.pos.y;
        const auto dv0 = (v1 - v0) / deltaY;
        const auto dv1 = (v2 - v0) / deltaY;

        auto itEdge1 = v0;

        DrawFlatTriangle(v0, v1, v2, dv0, dv1, itEdge1);
    }

    void DrawFlatBottomTriangle(const GSOut& v0, const GSOut& v1, const GSOut& v2)
    {
        const float deltaY = v2.pos.y - v0.pos.y;
        const auto dv0 = (v2 - v0) / deltaY;
        const auto dv1 = (v2 - v1) / deltaY;

        auto itEdge1 = v1;

        DrawFlatTriangle(v0, v1, v2, dv0, dv1, itEdge1);
    }

    void DrawFlatTriangle(
        const GSOut& v0,
        const GSOut& v1,
        const GSOut& v2,
        const GSOut& dv0,
        const GSOut& dv1,
        GSOut itEdge1
    )
    {
        // create edge interpolant for left edge (always v0)
        auto itEdge0 = v0;

        // calculate start and end scanlines
        const int yStart = std::max((int)ceilf(v0.pos.y - 0.5f), 0);
        const int yEnd = std::min((int)ceilf(v2.pos.y - 0.5f), (int)Graphics::ScreenHeight - 1);

        // do interpolant preStep
        itEdge0 += dv0 * (float(yStart) + 0.5f - v0.pos.y);
        itEdge1 += dv1 * (float(yStart) + 0.5f - v0.pos.y);

        for (int y = yStart; y < yEnd; y++, itEdge0 += dv0, itEdge1 += dv1)
        {
            // calculate start and end pixel
            const int xStart = std::max((int)ceilf(itEdge0.pos.x - 0.5f), 0);
            const int xEnd = std::min((int)ceilf(itEdge1.pos.x - 0.5f), (int)Graphics::ScreenWidth - 1);

            auto iLine = itEdge0;
            const float dx = itEdge1.pos.x - itEdge0.pos.x;
            const auto diLine = (itEdge1 - iLine) / dx;

            // create scanline tex coord interpolant and preStep
            iLine += diLine * (float(xStart) + 0.5f - itEdge0.pos.x);

            for (int x = xStart; x < xEnd; x++, iLine += diLine)
            {
                if (pZb->TestAndSet(x, y, iLine.pos.z))
                {
                    const float w = 1.0f / iLine.pos.w;
                    const auto attr = iLine * w;
                    gfx.PutPixel(x, y, effect.ps(attr));
                }
            }
        }

    }

public:
    Effect effect;
private:
    Graphics& gfx;
    NDCScreenTransformer pst;
    std::shared_ptr<ZBuffer> pZb;
};
