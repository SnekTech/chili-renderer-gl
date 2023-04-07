//
// Created by syf on 2023/3/19.
//

#ifndef CHILI_RENDERER_GL_SOLIDEFFECT_H
#define CHILI_RENDERER_GL_SOLIDEFFECT_H


#include "Render/Pipeline.h"
#include "DefaultVertexShader.h"
#include "DefaultGeometryShader.h"

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

    class VertexShader
    {
    public:
        struct Output
        {
            Vec4 pos{};
            Color color;

            Output() = default;

            explicit Output(const Vec4& pos)
                : pos(pos)
            {
            }

            Output(const Vec4& pos, const Vertex& src)
                : pos(pos), color(src.color)
            {
            }

            Output(const Vec4& pos, Color color)
                : pos(pos), color(color)
            {
            }

            Output& operator+=(const Output& rhs)
            {
                pos += rhs.pos;
                return *this;
            }

            Output operator+(const Output& rhs) const
            {
                return Output(*this) += rhs;
            }

            Output& operator-=(const Output& rhs)
            {
                pos -= rhs.pos;
                return *this;
            }

            Output operator-(const Output& rhs) const
            {
                return Output(*this) -= rhs;
            }

            Output& operator*=(float rhs)
            {
                pos *= rhs;
                return *this;
            }

            Output operator*(float rhs) const
            {
                return Output(*this) *= rhs;
            }

            Output& operator/=(float rhs)
            {
                pos /= rhs;
                return *this;
            }

            Output operator/(float rhs) const
            {
                return Output(*this) /= rhs;
            }
        };

    public:
        void BindWorld(const Mat4& transformation_in)
        {
            world = transformation_in;
            worldProj = world * proj;
        }

        void BindProjection(const Mat4& projectionMatrix)
        {
            proj = projectionMatrix;
            worldProj = world * proj;
        }

        [[nodiscard]] const Mat4& GetProj() const
        {
            return proj;
        }

        Output operator()(const Vertex& v) const
        {
            return { Vec4(v.pos) * worldProj, v.color };
        }

    private:
        Mat4 world = Mat4::Identity();
        Mat4 proj = Mat4::Identity();
        Mat4 worldProj = Mat4::Identity();
    };

    typedef DefaultGeometryShader<VertexShader::Output> GeometryShader;

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
    VertexShader vs;
    GeometryShader gs;
    PixelShader ps;
};


#endif //CHILI_RENDERER_GL_SOLIDEFFECT_H
