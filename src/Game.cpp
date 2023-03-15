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

    Mat3 rot = Mat3::RotationX(theta_x)
               * Mat3::RotationY(theta_y);

    auto triangles = cube.GetTriangles();

    for (auto& v: triangles.vertices)
    {
        v *= rot;
        v += { 0, 0, offset_z };
        pst.Transform(v);
    }
    for (auto i = triangles.indices.cbegin(); i != triangles.indices.cend(); std::advance(i, 3))
    {
        using std::next;

        Vec2 v0 = triangles.vertices[*i];
        Vec2 v1 = triangles.vertices[*next(i)];
        Vec2 v2 = triangles.vertices[*next(i, 2)];

        unsigned int colorIndex = std::distance(triangles.indices.cbegin(), i) / 3;
        gfx.DrawTriangle(v0, v1, v2, colors[colorIndex]);
    }
}