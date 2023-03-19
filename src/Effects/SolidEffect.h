//
// Created by syf on 2023/3/19.
//

#ifndef CHILI_RENDERER_GL_SOLIDEFFECT_H
#define CHILI_RENDERER_GL_SOLIDEFFECT_H


#include "Render/Pipeline.h"

class SolidEffect
{
public:
    class Vertex
    {
    public:
        Vertex() = default;

        explicit Vertex(const Vec3& pos)
            : pos(pos)
        {
        }

        Vertex(const Vec3& pos, const Vertex& src)
            : color(src.color), pos(pos)
        {
        } // change pos only when copying
        Vertex(const Vec3& pos, const Vec3& color)
            : color(color), pos(pos)
        {
        }

        Vertex& operator+=(const Vertex& rhs)
        {
            pos += rhs.pos;
            return *this;
        }

        Vertex operator+(const Vertex& rhs) const
        {
            return Vertex(*this) += rhs;
        }

        Vertex& operator-=(const Vertex& rhs)
        {
            pos -= rhs.pos;
            return *this;
        }

        Vertex operator-(const Vertex& rhs) const
        {
            return Vertex(*this) -= rhs;
        }

        Vertex& operator*=(float rhs)
        {
            pos *= rhs;
            return *this;
        }

        Vertex operator*(float rhs) const
        {
            return Vertex(*this) *= rhs;
        }

        Vertex& operator/=(float rhs)
        {
            pos /= rhs;
            return *this;
        }

        Vertex operator/(float rhs) const
        {
            return Vertex(*this) /= rhs;
        }

    public:
        Vec3 pos = { 0, 0, 0 };
        Color color;
    };

    class PixelShader
    {
    public:
        template<class Input>
        Color operator()(const Input& in) const
        {
            return Color(in.color);
        }
    };

public:
    PixelShader ps;
};


#endif //CHILI_RENDERER_GL_SOLIDEFFECT_H