#pragma once

#include "Render/Pipeline.h"
#include "DefaultGeometryShader.h"

// flat shading with vertex normals
class SpecularPhongPointEffect
{
public:
    // the vertex type that will be input into the pipeline
    class Vertex
    {
    public:
        Vertex() = default;

        explicit Vertex(const Vec3& pos)
            :
            pos(pos)
        {
        }

        Vertex(const Vec3& pos, const Vertex& src)
            :
            n(src.n),
            pos(pos)
        {
        }

        Vertex(const Vec3& pos, const Vec3& n)
            :
            n(n),
            pos(pos)
        {
        }

    public:
        Vec3 pos{};
        Vec3 n{};
    };

    // calculate color based on normal to light angle
    // no interpolation of color attribute
    class VertexShader
    {
    public:
        class Output
        {
        public:
            Output() = default;

            explicit Output(const Vec4& pos)
                :
                pos(pos)
            {
            }

            Output(const Vec4& pos, const Output& src)
                :
                pos(pos),
                n(src.n),
                worldPos(src.worldPos)
            {
            }

            Output(const Vec4& pos, const Vec4& n, const Vec3& worldPos)
                :
                pos(pos),
                n(n),
                worldPos(worldPos)
            {
            }

            Output& operator+=(const Output& rhs)
            {
                pos += rhs.pos;
                n += rhs.n;
                worldPos += rhs.worldPos;
                return *this;
            }

            Output operator+(const Output& rhs) const
            {
                return Output(*this) += rhs;
            }

            Output& operator-=(const Output& rhs)
            {
                pos -= rhs.pos;
                n -= rhs.n;
                worldPos -= rhs.worldPos;
                return *this;
            }

            Output operator-(const Output& rhs) const
            {
                return Output(*this) -= rhs;
            }

            Output& operator*=(float rhs)
            {
                pos *= rhs;
                n *= rhs;
                worldPos *= rhs;
                return *this;
            }

            Output operator*(float rhs) const
            {
                return Output(*this) *= rhs;
            }

            Output& operator/=(float rhs)
            {
                pos /= rhs;
                n /= rhs;
                worldPos /= rhs;
                return *this;
            }

            Output operator/(float rhs) const
            {
                return Output(*this) /= rhs;
            }

        public:
            Vec4 pos{};
            Vec4 n{};
            Vec3 worldPos{};
        };

    public:
        void BindTransformation(const Mat4& transformationIn)
        {
            transformation = transformationIn;
        }

        Output operator()(const Vertex& v) const
        {
            const auto positionTransformed = Vec4(v.pos) * transformation;
            const auto normalTransformed = Vec4(v.n, 0.0f) * transformation;
            return { positionTransformed, normalTransformed, positionTransformed };
        }

    private:
        Mat4 transformation;
    };

    // default gs passes vertices through and outputs triangle
    typedef DefaultGeometryShader<VertexShader::Output> GeometryShader;

    // invoked for each pixel of a triangle
    // takes an input of attributes that are the
    // result of interpolating vertex attributes
    // and outputs a color
    class PixelShader
    {
    public:
        template<class Input>
        Color operator()(const Input& in) const
        {
            const Vec3 raw = in.n;
            const auto surfaceN = raw.GetNormalized();
            const auto v_to_l = light_pos - in.worldPos;
            const auto distance = v_to_l.Len();
            const auto dir = v_to_l / distance;

            const auto attenuation = 1.0f /
                                     (constant_attenuation + linear_attenuation * distance +
                                      quadratic_attenuation * sq(distance));
            const auto diffuse = light_diffuse * attenuation * std::max(0.0f, surfaceN * dir);

            const auto w = surfaceN * (v_to_l * surfaceN);
            const auto r = w * 2.0f - v_to_l;

            const auto specular = light_diffuse * specular_intensity * std::pow(
                std::max(0.0f, -r.GetNormalized() * in.worldPos.GetNormalized()), specular_power);

            return Color(material_color.GetHadamard(diffuse + light_ambient + specular).Saturate() * 255.0f);
        }

        void SetDiffuseLight(const Vec3& c)
        {
            light_diffuse = c;
        }

        void SetAmbientLight(const Vec3& c)
        {
            light_ambient = c;
        }

        void SetLightPosition(const Vec3& pos_in)
        {
            light_pos = pos_in;
        }


    private:
        Vec3 light_pos = { 0.0f, 0.0f, 0.5f };
        Vec3 light_diffuse = { 1.0f, 1.0f, 1.0f };
        Vec3 light_ambient = { 0.1f, 0.1f, 0.1f };
        Vec3 material_color = { 0.8f, 0.85f, 1.0f };
        // diffuse
        float linear_attenuation = 1.0f;
        float quadratic_attenuation = 2.619f;
        float constant_attenuation = 0.382f;
        // specular
        float specular_power = 30.0f;
        float specular_intensity = 0.6f;
    };

public:
    VertexShader vs{};
    GeometryShader gs;
    PixelShader ps;
};