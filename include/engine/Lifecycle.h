#pragma once

class Lifecycle
{
public:
    virtual ~Lifecycle() {}

    virtual void Start() {}
    virtual void Update(float dt) {}
    virtual void FixedUpdate(float dt) {}
    virtual void LateUpdate(float dt) {}
    virtual void Draw(unsigned int shader) {}
};
