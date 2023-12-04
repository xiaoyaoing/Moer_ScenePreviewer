#ifndef __CAMERA_H__
#define __CAMERA_H__
#include <iostream>
#include <nlohmann/json.hpp>

#include "Image.h"
#include "Transform.h"
#include "ZBuffer.h"


using Vector3f = Eigen::Vector3f;
using Vector4f = Eigen::Vector4f;
using Vector2i = Eigen::Vector2i;
using Point3f = Eigen::Vector3f;
using Json = nlohmann::json;

class PinHoleCamera {
  public:
   PinHoleCamera(Point3f lookFrom, Point3f lookAt, Vector3f up,
                 float xFov = 45.f, Vector2i resolution = Vector2i(512, 512));
   PinHoleCamera(const Json& cameraJson);
   Vector3f lookAt, up, right;
   Point3f cameraPosition, pointLookAt;

  public:
   Matrix4f view, projection, viewPort;
   Matrix4f worldToScreen;
   std::unique_ptr<RGBColorImage> film;
   std::unique_ptr<ZBuffer> zBuffer;

  private:
   void setActualValue(Point3f lookFrom, Point3f lookAt, Vector3f up,
                       float xFov, Vector2i resolution);
};
#endif