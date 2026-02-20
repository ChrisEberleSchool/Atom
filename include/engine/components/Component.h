#pragma once
#include "engine/Lifecycle.h"

class GameObject;

class Component : public Lifecycle
{
public:
    GameObject *Owner = nullptr;
};
