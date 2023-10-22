#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include "Scene.h"

static void glfw_error_callback(int error, const char* description) {
   std::cerr << "GLFW Error: " << error << " " << description << ". Exit now."
             << std::endl;
   exit(1);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action,
                         int mods) {
   if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
      glfwSetWindowShouldClose(window, GLFW_TRUE);
}

int main(int argc, char** argv) {
   if (argc < 2) {
      std::cout << "Usage: ./program_name <working_directory>" << std::endl;
      return 1;
   }

   std::string workingDir = std::string(argv[1]);
   Scene scene(workingDir);
   scene.render();

   if (!glfwInit()) {
      std::cerr << "Failed to initialize GLFW" << std::endl;
      return -1;
   }
   const int windowWidth = scene.camera->film->getWidth();
   const int windowHeight = scene.camera->film->getHeight();
   GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight,
                                         "Image Window", nullptr, nullptr);
   if (!window) {
      std::cerr << "Failed to create GLFW window" << std::endl;
      glfwTerminate();
      return -1;
   }

   glfwMakeContextCurrent(window);
   gladLoadGL();

   glfwSetKeyCallback(window, key_callback);
   while (!glfwWindowShouldClose(window)) {
      glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT);

      glRasterPos2i(-1, -1);
      glDrawPixels(windowWidth, windowHeight, GL_RGB, GL_UNSIGNED_BYTE,
                   scene.camera->film->getData());

      glfwSwapBuffers(window);

      glfwPollEvents();
   }

   glfwTerminate();
   return 0;
}