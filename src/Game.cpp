//
// Created by syf on 2023/3/10.
//

#include "Game.h"
#include "DataStructures/Mat.h"
#include "Widgets/ChiliMath.h"

using Button = Widgets::Controller::Button;

Game::Game()
    : controller(GLFW_JOYSTICK_1), cube(1)
{

}

void Game::Go()
{
    gfx.BeginFrame();

    controller.UpdateState();
    UpdateModel();
    ComposeFrame();
    gfx.EndFrame();
}

void Game::UpdateModel()
{
    // rotation
    auto rotation = controller.RightAxis() * deltaTime;
    theta_x = wrap_angle(theta_x + rotation.y);
    theta_y = wrap_angle(theta_y + rotation.x);

    // movement
    auto movement = controller.LeftAxis() * deltaTime;
    offset_z += movement.y;

    // reset
    if (controller.IsPressed(Button::BumperRight))
    {
        theta_x = lerp(theta_x, 0.0f, LerpAlpha);
        theta_y = lerp(theta_y, 0.0f, LerpAlpha);
        offset_z = lerp(offset_z, 2.0f, LerpAlpha);
    }
}

void Game::ComposeFrame()
{
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

    Mat3 rot = Mat3::RotationX(theta_x) *
               Mat3::RotationY(theta_y);

    auto triangles = cube.GetTriangles();

    // model space --> world(/view) space
    for (auto& v: triangles.vertices)
    {
        v *= rot;
        v += { 0, 0, offset_z };
    }

    const auto& vertices = triangles.vertices;
    const auto& indices = triangles.indices;

    // backface culling
    for (int i = 0; i < triangles.indices.size(); i += 3)
    {
        const auto v0 = vertices[indices[i]];
        const auto v1 = vertices[indices[i + 1]];
        const auto v2 = vertices[indices[i + 2]];

        triangles.cullFlags[i / 3] = (v1 - v0).Cross(v2 - v0) * v0 >= 0.0f;
    }

    // world space --> screen space
    for (auto& v: triangles.vertices)
    {
        pst.Transform(v);
    }

    for (int i = 0; i < indices.size(); i += 3)
    {
        int triangleIndex = i / 3;
        // skip culled triangle
        if (triangles.cullFlags[triangleIndex]) continue;

        const auto v0 = vertices[indices[i]];
        const auto v1 = vertices[indices[i + 1]];
        const auto v2 = vertices[indices[i + 2]];

        gfx.DrawTriangle(v0, v1, v2, colors[triangleIndex]);
    }
}