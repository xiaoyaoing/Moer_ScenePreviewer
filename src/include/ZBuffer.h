#ifndef __ZBUFFER_H__
#define __ZBUFFER_H__
#include <vector>
#include <stdexcept>

class ZBuffer {
   // Image origin is the left bottom corner
  public:
   size_t width, height;
   ZBuffer(size_t width, size_t height);
   double getPixel(size_t x, size_t y);
   bool setPixel(double x, double y, double depth);

  private:
   std::vector<double> data;
};


#endif