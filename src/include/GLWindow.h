#pragma once
#include <memory>

#include "ImGuiContext.h"
#include "ImGuiManager.h"
#include "OpenGLContext.h"
#include "Scene.h"
#include "Window.h"

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
   // Because of dpi scale, the actual height/width may different from init
   // height/weight
   // actual height/width = scale * init height/width
   int actualHeight, actualWidth;

   GLFWwindow* glfwWindow;
   std::unique_ptr<OpenGL_Context> openglCtx;
   std::unique_ptr<ImGui_Context> imguiCtx;
   ImGuiManager imGuiManager;
   std::shared_ptr<Scene> scene;
   bool isRunning;
};
