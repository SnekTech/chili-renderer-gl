//
// Created by syf on 2023/3/10.
//

#ifndef CHILI_RENDERER_GL_GAME_H
#define CHILI_RENDERER_GL_GAME_H

#include "Widgets/FrameTimer.h"
#include "Widgets/Controller.h"
#include "HardwareRender/Graphics.h"
#include "PreClipScreenTransformer.h"
#include "Shapes/Cube.h"

class Game
{
public:
	Game();
    Game(const Game& ) = delete;
    Game& operator=(const Game&) = delete;
    void Go();
private:
    void ComposeFrame();
    void UpdateModel();

    // user functions live here

private:
	Graphics gfx;
    FrameTimer frameTimer;

	// user variables live here
	PreClipScreenTransformer pst;
    Widgets::Controller controller;
    Cube cube;
    float theta_x = 0, theta_y = 0;
    const float speed = 1 / 60.0f;
};

#endif //CHILI_RENDERER_GL_GAME_H
