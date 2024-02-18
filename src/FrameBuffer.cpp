#include "FrameBuffer.h"

#include <iostream>

void FrameBuffer::create_buffers(int width, int height) {
   this->width = width;
   this->height = height;
   glGenFramebuffers(1, &FBO);
   glBindFramebuffer(GL_FRAMEBUFFER, FBO);

   // generate texture
   glGenTextures(1, &textureColorId);
   glBindTexture(GL_TEXTURE_2D, textureColorId);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                GL_UNSIGNED_BYTE, NULL);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glBindTexture(GL_TEXTURE_2D, 0);
   // attach it to currently bound FBO object
   glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                          textureColorId, 0);

   glGenRenderbuffers(1, &depthBufferId);
   glBindRenderbuffer(GL_RENDERBUFFER, depthBufferId);
   glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
   glBindRenderbuffer(GL_RENDERBUFFER, 0);

   glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
                             GL_RENDERBUFFER, depthBufferId);

   if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
      std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!"
                << std::endl;
   glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::delete_buffers() {
   if (FBO) {
      glDeleteFramebuffers(GL_FRAMEBUFFER, &FBO);
      glDeleteTextures(1, &textureColorId);
      glDeleteTextures(1, &depthBufferId);
      textureColorId = 0;
      depthBufferId = 0;
   }
}

void FrameBuffer::bind() {
   glBindFramebuffer(GL_FRAMEBUFFER, FBO);
   glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
   glEnable(GL_DEPTH_TEST);
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glViewport(0, 0, width, height);
   glEnable(GL_DEPTH_TEST);
   glEnable(GL_MULTISAMPLE);
}

void FrameBuffer::unbind() { glBindFramebuffer(GL_FRAMEBUFFER, 0); }

GLuint FrameBuffer::get_texture() { return textureColorId; }