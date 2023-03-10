//
// Created by syf on 2023/3/10.
//

#include "Game.h"

Game::Game(GLFWwindow* window)
	: window(window)
{
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
	gfx.PutPixel(200, 200, Colors::Yellow);
	gfx.DrawLine(100, 100, 200, 300, Colors::Yellow);
}