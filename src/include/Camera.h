#ifndef __CAMERA_H__
#define __CAMERA_H__
#include <nlohmann/json.hpp>

#include "Transform.h"

using Vector3f = Eigen::Vector3f;
using Vector4f = Eigen::Vector4f;
using Vector2i = Eigen::Vector2i;
using Json = nlohmann::json;

class PinHoleCamera {
  public:
   PinHoleCamera();
   PinHoleCamera(const Json& json);

  public:
   Matrix4f worldToCamera;
   Matrix4f persective, viewPort;
};
#endif