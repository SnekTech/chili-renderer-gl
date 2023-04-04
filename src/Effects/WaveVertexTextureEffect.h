//
// Created by syf on 2023/4/.
//

#ifndef CHILI_RENDERER_GL_WAVEVERTEXTEXTUREEFFECT_H
#define CHILI_RENDERER_GL_WAVEVERTEXTEXTUREEFFECT_H

#include <memory>
#include "DataStructures/Vec3.h"
#include "DataStructures/Mat.h"
#include "Render/Surface.h"

class WaveVertexTextureEffect
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
            : t(src.t), pos(pos)
        {
        } // change pos only when copying
        Vertex(const Vec3& pos, const Vec2& t)
            : t(t), pos(pos)
        {
        }

        Vertex& operator+=(const Vertex& rhs)
        {
            pos += rhs.pos;
            t += rhs.t;
            return *this;
        }

        Vertex operator+(const Vertex& rhs) const
        {
            return Vertex(*this) += rhs;
        }

        Vertex& operator-=(const Vertex& rhs)
        {
            pos -= rhs.pos;
            t -= rhs.t;
            return *this;
        }

        Vertex operator-(const Vertex& rhs) const
        {
            return Vertex(*this) -= rhs;
        }

        Vertex& operator*=(float rhs)
        {
            pos *= rhs;
            t *= rhs;
            return *this;
        }

        Vertex operator*(float rhs) const
        {
            return Vertex(*this) *= rhs;
        }

        Vertex& operator/=(float rhs)
        {
            pos /= rhs;
            t /= rhs;
            return *this;
        }

        Vertex operator/(float rhs) const
        {
            return Vertex(*this) /= rhs;
        }

    public:
        Vec3 pos = { 0, 0, 0 };
        Vec2 t = { 0, 0 };
    };

    class VertexShader
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
            Vec3 pos = in.pos * rotation + translation;
            pos.y += amplitude * std::sin(time * freqScroll + pos.x * freqWave);
            return { pos, in.t };
        }

        void SetTime(float t)
        {
            time = t;
        }

    private:
        Mat3 rotation = Mat3::Identity();
        Vec3 translation;
        float time = 0.0f;
        float freqWave = 10.0f;
        float freqScroll = 5.0f;
        float amplitude = 0.05f;
    };

    class GeometryShader
    {
    public:
        class Output
        {
        public:
            Output() = default;

            explicit Output(const Vec3& pos)
                : pos(pos)
            {
            }

            Output(const Vec3& pos, const Output& src)
                :
                t(src.t),
                l(src.l),
                pos(pos)
            {
            }

            Output(const Vec3& pos, const Vec2& t, float l)
                :
                t(t),
                l(l),
                pos(pos)
            {
            }

            Output& operator+=(const Output& rhs)
            {
                pos += rhs.pos;
                t += rhs.t;
                return *this;
            }

            Output operator+(const Output& rhs) const
            {
                return Output(*this) += rhs;
            }

            Output operator-=(const Output& rhs)
            {
                pos -= rhs.pos;
                t -= rhs.t;
                return *this;
            }

            Output operator-(const Output& rhs) const
            {
                return Output(*this) -= rhs;
            }


            Output operator*=(float rhs)
            {
                pos *= rhs;
                t *= rhs;
                return *this;
            }

            Output operator*(float rhs) const
            {
                return Output(*this) *= rhs;
            }

            Output operator/=(float rhs)
            {
                pos /= rhs;
                t /= rhs;
                return *this;
            }

            Output operator/(float rhs) const
            {
                return Output(*this) /= rhs;
            }

        public:
            Vec3 pos{};
            Vec2 t{};
            float l{};
        };

    public:
        Triangle<Output> operator()(
            const VertexShader::Output& in0,
            const VertexShader::Output& in1,
            const VertexShader::Output& in2,
            size_t triangleIndex)
        {
            const auto n = ((in1.pos - in2.pos).Cross(in2.pos - in0.pos)).GetNormalized();
            const auto l = std::min(1.0f, diffuse * std::max(0.0f, -n * dir) + ambient);
            return
                {
                    { in0.pos, in0.t, l },
                    { in1.pos, in1.t, l },
                    { in2.pos, in2.t, l },
                };
        }

        void SetDiffuseLight(float d)
        {
            diffuse = d;
        }

        void SetAmbientLight(float a)
        {
            ambient = a;
        }

        void SetLightDirection(const Vec3& direction)
        {
            assert(direction.LenSq() >= 0.001f);
            dir = direction.GetNormalized();
        }

    private:
        Mat3 rotation;
        Vec3 translation;
        Vec3 dir = { 0, 0, 0 };
        float diffuse = 1;
        float ambient = 0.15;
    };

    class PixelShader
    {
    public:
        template<class Input>
        Color operator()(const Input& in) const
        {
            const auto x = (int)std::clamp(in.t.x * texWidth, 0.0f, texClampX);
            const auto y = (int)std::clamp(in.t.y * texHeight, 0.0f, texClampY);
            const Vec3 color = Vec3(pTex->GetPixel(x, y));
            return Color(color * in.l);
        }

        void BindTexture(const std::string& filename)
        {
            pTex = std::make_unique<Surface>(Surface::FromFile(filename));
            texWidth = float(pTex->GetWidth());
            texHeight = float(pTex->GetHeight());
            texClampX = texWidth - 1;
            texClampY = texHeight - 1;
        }

    private:
        std::unique_ptr<Surface> pTex;
        float texWidth;
        float texHeight;
        float texClampX;
        float texClampY;
    };

public:
    VertexShader vs;
    GeometryShader gs;
    PixelShader ps;
};

#endif //CHILI_RENDERER_GL_WAVEVERTEXTEXTUREEFFECT_H
