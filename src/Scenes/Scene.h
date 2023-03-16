//
// Created by syf on 2023/3/16.
//

#ifndef CHILI_RENDERER_GL_SCENE_H
#define CHILI_RENDERER_GL_SCENE_H

#include "HardwareRender/Graphics.h"
#include "Widgets/Controller.h"

class Scene
{
public:
    explicit Scene(const Widgets::Controller& controller) : controller(controller) {}
    virtual void Update(float deltaTime) = 0;
    virtual void Draw(Graphics& gfx) const = 0;
    virtual ~Scene() = default;
protected:
    const Widgets::Controller& controller;
};

#endif //CHILI_RENDERER_GL_SCENE_H
