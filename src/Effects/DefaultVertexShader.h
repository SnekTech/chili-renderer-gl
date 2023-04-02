//
// Created by syf on 2023/4/2.
//

#ifndef CHILI_RENDERER_GL_DEFAULTVERTEXSHADER_H
#define CHILI_RENDERER_GL_DEFAULTVERTEXSHADER_H

#include "DataStructures/Mat.h"

template<class Vertex>
class DefaultVertexShader
{
public:
    typedef Vertex Output;
public:
    void BindRotation(const Mat3& rotation_in)
    {
        rotation = rotation_in;
    }

    void BindTranslation(const Vec3& translation_in)
    {
        translation = translation_in;
    }

    Output operator()(const Vertex& in) const
    {
        return { in.pos * rotation + translation, in };
    }

private:
    Mat3 rotation = Mat3::Identity();
    Vec3 translation;
};

#endif //CHILI_RENDERER_GL_DEFAULTVERTEXSHADER_H
