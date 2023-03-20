//
// Created by syf on 2023/3/19.
//

#ifndef CHILI_RENDERER_GL_CUBEVERTEXCOLORSCENE_H
#define CHILI_RENDERER_GL_CUBEVERTEXCOLORSCENE_H

#include "Scene.h"
#include "Shapes/Cube.h"
#include "DataStructures/Mat.h"
#include "Render/Pipeline.h"
#include "Effects/VertexColorEffect.h"

class CubeVertexColorScene : public Scene
{
public:
    typedef Pipeline<VertexColorEffect> Pipeline;
    typedef Pipeline::Vertex Vertex;
public:
    explicit CubeVertexColorScene(Graphics& gfx)
        : itList(Cube::GetPlain<Vertex>()),
          pipeline(gfx)
    {

        itList.vertices[0].color = Vec3(Colors::Red);
        itList.vertices[1].color = Vec3(Colors::Green);
        itList.vertices[2].color = Vec3(Colors::Blue);
        itList.vertices[3].color = Vec3(Colors::Yellow);
        itList.vertices[4].color = Vec3(Colors::Cyan);
        itList.vertices[5].color = Vec3(Colors::Magenta);
        itList.vertices[6].color = Vec3(Colors::White);
        itList.vertices[7].color = Vec3(Colors::Black);
    }

    void Update(const Widgets::Controller& controller, float deltaTime) override
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

        pipeline.BindRotation(rot);
        pipeline.BindTranslation(trans);

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

#endif //CHILI_RENDERER_GL_CUBEVERTEXCOLORSCENE_H
