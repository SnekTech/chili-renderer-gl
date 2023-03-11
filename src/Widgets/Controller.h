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
        };
    public:
        explicit Controller(int joystickId)
            :
            id(joystickId),
            name(glfwGetGamepadName(joystickId))
        {
            std::cout << "Controller discovered: " << name << std::endl;
        }
        Controller(const Controller& rhs) = delete;
        Controller& operator=(const Controller& rhs) = delete;

        void UpdateState()
        {
            glfwGetGamepadState(id, &state);
        }

        [[nodiscard]] bool IsPressed(Button button) const;

        [[nodiscard]]
        Vec2 LeftAxis() const
        {
            const auto& axes = state.axes;
            return {axes[GLFW_GAMEPAD_AXIS_LEFT_X], axes[GLFW_GAMEPAD_AXIS_LEFT_Y]};
        }
    public:
        int id;
        GLFWgamepadstate state{};
        std::string name;
    };
}

#endif //CHILI_RENDERER_GL_CONTROLLER_H
