//
// Created by syf on 2023/3/10.
//

#include "Game.h"

#define Testing 0
#if Testing
#include "Widgets/CppFeatureTester.h"
#endif

Game::Game()
	: cube(1.0f)
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

}

void Game::ComposeFrame()
{
	auto lines = cube.GetLines();

	for (auto& v : lines.vertices)
	{
		v += {0, 0, 1};
		pst.Transform(v);
	}
	for (auto i = lines.indices.cbegin(); i != lines.indices.cend(); std::advance(i, 2))
	{
		gfx.DrawLine(lines.vertices[*i], lines.vertices[*std::next(i)], Colors::White);
	}
}