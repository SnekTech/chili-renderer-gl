//
// Created by syf on 2023/3/11.
//

#include "Controller.h"

#include <map>

using Button = Widgets::Controller::Button;
static const std::map<Button, uint8_t> buttonToGlfwId = {
    { Button::A,           GLFW_GAMEPAD_BUTTON_A },
    { Button::B,           GLFW_GAMEPAD_BUTTON_B },
    { Button::X,           GLFW_GAMEPAD_BUTTON_X },
    { Button::Y,           GLFW_GAMEPAD_BUTTON_Y },
    { Button::Up,          GLFW_GAMEPAD_BUTTON_DPAD_UP },
    { Button::Down,        GLFW_GAMEPAD_BUTTON_DPAD_DOWN },
    { Button::Left,        GLFW_GAMEPAD_BUTTON_DPAD_LEFT },
    { Button::Right,       GLFW_GAMEPAD_BUTTON_DPAD_RIGHT },
    { Button::LeftBumper,  GLFW_GAMEPAD_BUTTON_LEFT_BUMPER },
    { Button::RightBumper, GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER },
    { Button::View,        GLFW_GAMEPAD_BUTTON_GUIDE },
    { Button::Menu,        GLFW_GAMEPAD_BUTTON_START },
    { Button::LeftThumb,   GLFW_GAMEPAD_BUTTON_LEFT_THUMB },
    { Button::RightThumb,  GLFW_GAMEPAD_BUTTON_RIGHT_THUMB },
};

bool Widgets::Controller::IsPressed(Widgets::Controller::Button button) const
{
    if (button == Any)
    {
        for (unsigned char btn : state.buttons) {
            if (btn == GLFW_PRESS)
            {
                return true;
            }
        }

        return false;
    }

    auto index = buttonToGlfwId.at(button);
    return state.buttons[index] == GLFW_PRESS;
}

Vec2 Widgets::Controller::LeftAxis() const
{
    const auto& axes = state.axes;
    return -Vec2(axes[GLFW_GAMEPAD_AXIS_LEFT_X], axes[GLFW_GAMEPAD_AXIS_LEFT_Y]);
}

Vec2 Widgets::Controller::RightAxis() const
{
    const auto& axes = state.axes;
    return -Vec2(axes[GLFW_GAMEPAD_AXIS_RIGHT_X], axes[GLFW_GAMEPAD_AXIS_RIGHT_Y]);
}

bool Widgets::Controller::IsReleased(Button button) const
{
    auto index = buttonToGlfwId.at(button);
    return state.buttons[index] == GLFW_RELEASE;
}
