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

size_t RGBColorImage::getWidth() const { return width; }
size_t RGBColorImage::getHeight() const { return height; }

RGBColor RGBColor::operator*(float scale) {
   int new_red = std::clamp(static_cast<int>(r * scale), 0, 255);
   int new_green = std::clamp(static_cast<int>(g * scale), 0, 255);
   int new_blue = std::clamp(static_cast<int>(b * scale), 0, 255);
   return RGBColor(new_red, new_green, new_blue);
}

RGBColor RGBColor::operator+(int value) {
   int new_red = std::clamp(r + value, 0, 255);
   int new_green = std::clamp(g + value, 0, 255);
   int new_blue = std::clamp(b + value, 0, 255);
   return RGBColor(new_red, new_green, new_blue);
}