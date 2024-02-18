#pragma once
#include "RenderContext.h"

class OpenGL_Context : public Render::RenderContext {
  public:
   bool init(IWindow* window) override;
   void preRender() override;
   void postRender() override;
   void end() override;
};