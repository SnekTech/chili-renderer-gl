//
// Created by syf on 2023/3/16.
//

#ifndef CHILI_RENDERER_GL_CUBEORDERSCENE_H
#define CHILI_RENDERER_GL_CUBEORDERSCENE_H

#include "Scene.h"
#include "PreClipScreenTransformer.h"
#include "Shapes/Cube.h"
#include "Widgets/Controller.h"
#include "DataStructures/Mat.h"

class CubeOrderScene : public Scene
{
public:
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

    void Draw(Graphics& gfx) const override
    {
        //////////////////////////////////////////////////////
        //      FIXED CUBE
        {
            // generate rotation matrix from euler angles
            const Mat3 rot =
                Mat3::RotationX(theta_x) *
                Mat3::RotationY(theta_y + PI) *
                Mat3::RotationZ(theta_z);
            // generate indexed triangle list
            auto triangles = cube.GetTriangles();
            // transform from model space -> world (/view) space
            for (auto& v: triangles.vertices)
            {
                v *= rot;
                v += { 0.0f, 0.0f, 2.0f };
            }
            // backface culling test (must be done in world (/view) space)
            for (size_t i = 0,
                     end = triangles.indices.size() / 3;
                 i < end; i++)
            {
                const Vec3& v0 = triangles.vertices[triangles.indices[i * 3]];
                const Vec3& v1 = triangles.vertices[triangles.indices[i * 3 + 1]];
                const Vec3& v2 = triangles.vertices[triangles.indices[i * 3 + 2]];
                triangles.cullFlags[i] = (v1 - v0).Cross(v2 - v0) * v0 > 0.0f;
            }
            // transform to screen space (includes perspective transform)
            for (auto& v: triangles.vertices)
            {
                pst.Transform(v);
            }
            // draw the mf triangles!
            for (size_t i = 0,
                     end = triangles.indices.size() / 3;
                 i < end; i++)
            {
                // skip triangles previously determined to be back-facing
                if (!triangles.cullFlags[i])
                {
                    gfx.DrawTriangle(
                        triangles.vertices[triangles.indices[i * 3]],
                        triangles.vertices[triangles.indices[i * 3 + 1]],
                        triangles.vertices[triangles.indices[i * 3 + 2]],
                        colors[i]);
                }
            }
        }

        //////////////////////////////////////////////////////
        //      MOBILE CUBE
        {
            // generate rotation matrix from euler angles
            const Mat3 rot =
                Mat3::RotationX(theta_x) *
                Mat3::RotationY(theta_y) *
                Mat3::RotationZ(theta_z);
            // generate indexed triangle list
            auto triangles = cube.GetTriangles();
            // transform from model space -> world (/view) space
            for (auto& v: triangles.vertices)
            {
                v *= rot;
                v += { 0.0f, 0.0f, offset_z };
            }
            // backface culling test (must be done in world (/view) space)
            for (size_t i = 0,
                     end = triangles.indices.size() / 3;
                 i < end; i++)
            {
                const Vec3& v0 = triangles.vertices[triangles.indices[i * 3]];
                const Vec3& v1 = triangles.vertices[triangles.indices[i * 3 + 1]];
                const Vec3& v2 = triangles.vertices[triangles.indices[i * 3 + 2]];
                triangles.cullFlags[i] = (v1 - v0).Cross(v2 - v0) * v0 > 0.0f;
            }
            // transform to screen space (includes perspective transform)
            for (auto& v: triangles.vertices)
            {
                pst.Transform(v);
            }
            // draw the mf triangles!
            for (size_t i = 0,
                     end = triangles.indices.size() / 3;
                 i < end; i++)
            {
                // skip triangles previously determined to be back-facing
                if (!triangles.cullFlags[i])
                {
                    gfx.DrawTriangle(
                        triangles.vertices[triangles.indices[i * 3]],
                        triangles.vertices[triangles.indices[i * 3 + 1]],
                        triangles.vertices[triangles.indices[i * 3 + 2]],
                        colors[i]);
                }
            }
        }
    }

private:
    PreClipScreenTransformer pst;
    const Color colors[12] = {
        Colors::White,
        Colors::Blue,
        Colors::Cyan,
        Colors::Gray,
        Colors::Green,
        Colors::Magenta,
        Colors::LightGray,
        Colors::Red,
        Colors::Yellow,
        Colors::White,
        Colors::Blue,
        Colors::Cyan
    };
    const float LerpAlpha = 0.5;
    Cube cube = Cube(1.0f);
    float theta_x = 0, theta_y = 0, theta_z = 0;
    static constexpr float dTheta = PI;

    float offset_z = 2;
};

#endif //CHILI_RENDERER_GL_CUBEORDERSCENE_H