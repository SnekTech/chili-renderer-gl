//
// Created by syf on 2023/3/10.
//

#include "Game.h"
#include "Widgets/ChiliMath.h"
#include "Scenes/SolidCubeScene.h"

using Button = Widgets::Controller::Button;

Game::Game() : controller(GLFW_JOYSTICK_1)
{
    scenes.push_back(std::make_unique<SolidCubeScene>(controller));

    currentScene = scenes.begin();
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
    const float dt = 1.0f / 60.0f;
    controller.UpdateState();

    if (controller.IsPressed(Button::Right))
    {
        CycleScenes();
    }
    else if (controller.IsPressed(Button::Left))
    {
        CycleScenes(true);
    }

    (*currentScene)->Update(dt);
}

void Game::ComposeFrame()
{
    (*currentScene)->Draw(gfx);
}

void Game::CycleScenes(bool backwards)
{
    if (!backwards)
    {
        currentScene++;
        if (currentScene == scenes.end())
        {
            currentScene = scenes.begin();
        }
        return;
    }

    if (currentScene == scenes.begin())
    {
        currentScene = scenes.end() - 1;
        return;
    }
    currentScene--;
}
