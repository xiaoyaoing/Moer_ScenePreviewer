#include "GLWindow.h"

GLWindow::GLWindow() : glfwWindow(nullptr), isRunning(true) {
   // TODO: create context here
   openglCtx = std::make_unique<OpenGL_Context>();
}
void GLWindow::setNativeWindow(void* window) {
   glfwWindow = (GLFWwindow*)window;
}
void* GLWindow::getNativeWindow() { return glfwWindow; }
bool GLWindow::getIsRunning() { return isRunning; }

bool GLWindow::init(int width, int height, const std::string& title) {
   this->width = width;
   this->height = height;
   this->title = title;

   // TODO: init more member here
   openglCtx->init(this);
   return isRunning;
}
GLWindow::~GLWindow() {
   // TODO: call some end function here
   openglCtx->end();
}

void GLWindow::onResize(int width, int height) {
   this->width = width;
   this->height = height;

   // TODO: call actual resize here
   render();
}

void GLWindow::onScroll(float delta) {
   // mSceneView->on_mouse_wheel(delta);
}

void GLWindow::onKey(int key, int scancode, int action, int mods) {
   if (action == GLFW_PRESS) {
   }
}

void GLWindow::onClose() { isRunning = false; }

void GLWindow::render() {
   // // Clear the view
   openglCtx->preRender();

   // // Initialize UI components
   // mUICtx->preRender();

   // // render scene to framebuffer and add it to scene view
   // mSceneView->render();

   // mPropertyPanel->render(mSceneView.get());

   // // Render the UI
   // mUICtx->postRender();

   // // Render end, swap buffers
   openglCtx->postRender();

   handleInput();
}

void GLWindow::handleInput() {
   // TODO: move this and camera to scene UI component?

   if (glfwGetKey(glfwWindow, GLFW_KEY_W) == GLFW_PRESS) {
      // mSceneView->on_mouse_wheel(-0.4f);
   }

   if (glfwGetKey(glfwWindow, GLFW_KEY_S) == GLFW_PRESS) {
      // mSceneView->on_mouse_wheel(0.4f);
   }

   if (glfwGetKey(glfwWindow, GLFW_KEY_F) == GLFW_PRESS) {
      // mSceneView->reset_view();
   }

   double x, y;
   glfwGetCursorPos(glfwWindow, &x, &y);

   // mSceneView->on_mouse_move(x, y, Input::GetPressedButton(mWindow));
}