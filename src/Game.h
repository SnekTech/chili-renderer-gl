//
// Created by syf on 2023/3/10.
//

#ifndef CHILI_RENDERER_GL_GAME_H
#define CHILI_RENDERER_GL_GAME_H

#include <GLFW/glfw3.h>

#include "FrameTimer.h"
#include "Graphics.h"

class Game
{
public:
	Game(GLFWwindow* window);
    Game(const Game& ) = delete;
    Game& operator=(const Game&) = delete;
    void Go();
private:
    void ComposeFrame();
    void UpdateModel();

    // user functions live here

private:
	GLFWwindow* window;
	Graphics gfx;
    FrameTimer frameTimer;
};

#endif //CHILI_RENDERER_GL_GAME_H
