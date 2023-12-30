#pragma once

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <iostream>

#include "FiraCode.h"
#include "RenderContext.h"


class ImGui_Context : public Render::RenderContext {
  public:
   bool init(IWindow* window) override;
   void preRender() override;
   void postRender() override;
   void end() override;
};