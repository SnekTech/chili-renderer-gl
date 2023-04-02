//
// Created by syf on 2023/4/2.
//

#ifndef CHILI_RENDERER_GL_DEFAULTGEOMETRYSHADER_H
#define CHILI_RENDERER_GL_DEFAULTGEOMETRYSHADER_H

#include "DataStructures/Triangle.h"

template<class Vertex>
class DefaultGeometryShader
{
public:
    typedef Vertex Output;
public:
    Triangle<Output> operator()(
        const Vertex& in0, const Vertex& in1, const Vertex& in2, unsigned int triangle_index) const
    {
        return { in0, in1, in2 };
    }
};

#endif //CHILI_RENDERER_GL_DEFAULTGEOMETRYSHADER_H
