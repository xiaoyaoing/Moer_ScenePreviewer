#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <string>

class IWindow {
  public:
   virtual void* getNativeWindow() = 0;
   virtual void setNativeWindow(void* window) = 0;
   virtual void onScroll(float delta) = 0;
   virtual void onKey(int key, int scancode, int action, int mods) = 0;
   virtual void onResize(int width, int height) = 0;
   virtual void onClose() = 0;

   int width;
   int height;
   std::string title;
};