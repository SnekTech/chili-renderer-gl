//
// Created by syf on 2023/3/10.
//

#include <iostream>
#include <cmath>

#include "Game.h"
#include "DataStructures/Mat.h"


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
    auto leftAxis = controller.LeftAxis();
    leftAxis *= speed;
    theta_x += leftAxis.y;
    theta_y += leftAxis.x;

    using Button = Widgets::Controller::Button;
    if (controller.IsPressed(Button::A))
    {
        using std::lerp;
        theta_x = lerp(theta_x, 0, 0.3f);
        theta_y = lerp(theta_y, 0, 0.3f);
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
		v += {0, 0, 1};
		pst.Transform(v);
	}
	for (auto i = lines.indices.cbegin(); i != lines.indices.cend(); std::advance(i, 2))
	{
		gfx.DrawLine(lines.vertices[*i], lines.vertices[*std::next(i)], Colors::White);
	}
}