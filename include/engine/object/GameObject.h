#pragma once
#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include "engine/components/Component.h"
#include "engine/object/Transform.h"

class GameObject : public Lifecycle
{
public:
    Transform Transform;

    template <typename T, typename... Args>
    T *AddComponent(Args &&...args)
    {
        auto comp = std::make_unique<T>(std::forward<Args>(args)...);
        comp->Owner = this;
        T *rawPtr = comp.get();
        components.push_back(std::move(comp));
        return rawPtr;
    }

    void Start() override
    {
        for (auto &c : components)
            c->Start();
    }

    void Update(float dt) override
    {
        for (auto &c : components)
            c->Update(dt);
    }

    void FixedUpdate(float dt) override
    {
        for (auto &c : components)
            c->FixedUpdate(dt);
    }

    void LateUpdate(float dt) override
    {
        for (auto &c : components)
            c->LateUpdate(dt);
    }

    void Draw(unsigned int shader) override
    {
        for (auto &c : components)
            c->Draw(shader);
    }

    std::vector<Component *> GetComponents()
    {
        std::vector<Component *> raw;
        for (auto &c : components)
            raw.push_back(c.get());
        return raw;
    }

    template <typename T>
    T *GetComponent()
    {
        for (auto &c : components)
        {
            // Try to cast the base Component to the requested type
            T *casted = dynamic_cast<T *>(c.get());
            if (casted)
                return casted;
        }
        return nullptr; // not found
    }

private:
    std::vector<std::unique_ptr<Component>> components;
};
