#pragma once
#include <vector>
#include <memory>
#include "engine/object/GameObject.h"
#include "engine/Lifecycle.h"
#include <GLFW/glfw3.h>

class Scene : public Lifecycle
{
public:
    std::shared_ptr<GameObject> CreateObject()
    {
        auto obj = std::make_shared<GameObject>();
        objects.push_back(obj);
        return obj;
    }

    void Start() override
    {
        for (auto &obj : objects)
            obj->Start();
    }

    void Update(float dt) override
    {
        for (auto &obj : objects)
            obj->Update(dt);
    }

    void FixedUpdate(float dt) override
    {
        for (auto &obj : objects)
            obj->FixedUpdate(dt);
    }

    void LateUpdate(float dt) override
    {
        for (auto &obj : objects)
            obj->LateUpdate(dt);
    }

    void Draw(unsigned int shader) override
    {
        for (auto &obj : objects)
            obj->Draw(shader);
    }

    const std::vector<std::shared_ptr<GameObject>> &GetObjects() const
    {
        return objects;
    }

    virtual void HandleInput(GLFWwindow *window) {}

private:
    std::vector<std::shared_ptr<GameObject>> objects;
};
