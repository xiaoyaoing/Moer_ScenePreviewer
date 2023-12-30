#pragma once
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <memory>

#include "Scene.h"

class ImGuiManager {
  public:
   void render(std::shared_ptr<Scene> scene);
};