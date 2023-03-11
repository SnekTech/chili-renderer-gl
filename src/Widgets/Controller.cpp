//
// Created by syf on 2023/3/11.
//

#include "Controller.h"

#include <map>
using Button = Widgets::Controller::Button;
static std::map<Button, uint8_t> buttonToGlfwId = {
    {Button::A, GLFW_GAMEPAD_BUTTON_A},
    {Button::B, GLFW_GAMEPAD_BUTTON_B},
    {Button::X, GLFW_GAMEPAD_BUTTON_X},
    {Button::Y, GLFW_GAMEPAD_BUTTON_Y},
};

bool Widgets::Controller::IsPressed(Widgets::Controller::Button button) const
{
    auto index = buttonToGlfwId[button];
    return state.buttons[index] == GLFW_PRESS;
}
