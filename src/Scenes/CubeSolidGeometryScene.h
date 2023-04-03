//
// Created by syf on 2023/4/2.
//

#ifndef CHILI_RENDERER_GL_CUBESOLIDGEOMETRYSCENE_H
#define CHILI_RENDERER_GL_CUBESOLIDGEOMETRYSCENE_H

#include "Scene.h"
#include "Shapes/Cube.h"
#include "DataStructures/Mat.h"
#include "Render/Pipeline.h"
#include "Effects/SolidGeometryEffect.h"

class CubeSolidGeometryScene : public Scene
{
public:
    typedef Pipeline<SolidGeometryEffect> Pipeline;
    typedef Pipeline::Vertex Vertex;
public:
    explicit CubeSolidGeometryScene(Graphics& gfx)
        : itList(Cube::GetPlain<Vertex>()),
          pipeline(gfx)
    {

        pipeline.effect.gs.BindColors(
            { Colors::Red, Colors::Green, Colors::Blue, Colors::Magenta, Colors::Yellow, Colors::Cyan }
        );
    }

    void Update(const Widgets::Controller& controller, const Widgets::Keyboard& kbd, float deltaTime) override
    {
        using Button = Widgets::Controller::Button;

        // rotation
        auto rotationXY = controller.RightAxis() * dTheta * deltaTime;
        float rotationZ = 0.0f;
        rotationZ += (float)controller.IsPressed(Button::LeftBumper) * dTheta * deltaTime;
        rotationZ += (float)controller.IsPressed(Button::RightBumper) * -dTheta * deltaTime;

        theta_x = wrap_angle(theta_x + rotationXY.y);
        theta_y = wrap_angle(theta_y + rotationXY.x);
        theta_z = wrap_angle(theta_z + rotationZ);

        // movement
        auto movement = controller.LeftAxis() * deltaTime;
        offset_z += movement.y;

        // reset
        if (controller.IsPressed(Button::RightThumb))
        {
            theta_x = lerp(theta_x, 0.0f, LerpAlpha);
            theta_y = lerp(theta_y, 0.0f, LerpAlpha);
            theta_z = lerp(theta_z, 0.0f, LerpAlpha);
            offset_z = lerp(offset_z, 2.0f, LerpAlpha);
        }
    }

    void Draw() override
    {
        pipeline.BeginFrame();
        Mat3 rot = Mat3::RotationX(theta_x) *
                   Mat3::RotationY(theta_y) *
                   Mat3::RotationZ(theta_z);

        const Vec3 trans = { 0, 0, offset_z };

        pipeline.effect.vs.BindRotation(rot);
        pipeline.effect.vs.BindTranslation(trans);

        pipeline.Draw(itList);
    }

private:
    IndexedTriangleList<Vertex> itList;
    const float LerpAlpha = 0.5;
    Pipeline pipeline;
    static constexpr float dTheta = PI;
    float theta_x = 0, theta_y = 0, theta_z = 0;
    float offset_z = 2;
};


#endif //CHILI_RENDERER_GL_CUBESOLIDGEOMETRYSCENE_H
