//
// Created by syf on 2023/3/10.
//

#include "Game.h"
#include "DataStructures/Mat.h"

#define Testing 0
#if Testing
#include "Widgets/CppFeatureTester.h"
#endif

Game::Game()
    : cube(1)
{
#if Testing
	Widgets::CppFeatureTester tester;
#endif
}

void Game::Go()
{
	gfx.BeginFrame();
	UpdateModel();
	ComposeFrame();
	gfx.EndFrame();
}

void Game::UpdateModel()
{
    if (glfwGetGamepadState(GLFW_JOYSTICK_1, &controller))
    {
        float xAxis = controller.axes[GLFW_GAMEPAD_AXIS_LEFT_X];
        float yAxis = controller.axes[GLFW_GAMEPAD_AXIS_LEFT_Y];
        theta_x += yAxis * speed;
        theta_y += xAxis * speed;
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