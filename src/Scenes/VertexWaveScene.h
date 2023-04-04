//
// Created by syf on 2023/4/2.
//

#ifndef CHILI_RENDERER_GL_VERTEXWAVESCENE_H
#define CHILI_RENDERER_GL_VERTEXWAVESCENE_H

#include "Scene.h"
#include "Shapes/Plane.h"
#include "DataStructures/Mat.h"
#include "Render/Pipeline.h"
#include "Effects/WaveVertexTextureEffect.h"

class VertexWaveScene : public Scene
{
public:
    typedef Pipeline<WaveVertexTextureEffect> Pipeline;
    typedef Pipeline::Vertex Vertex;
public:
    explicit VertexWaveScene(Graphics& gfx)
        : itList(Plane::GetSkinned<Vertex>(20)),
          pipeline(gfx)
    {
        pipeline.effect.ps.BindTexture("../images/sauron-bhole-100x100.png");
    }

    void Update(const Widgets::Controller& controller, const Widgets::Keyboard& kbd, float dt) override
    {
        using Button = Widgets::Controller::Button;

        // rotation
        auto rotationXY = controller.RightAxis() * dTheta * dt;
        float rotationZ = 0.0f;
        rotationZ += (float)controller.IsPressed(Button::LeftBumper) * dTheta * dt;
        rotationZ += (float)controller.IsPressed(Button::RightBumper) * -dTheta * dt;

        theta_x = wrap_angle(theta_x + rotationXY.y);
        theta_y = wrap_angle(theta_y + rotationXY.x);
        theta_z = wrap_angle(theta_z + rotationZ);

        if( kbd.KeyIsPressed( 'U' ) )
        {
            phi_x = wrap_angle( phi_x + dTheta * dt );
        }
        if( kbd.KeyIsPressed( 'I' ) )
        {
            phi_y = wrap_angle( phi_y + dTheta * dt );
        }
        if( kbd.KeyIsPressed( 'O' ) )
        {
            phi_z = wrap_angle( phi_z + dTheta * dt );
        }
        if( kbd.KeyIsPressed( 'J' ) )
        {
            phi_x = wrap_angle( phi_x - dTheta * dt );
        }
        if( kbd.KeyIsPressed( 'K' ) )
        {
            phi_y = wrap_angle( phi_y - dTheta * dt );
        }
        if( kbd.KeyIsPressed( 'L' ) )
        {
            phi_z = wrap_angle( phi_z - dTheta * dt );
        }

        // movement
        auto movement = controller.LeftAxis() * dt;
        offset_z += movement.y;

        // reset
        if (controller.IsPressed(Button::RightThumb))
        {
            theta_x = lerp(theta_x, 0.0f, LerpAlpha);
            theta_y = lerp(theta_y, 0.0f, LerpAlpha);
            theta_z = lerp(theta_z, 0.0f, LerpAlpha);
            offset_z = lerp(offset_z, 2.0f, LerpAlpha);
        }

        time += dt;
    }

    void Draw() override
    {
        pipeline.BeginFrame();
        Mat3 rot = Mat3::RotationX(theta_x) *
                   Mat3::RotationY(theta_y) *
                   Mat3::RotationZ(theta_z);
        const Mat3 rot_phi =
            Mat3::RotationX(phi_x) *
            Mat3::RotationY(phi_y) *
            Mat3::RotationZ(phi_z);

        const Vec3 trans = { 0, 0, offset_z };

        pipeline.effect.vs.BindRotation(rot);
        pipeline.effect.vs.BindTranslation(trans);
        pipeline.effect.vs.SetTime(time);

        pipeline.effect.gs.SetLightDirection(lightDirection * rot_phi);

        pipeline.Draw(itList);
    }

private:
    IndexedTriangleList<Vertex> itList;
    const float LerpAlpha = 0.5;
    Pipeline pipeline;
    static constexpr float dTheta = PI;
    float theta_x = 0, theta_y = 0, theta_z = 0;
    float offset_z = 2;
    float time = 0;
    float phi_x = 0.0f;
    float phi_y = 0.0f;
    float phi_z = 0.0f;
    Vec3 lightDirection = { 0.2f, -0.5f, 1.0f };
};

#endif //CHILI_RENDERER_GL_VERTEXWAVESCENE_H
