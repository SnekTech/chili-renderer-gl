//
// Created by syf on 2023/3/10.
//

#include <iostream>

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
    Mat3 rot = Mat3::RotationX(theta_x)
        * Mat3::RotationY(theta_y);

	auto lines = cube.GetLines();

	for (auto& v : lines.vertices)
	{
        v *= rot;
		v += {0, 0, offset_z};
		pst.Transform(v);
	}
	for (auto i = lines.indices.cbegin(); i != lines.indices.cend(); std::advance(i, 2))
	{
		gfx.DrawLine(lines.vertices[*i], lines.vertices[*std::next(i)], Colors::White);
	}
}