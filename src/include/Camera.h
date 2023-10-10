#ifndef __CAMERA_H__
#define __CAMERA_H__
#include <nlohmann/json.hpp>

#include "Image.h"
#include "Transform.h"
#include "ZBuffer.h"

using Vector3f = Eigen::Vector3f;
using Vector4f = Eigen::Vector4f;
using Vector2i = Eigen::Vector2i;
using Json = nlohmann::json;

class PinHoleCamera {
  public:
   PinHoleCamera(const Json& cameraJson);
   Vector3f lookAt, up, right;

  public:
   Matrix4f view, projection, viewPort;
   Matrix4f worldToScreen;
   std::unique_ptr<RGBColorImage> film;
   std::unique_ptr<ZBuffer> zBuffer;
};
#endif