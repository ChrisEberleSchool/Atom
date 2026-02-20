#include "Application.h"
#include "scenes/ExampleScene.h"
#include <iostream>

int main()
{
  Application app;
  // Create ExampleScene and set it active
  auto scene = std::make_unique<ExampleScene>();
  app.SetActiveScene(std::move(scene));
  app.Run();
  return 0;
}
