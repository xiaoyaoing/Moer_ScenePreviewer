#pragma once

#include <memory>

#include "Scene.h"
#include <GL/gl.h>

class ImGuiManager {
  public:
   void render(std::shared_ptr<Scene> scene, GLuint renderResultTextureId);
};