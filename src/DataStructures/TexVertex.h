//
// Created by syf on 2023/3/16.
//

#ifndef CHILI_RENDERER_GL_TEXVERTEX_H
#define CHILI_RENDERER_GL_TEXVERTEX_H

#include "Vec2.h"
#include "Vec3.h"

class TexVertex
{
public:
    TexVertex(const Vec3& pos, const Vec2& tc)
        : pos(pos), tc(tc)
    {}
    [[nodiscard]] TexVertex InterpolateTo(const TexVertex& dest, float alpha) const
    {

        TexVertex vi = {
            pos.InterpolateTo(dest.pos, alpha),
            tc.InterpolateTo(dest.tc, alpha)
        };

        return vi;
    }

    Vec3 pos;
    Vec2  tc;
};

#endif //CHILI_RENDERER_GL_TEXVERTEX_H
