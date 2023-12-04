#pragma once
#include <memory>

#include "OpenGLContext.h"
#include "Window.h"

class GLWindow : public IWindow {
  public:
   GLWindow();
   ~GLWindow();

   bool init(int width, int height, const std::string& title);
   void render();
   void handleInput();
   bool getIsRunning();

   void* getNativeWindow() override;
   void setNativeWindow(void* window) override;
   void onScroll(float delta) override;
   void onKey(int key, int scancode, int action, int mods) override;
   void onResize(int width, int height) override;
   void onClose() override;

  private:
   GLFWwindow* glfwWindow;

   // Render contexts
   // std::unique_ptr<UIContext> mUICtx;

   std::unique_ptr<OpenGL_Context> openglCtx;

   // // UI components
   // std::unique_ptr<Property_Panel> mPropertyPanel;

   // std::unique_ptr<SceneView> mSceneView;

   bool isRunning;
};
