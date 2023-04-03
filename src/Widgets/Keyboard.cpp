//
// Created by syf on 2023/4/3.
//
#include <array>
#include <iostream>
#include "Keyboard.h"

constexpr int KeyCount = 26;
static std::array<bool, KeyCount> keys;

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    int index = key - 65;
    if(index < 0 || index >= KeyCount)
        return;

    keys[index] = action != GLFW_RELEASE;
}

Widgets::Keyboard::Keyboard(GLFWwindow* window)
{
    glfwSetKeyCallback(window, key_callback);
}


bool Widgets::Keyboard::KeyIsPressed(int key) const
{
    int offset = std::islower(key) ? 97 : 65;
    int index = key - offset;
    if (index < 0 || index >= KeyCount)
        return false;

    return keys[index];
}
