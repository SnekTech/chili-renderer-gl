//
// Created by syf on 2023/3/10.
//

#include "Game.h"
#include "Widgets/ChiliMath.h"
#include "Shapes/Sphere.h"
#include "Shapes/TestTriangle.h"
//#include "Scenes/CubeSkinnedScene.h"
//#include "Scenes/CubeVertexColorScene.h"
//#include "Scenes/CubeSolidScene.h"
//#include "Scenes/DoubleCubeScene.h"
//#include "Scenes/VertexWaveScene.h"
//#include "Scenes/CubeVertexPositionColorScene.h"
//#include "Scenes/CubeSolidGeometryScene.h"
//#include "Scenes/CubeFlagIndependentScene.h"
//#include "Scenes/GeometryFlatScene.h"
//#include "Scenes/GouraudScene.h"
//#include "Scenes/GouraudPointScene.h"
//#include "Scenes/PhongPointScene.h"
#include "Scenes/SpecularPhongPointScene.h"

using Button = Widgets::Controller::Button;


Game::Game(GLFWwindow* window) : controller(GLFW_JOYSTICK_1), keyboard(window)
{
//    scenes.push_back(std::make_unique<SpecularPhongPointScene>(gfx, TestTriangle::GetNormals<SpecularPhongPointEffect::Vertex>()));
    scenes.push_back(std::make_unique<SpecularPhongPointScene>(gfx, IndexedTriangleList<SpecularPhongPointEffect::Vertex>::LoadNormals("../models/suzanne.obj")));
//    scenes.push_back(std::make_unique<PhongPointScene>(gfx, IndexedTriangleList<PhongPointScene::Vertex>::LoadNormals("../models/suzanne.obj")));
//    scenes.push_back(std::make_unique<PhongPointScene>(gfx, Plane::GetNormals<PhongPointScene::Vertex>(1)));
//    scenes.push_back(std::make_unique<GouraudPointScene>(gfx, IndexedTriangleList<GouraudPointScene::Vertex>::LoadNormals("../models/suzanne.obj")));
//    scenes.push_back(std::make_unique<GouraudScene>(gfx, IndexedTriangleList<GouraudEffect::Vertex>::LoadNormals("../models/suzanne.obj")));
//    scenes.push_back(std::make_unique<GouraudScene>(gfx, Sphere::GetPlainNormals<GouraudScene::Vertex>()));
//    scenes.push_back(std::make_unique<GeometryFlatScene>(gfx, Sphere::GetPlain<GeometryFlatScene::Vertex>()));
//    scenes.push_back(std::make_unique<VertexWaveScene>(gfx));
//    scenes.push_back(std::make_unique<GeometryFlatScene>(gfx, Cube::GetPlain<GeometryFlatScene::Vertex>()));
//    scenes.push_back(std::make_unique<GeometryFlatScene>(gfx, IndexedTriangleList<GeometryFlatScene::Vertex>::Load("../models/bunny.obj")));
//    scenes.push_back(std::make_unique<CubeFlatIndependentScene>(gfx));
//    scenes.push_back(std::make_unique<CubeSolidGeometryScene>(gfx));
//    scenes.push_back(std::make_unique<CubeVertexPositionColorScene>(gfx));
//    scenes.push_back(std::make_unique<CubeSkinnedScene>(gfx, "../images/dice_skin.png"));
//    scenes.push_back(std::make_unique<CubeVertexColorScene>(gfx));
//    scenes.push_back(std::make_unique<CubeSolidScene>(gfx));
//    scenes.push_back(std::make_unique<DoubleCubeScene>(gfx));

    currentScene = scenes.begin();
}

void Game::Go()
{
    gfx.BeginFrame();
    UpdateModel();
    ComposeFrame();
    gfx.EndFrame();
}

void Game::UpdateModel()
{
    const float dt = frameTimer.Mark();

    controller.UpdateState();

    if (!isSwitchingScene)
    {
        if (controller.IsPressed(Button::Right))
        {
            isSwitchingScene = true;
            CycleScenes();
        }
    }
    if (controller.IsReleased(Button::Right))
    {
        isSwitchingScene = false;
    }

    (*currentScene)->Update(controller, keyboard, dt);
}

void Game::ComposeFrame()
{
    (*currentScene)->Draw();
}

void Game::CycleScenes(bool backwards)
{
    if (!backwards)
    {
        currentScene++;
        if (currentScene == scenes.end())
        {
            currentScene = scenes.begin();
        }
        return;
    }

    if (currentScene == scenes.begin())
    {
        currentScene = scenes.end() - 1;
        return;
    }
    currentScene--;
}
