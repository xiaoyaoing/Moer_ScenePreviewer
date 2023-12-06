#pragma once
#include <memory>

#include "OpenGLContext.h"
#include "Window.h"
#include "Scene.h"

class GLWindow : public IWindow {
  public:
   GLWindow();
   ~GLWindow();

   bool init(int width, int height, const std::string& title);
   void render();
   void handleInput();
   bool getIsRunning();
   void setScene(std::shared_ptr<Scene> scene);

   void* getNativeWindow() override;
   void setNativeWindow(void* window) override;
   void onScroll(float delta) override;
   void onKey(int key, int scancode, int action, int mods) override;
   void onResize(int width, int height) override;
   void onClose() override;

  private:
   GLFWwindow* glfwWindow;
   std::unique_ptr<OpenGL_Context> openglCtx;
   std::shared_ptr<Scene> scene;
   bool isRunning;
};
