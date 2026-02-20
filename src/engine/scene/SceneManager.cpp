#include "engine/scene/SceneManager.h"

std::unique_ptr<Scene> SceneManager::currentScene = nullptr;

void SceneManager::LoadScene(std::unique_ptr<Scene> newScene)
{
    currentScene = std::move(newScene);
    if (currentScene)
        currentScene->Start();
}

Scene *SceneManager::GetCurrent()
{
    return currentScene.get();
}
