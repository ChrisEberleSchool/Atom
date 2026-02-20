#pragma once
#include <memory>
#include "engine/scene/Scene.h"

class SceneManager
{
public:
    static void LoadScene(std::unique_ptr<Scene> newScene);
    static Scene *GetCurrent();

private:
    static std::unique_ptr<Scene> currentScene;
};
