#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include "GLWindow.h"
#include "Scene.h"

int main(int argc, char** argv) {
   std::unique_ptr<GLWindow> window = std::make_unique<GLWindow>();
   window->init(1280, 720, "preview");

   while (window->getIsRunning()) {
      window->render();
   }
   return 0;
}