#include "GLWindow.h"

int main(int argc, char** argv) {
   std::unique_ptr<GLWindow> window = std::make_unique<GLWindow>();
   window->init(1280, 720, "Moer ScenePreviewer");

   while (window->getIsRunning()) {
      window->render();
   }
   return 0;
}