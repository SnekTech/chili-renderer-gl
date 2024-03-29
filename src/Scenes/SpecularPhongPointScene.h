#pragma once

#include "Scene.h"
#include "Shapes/Cube.h"
#include "DataStructures/Mat.h"
#include "Render/Pipeline.h"
#include "Effects/SpecularPhongPointEffect.h"
#include "Effects/SolidEffect.h"
#include "Shapes/Sphere.h"

class SpecularPhongPointScene : public Scene
{
public:
    typedef ::Pipeline<SpecularPhongPointEffect> Pipeline;
    typedef ::Pipeline<SolidEffect> LightIndicatorPipeline;
    typedef Pipeline::Vertex Vertex;
public:
    SpecularPhongPointScene(Graphics& gfx, IndexedTriangleList<Vertex> tl)
        :
        itlist(std::move(tl)),
        pZb(std::make_shared<ZBuffer>(Graphics::ScreenWidth, Graphics::ScreenHeight)),
        pipeline(gfx, pZb),
        liPipeline(gfx, pZb)
    {
        itlist.AdjustToTrueCenter();
        offset_z = itlist.GetRadius() * 1.6f;

        for (auto& v: lightIndicator.vertices)
        {
            v.color = Colors::White;
        }
    }

    void Update(const Widgets::Controller& controller, const Widgets::Keyboard& kbd, float dt) override
    {
        if (kbd.KeyIsPressed('Q'))
        {
            theta_x = wrap_angle(theta_x + dTheta * dt);
        }
        if (kbd.KeyIsPressed('W'))
        {
            theta_y = wrap_angle(theta_y + dTheta * dt);
        }
        if (kbd.KeyIsPressed('E'))
        {
            theta_z = wrap_angle(theta_z + dTheta * dt);
        }
        if (kbd.KeyIsPressed('A'))
        {
            theta_x = wrap_angle(theta_x - dTheta * dt);
        }
        if (kbd.KeyIsPressed('S'))
        {
            theta_y = wrap_angle(theta_y - dTheta * dt);
        }
        if (kbd.KeyIsPressed('D'))
        {
            theta_z = wrap_angle(theta_z - dTheta * dt);
        }
        if (kbd.KeyIsPressed('U'))
        {
            lpos_x += 0.2f * dt;
        }
        if (kbd.KeyIsPressed('I'))
        {
            lpos_y += 0.2f * dt;
        }
        if (kbd.KeyIsPressed('O'))
        {
            lpos_z += 0.2f * dt;
        }
        if (kbd.KeyIsPressed('J'))
        {
            lpos_x -= 0.2f * dt;
        }
        if (kbd.KeyIsPressed('K'))
        {
            lpos_y -= 0.2f * dt;
        }
        if (kbd.KeyIsPressed('L'))
        {
            lpos_z -= 0.2f * dt;
        }
        if (kbd.KeyIsPressed('R'))
        {
            offset_z += 0.7f * dt;
        }
        if (kbd.KeyIsPressed('F'))
        {
            offset_z -= 0.7f * dt;
        }
        if (kbd.KeyIsPressed('N'))
            phi -= 1.8f * dt;
        if (kbd.KeyIsPressed('M'))
            phi += 1.8f * dt;
    }

    void Draw() override
    {
        pipeline.BeginFrame();

        const auto proj = Mat4::ProjectionHFOV(100.0f, 1.33333f, 0.5f, 4.0f);
        pipeline.effect.vs.BindWorld(
            Mat4::RotationX(theta_x) *
            Mat4::RotationY(theta_y) *
            Mat4::RotationZ(theta_z) *
            Mat4::Translation(0.0f, 0.0f, offset_z) *
            Mat4::RotationY(phi)
        );
        pipeline.effect.vs.BindProjection(proj);
        pipeline.effect.ps.SetLightPosition({ lpos_x, lpos_y, lpos_z });
        // render triangles
        pipeline.Draw(itlist);

        liPipeline.effect.vs.BindWorld(Mat4::Translation(lpos_x, lpos_y, lpos_z));
        liPipeline.effect.vs.BindProjection(proj);
        liPipeline.Draw(lightIndicator);
    }

private:
    IndexedTriangleList<Vertex> itlist;
    IndexedTriangleList<SolidEffect::Vertex> lightIndicator = Sphere::GetPlain<SolidEffect::Vertex>(0.05f);
    std::shared_ptr<ZBuffer> pZb;
    Pipeline pipeline;
    LightIndicatorPipeline liPipeline;
    static constexpr float dTheta = PI;
    float offset_z = 2.0f;
    float theta_x = 0.0f;
    float theta_y = 0.0f;
    float theta_z = 0.0f;
    float lpos_x = 0.0f;
    float lpos_y = 0.0f;
    float lpos_z = 0.6f;
    float phi = 0.0f;
};