#ifndef __IMAGE_H__
#define __IMAGE_H__
#include <memory>
#include <vector>
#include <stdexcept>
class RGBColor {
  public:
   uint8_t r;
   uint8_t g;
   uint8_t b;

  public:
   RGBColor(uint8_t red, uint8_t green, uint8_t blue)
       : r(red), g(green), b(blue) {}
};

class RGBColorImage {
   // Image origin is the left bottom corner
  public:
   size_t width, height;
   RGBColorImage(size_t width, size_t height);
   RGBColor getPixel(size_t x, size_t y);
   void setPixel(size_t x, size_t y, RGBColor color);

  private:
   std::vector<RGBColor> data;
};


#endif