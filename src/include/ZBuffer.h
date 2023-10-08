#ifndef __ZBUFFER_H__
#define __ZBUFFER_H__
#include <vector>
#include <stdexcept>

class ZBuffer {
   // Image origin is the left bottom corner
  public:
   size_t width, height;
   ZBuffer(size_t width, size_t height);
   float getPixel(size_t x, size_t y);
   void setPixel(float x, float y, float depth);

  private:
   std::vector<float> data;
};


#endif