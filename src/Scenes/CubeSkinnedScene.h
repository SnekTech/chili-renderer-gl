//
// Created by syf on 2023/3/19.
//

#ifndef CHILI_RENDERER_GL_CUBESKINNEDSCENE_H
#define CHILI_RENDERER_GL_CUBESKINNEDSCENE_H

#include "Scene.h"
#include "Shapes/CubeSkinned.h"
#include "PreClipScreenTransformer.h"
#include "DataStructures/Mat.h"

class CubeSkinnedScene : public Scene
{
public:
    explicit CubeSkinnedScene(const std::string& filename)
        : sbTex(Surface::FromFile(filename))
    {}
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
        Mat3 rot = Mat3::RotationX(theta_x) *
                   Mat3::RotationY(theta_y) *
                   Mat3::RotationZ(theta_z);

        auto triangles = cube.GetTrianglesTex();

        // model space --> world(/view) space
        for (auto& v: triangles.vertices)
        {
            v.pos *= rot;
            v.pos += { 0, 0, offset_z };
        }

        const auto& vertices = triangles.vertices;
        const auto& indices = triangles.indices;

        // backface culling
        for (int i = 0; i < indices.size() / 3; i++)
        {
            const auto& v0 = vertices[indices[i * 3]].pos;
            const auto& v1 = vertices[indices[i * 3 + 1]].pos;
            const auto& v2 = vertices[indices[i * 3 + 2]].pos;

            triangles.cullFlags[i] = (v1 - v0).Cross(v2 - v0) * v0 > 0.0f;
        }

        // world space --> screen space
        for (auto& v: triangles.vertices)
        {
            pst.Transform(v.pos);
        }

        for (int i = 0; i < indices.size() / 3; i++)
        {
            // skip culled triangle
            if (triangles.cullFlags[i]) continue;

            const auto& v0 = vertices[indices[i * 3]];
            const auto& v1 = vertices[indices[i * 3 + 1]];
            const auto& v2 = vertices[indices[i * 3 + 2]];

            gfx.DrawTriangleTex(v0, v1, v2, sbTex);
        }
    }

private:
    PreClipScreenTransformer pst;
    const float LerpAlpha = 0.5;
    CubeSkinned cube = CubeSkinned(1.0f);
    Surface sbTex;
    float theta_x = 0, theta_y = 0, theta_z = 0;
    static constexpr float dTheta = PI;

    float offset_z = 2;
};

#endif //CHILI_RENDERER_GL_CUBESKINNEDSCENE_H
