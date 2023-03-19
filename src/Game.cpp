//
// Created by syf on 2023/3/10.
//

#include "Game.h"
#include "Widgets/ChiliMath.h"
#include "Scenes/CubeSkinnedScene.h"

using Button = Widgets::Controller::Button;


Game::Game() : controller(GLFW_JOYSTICK_1)
{
    scenes.push_back(std::make_unique<CubeSkinnedScene>(gfx, "../images/dice_skin.png"));

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
    const float dt = frameTimer.Mark();

    controller.UpdateState();

    if (!isSwitchingScene)
    {
        if (controller.IsPressed(Button::Right))
        {
            isSwitchingScene = true;
            CycleScenes();
        }
    }
    if (controller.IsReleased(Button::Right))
    {
        isSwitchingScene = false;
    }

    (*currentScene)->Update(controller, dt);
}

void Game::ComposeFrame()
{
    (*currentScene)->Draw();
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
