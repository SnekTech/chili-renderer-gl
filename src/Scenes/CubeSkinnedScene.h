//
// Created by syf on 2023/3/19.
//

#ifndef CHILI_RENDERER_GL_CUBESKINNEDSCENE_H
#define CHILI_RENDERER_GL_CUBESKINNEDSCENE_H

#include "Scene.h"
#include "Shapes/Cube.h"
#include "DataStructures/Mat.h"
#include "Render/Pipeline.h"
#include "Effects/TextureEffect.h"

class CubeSkinnedScene : public Scene
{
public:
    typedef Pipeline<TextureEffect> Pipeline;
    typedef Pipeline::Vertex Vertex;
public:
    explicit CubeSkinnedScene(Graphics& gfx, const std::string& filename)
        : itList(Cube::GetSkinned<Vertex>()),
          pipeline(gfx)
    {
        pipeline.effect.ps.BindTexture(filename);
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
    const float LerpAlpha = 0.5;
    IndexedTriangleList<Vertex> itList;
    Pipeline pipeline;
    float theta_x = 0, theta_y = 0, theta_z = 0;
    static constexpr float dTheta = PI;
    float offset_z = 2;
};

#endif //CHILI_RENDERER_GL_CUBESKINNEDSCENE_H
