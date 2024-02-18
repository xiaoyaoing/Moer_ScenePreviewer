#pragma once

#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <imgui_internal.h>

#include "RenderContext.h"

class ImGui_Context : public Render::RenderContext {
  public:
   bool init(IWindow* window) override;
   void preRender() override;
   void postRender() override;
   void end() override;
};