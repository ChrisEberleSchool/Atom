#pragma once
#include "engine/core/Lifecycle.h"

class GameObject;

class Component : public Lifecycle {
 public:
  GameObject* Owner = nullptr;
};
