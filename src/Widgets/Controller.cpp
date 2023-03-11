//
// Created by syf on 2023/3/11.
//

#include "Controller.h"

#include <map>
using Button = Widgets::Controller::Button;
static const std::map<Button, uint8_t> buttonToGlfwId = {
    {Button::A,           GLFW_GAMEPAD_BUTTON_A},
    {Button::B,           GLFW_GAMEPAD_BUTTON_B},
    {Button::X,           GLFW_GAMEPAD_BUTTON_X},
    {Button::Y,           GLFW_GAMEPAD_BUTTON_Y},
    {Button::Up,          GLFW_GAMEPAD_BUTTON_DPAD_UP},
    {Button::Down,        GLFW_GAMEPAD_BUTTON_DPAD_DOWN},
    {Button::Left,        GLFW_GAMEPAD_BUTTON_DPAD_LEFT},
    {Button::Right,       GLFW_GAMEPAD_BUTTON_DPAD_RIGHT},
    {Button::BumperLeft,  GLFW_GAMEPAD_BUTTON_LEFT_BUMPER},
    {Button::BumperRight, GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER},
};

bool Widgets::Controller::IsPressed(Widgets::Controller::Button button) const
{
    auto index = buttonToGlfwId.at(button);
    return state.buttons[index] == GLFW_PRESS;
}

Vec2 Widgets::Controller::LeftAxis() const
{
    const auto& axes = state.axes;
    return {axes[GLFW_GAMEPAD_AXIS_LEFT_X], axes[GLFW_GAMEPAD_AXIS_LEFT_Y]};
}

Vec2 Widgets::Controller::RightAxis() const
{
    const auto& axes = state.axes;
    return {axes[GLFW_GAMEPAD_AXIS_RIGHT_X], axes[GLFW_GAMEPAD_AXIS_RIGHT_Y]};
}
