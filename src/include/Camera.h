#ifndef __CAMERA_H__
#define __CAMERA_H__
#include <iostream>
#include <nlohmann/json.hpp>

#include "Image.h"
#include "Transform.h"
#include "ZBuffer.h"


using Vector3d = Eigen::Vector3d;
using Vector4d = Eigen::Vector4d;
using Vector2i = Eigen::Vector2i;
using Point3f = Eigen::Vector3d;
using Json = nlohmann::json;

class PinHoleCamera {
  public:
   PinHoleCamera(Point3f lookFrom, Point3f lookAt, Vector3d up,
                 double xFov = 45.f, Vector2i resolution = Vector2i(512, 512));
   PinHoleCamera(const Json& cameraJson);
   Vector3d lookAt, up, right;

  public:
   Matrix4d view, projection, viewPort;
   Matrix4d worldToScreen;
   std::unique_ptr<RGBColorImage> film;
   std::unique_ptr<ZBuffer> zBuffer;

  private:
   void setActualValue(Point3f lookFrom, Point3f lookAt, Vector3d up,
                       double xFov, Vector2i resolution);
};
#endif