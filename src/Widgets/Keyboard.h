//
// Created by syf on 2023/4/3.
//

#ifndef CHILI_RENDERER_GL_KEYBOARD_H
#define CHILI_RENDERER_GL_KEYBOARD_H
#include <GLFW/glfw3.h>

namespace Widgets
{
    class Keyboard
    {
    public:
        explicit Keyboard(GLFWwindow* window);
        Keyboard(const Keyboard& other) = delete;
        Keyboard& operator=(const Keyboard& other) = delete;
        void Update();
        [[nodiscard]] bool KeyIsPressed(int key) const;
    };
}

#endif //CHILI_RENDERER_GL_KEYBOARD_H
