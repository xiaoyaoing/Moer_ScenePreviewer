#include "Image.h"

RGBColorImage::RGBColorImage(size_t _width, size_t _height)
    : width(_width), height(_height) {
   data.reserve(_width * _height);
}

RGBColor RGBColorImage::getPixel(size_t x, size_t y) {
   if (x > width || y > height) {
      throw std::out_of_range("Out of Image's range");
   }
   return data[y * width + x];
}
void RGBColorImage::setPixel(size_t x, size_t y, RGBColor color) {
   if (x > width || y > height) {
      throw std::out_of_range("Out of Image's range");
   }
   data[y * width + x] = color;
}

uint8_t* RGBColorImage::getData() {
   return reinterpret_cast<uint8_t*>(data.data());
}