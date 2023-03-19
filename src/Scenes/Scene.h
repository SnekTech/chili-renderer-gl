//
// Created by syf on 2023/3/16.
//

#ifndef CHILI_RENDERER_GL_SCENE_H
#define CHILI_RENDERER_GL_SCENE_H

#include "Render/Graphics.h"
#include "Widgets/Controller.h"

class Scene
{
public:
    virtual void Update(const Widgets::Controller& controller, float deltaTime) = 0;
    virtual void Draw() = 0;
    virtual ~Scene() = default;
};

#endif //CHILI_RENDERER_GL_SCENE_H
