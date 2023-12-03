#pragma once
#include "Window.h"

namespace Render {
class RenderContext {
  public:
   RenderContext() : window(nullptr) {}

   virtual bool init(IWindow* window) {
      this->window = window;
      return true;
   }
   virtual void preRender() = 0;
   virtual void postRender() = 0;
   virtual void end() = 0;

  protected:
   IWindow* window;
};
};  // namespace Render