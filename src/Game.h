//
// Created by syf on 2023/3/10.
//

#ifndef CHILI_RENDERER_GL_GAME_H
#define CHILI_RENDERER_GL_GAME_H

#include <vector>
#include <memory>

#include "Widgets/FrameTimer.h"
#include "Widgets/Controller.h"
#include "Widgets/Keyboard.h"
#include "Render/Graphics.h"
#include "PreClipScreenTransformer.h"
#include "Shapes/Cube.h"
#include "Scenes/Scene.h"

class Game
{
public:
	explicit Game(GLFWwindow * window);
    Game(const Game& ) = delete;
    Game& operator=(const Game&) = delete;
    void Go();
private:
    void ComposeFrame();
    void UpdateModel();

    // user functions live here

    void CycleScenes(bool backwards = true);

private:
	Graphics gfx;
    Widgets::Controller controller;
    Widgets::Keyboard keyboard;
    FrameTimer frameTimer;
    bool isSwitchingScene = false;

	// user variables live here
    std::vector<std::unique_ptr<Scene>> scenes;
    std::vector<std::unique_ptr<Scene>>::iterator currentScene;
};

#endif //CHILI_RENDERER_GL_GAME_H
