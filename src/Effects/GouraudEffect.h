//
// Created by syf on 2023/4/5.
//

#pragma once

#include "Render/Pipeline.h"
#include "DefaultGeometryShader.h"

class GouraudEffect
{
public:
    struct Vertex
    {
        Vertex() = default;

        explicit Vertex(const Vec3& pos)
            : pos(pos)
        {
        }

        Vertex(const Vec3& pos, const Vertex& src)
            : pos(pos), n(src.n)
        {
        }

        Vertex(const Vec3& pos, const Vec3& n)
            : pos(pos), n(n)
        {
        }

        Vertex& operator+=(const Vertex& rhs)
        {
            pos += rhs.pos;
            return *this;
        }

        Vertex operator+(const Vertex& rhs)
        {
            return Vertex(*this) += rhs;
        }

        Vertex& operator-=(const Vertex& rhs)
        {
            pos -= rhs.pos;
            return *this;
        }

        Vertex operator-(const Vertex& rhs)
        {
            return Vertex(*this) += rhs;
        }

        Vertex& operator*=(float rhs)
        {
            pos *= rhs;
            return *this;
        }

        Vertex operator*(float rhs)
        {
            return Vertex(*this) *= rhs;
        }

        Vertex& operator/=(float rhs)
        {
            pos /= rhs;
            return *this;
        }

        Vertex operator/(float rhs)
        {
            return Vertex(*this) /= rhs;
        }

        Vec3 pos{};
        Vec3 n{};
    };

    class VertexShader
    {
    public:
        struct Output
        {
            Output() = default;

            explicit Output(const Vec3& pos)
                :
                pos(pos)
            {
            }

            Output(const Vec3& pos, const Output& src)
                :
                color(src.color),
                pos(pos)
            {
            }

            Output(const Vec3& pos, const Vec3& color)
                :
                color(color),
                pos(pos)
            {
            }

            Output& operator+=(const Output& rhs)
            {
                pos += rhs.pos;
                color += rhs.color;
                return *this;
            }

            Output operator+(const Output& rhs) const
            {
                return Output(*this) += rhs;
            }

            Output& operator-=(const Output& rhs)
            {
                pos -= rhs.pos;
                color -= rhs.color;
                return *this;
            }

            Output operator-(const Output& rhs) const
            {
                return Output(*this) -= rhs;
            }

            Output& operator*=(float rhs)
            {
                pos *= rhs;
                color *= rhs;
                return *this;
            }

            Output operator*(float rhs) const
            {
                return Output(*this) *= rhs;
            }

            Output& operator/=(float rhs)
            {
                pos /= rhs;
                color /= rhs;
                return *this;
            }

            Output operator/(float rhs) const
            {
                return Output(*this) /= rhs;
            }

            Vec3 pos{};
            Vec3 color{};
        };


    public:
        void BindRotation(const Mat3& rotation_in)
        {
            rotation = rotation_in;
        }

        void BindTranslation(const Vec3& translation_in)
        {
            translation = translation_in;
        }

        Output operator()(const Vertex& v) const
        {
            // calculate intensity based on angle of incidence
            const auto d = diffuse * std::max(0.0f, -(v.n * rotation) * dir);
            // add diffuse+ambient, filter by material color, saturate and scale
            const auto c = color.GetHadamard(d + ambient).Saturate() * 255.0f;
            return { v.pos * rotation + translation, c };
        }

        void SetDiffuseLight(const Vec3& c)
        {
            diffuse = { c.x, c.y, c.z };
        }

        void SetAmbientLight(const Vec3& c)
        {
            ambient = { c.x, c.y, c.z };
        }

        void SetLightDirection(const Vec3& dl)
        {
            assert(dl.LenSq() >= 0.001f);
            dir = dl.GetNormalized();
        }

        void SetMaterialColor(Color c)
        {
            color = Vec3(c);
        }

    private:
        Mat3 rotation;
        Vec3 translation;
        Vec3 dir = { 0.0f, 0.0f, 1.0f };
        Vec3 diffuse = { 1.0f, 1.0f, 1.0f };
        Vec3 ambient = { 0.1f, 0.1f, 0.1f };
        Vec3 color = { 0.8f, 0.85f, 1.0f };
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
    GeometryShader  gs;
    PixelShader ps;
};