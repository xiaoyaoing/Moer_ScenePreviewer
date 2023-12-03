#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include "GLWindow.h"
#include "Scene.h"

int main(int argc, char** argv) {
   if (argc < 2) {
      std::cout << "Usage: ./program_name <working_directory>" << std::endl;
      return 1;
   }

   std::string workingDir = std::string(argv[1]);
   Scene scene(workingDir);
   // scene.render();

   std::unique_ptr<GLWindow> window = std::make_unique<GLWindow>();
   window->init(1280, 720, "preview");
   while (window->getIsRunning()) {
      window->render();
   }
   return 0;
}