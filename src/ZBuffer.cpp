#include "ZBuffer.h"

ZBuffer::ZBuffer(size_t _width, size_t _height)
    : width(_width), height(_height) {
   data.reserve(_width * _height);
}

float ZBuffer::getPixel(size_t x, size_t y) {
   if (x > width || y > height) {
      throw std::out_of_range("Out of Image's range");
   }
   return data[y * width + x];
}
bool ZBuffer::setPixel(float x, float y, float depth) {
   if (x > width || y > height) {
      throw std::out_of_range("Out of Image's range");
   }
   size_t index = static_cast<size_t>(y * width + x);
   if (depth < data[index]) {
      data[index] = depth;
      return true;
   }
   return false;
}