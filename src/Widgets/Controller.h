//
// Created by syf on 2023/3/11.
//

#ifndef CHILI_RENDERER_GL_CONTROLLER_H
#define CHILI_RENDERER_GL_CONTROLLER_H

#include <GLFW/glfw3.h>
#include <string>
#include <iostream>

#include "DataStructures/Vec2.h"

namespace Widgets
{
    class Controller
    {
    public:
        enum Button
        {
            A,
            B,
            X,
            Y,
            Up,
            Down,
            Left,
            Right,
            RightBumper,
            LeftBumper,
            View,
            Menu,
            LeftThumb,
            RightThumb,
            Any
        };
    public:
        explicit Controller(int joystickId)
            :
            id(joystickId)
        {
            auto gamepadName = glfwGetGamepadName(joystickId);
            if (gamepadName)
            {
                name = gamepadName;
                std::cout << "Controller discovered: " << name << std::endl;
            }
            else
            {
                std::cerr << "Controller not found " << name << std::endl;
            }

        }
        Controller(const Controller& rhs) = delete;
        Controller& operator=(const Controller& rhs) = delete;

        void UpdateState()
        {
            glfwGetGamepadState(id, &state);
        }
        void ClearState()
        {
            state = GLFWgamepadstate{};
        }

        [[nodiscard]] bool IsPressed(Button button) const;
        [[nodiscard]] bool IsReleased(Button button) const;
        [[nodiscard]] Vec2 LeftAxis() const;
        [[nodiscard]] Vec2 RightAxis() const;
    public:
        int id;
        GLFWgamepadstate state{};
        std::string name;
    };
}

#endif //CHILI_RENDERER_GL_CONTROLLER_H
