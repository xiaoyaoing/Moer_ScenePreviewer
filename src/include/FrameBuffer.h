#pragma once
#include <glad/glad.h>
class FrameBuffer {
  public:
   void create_buffers(int width, int height);
   void delete_buffers();
   void bind();
   void unbind();
   GLuint get_texture();

  protected:
   GLuint FBO;
   GLuint textureColorId;
   GLuint depthBufferId;
   int width;
   int height;
};