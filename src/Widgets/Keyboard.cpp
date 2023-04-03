//
// Created by syf on 2023/4/3.
//
#include <array>
#include <cassert>
#include "Keyboard.h"

constexpr int KeyCount = 26;
static std::array<bool, KeyCount> keys;

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    int index = key - 65;
    assert(index >= 0 & index < KeyCount);
    keys[index] = true;
}

Widgets::Keyboard::Keyboard(GLFWwindow* window)
{
    glfwSetKeyCallback(window, key_callback);
}

void Widgets::Keyboard::Update()
{
    for (bool& isPressed : keys)
        isPressed = false;
}

bool Widgets::Keyboard::KeyIsPressed(int key) const
{
    int index = key - 65;
    assert(index >= 0 & index < KeyCount);
    return keys[index];
}
