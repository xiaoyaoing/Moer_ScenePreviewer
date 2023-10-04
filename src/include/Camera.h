#ifndef __CAMERA_H__
#define __CAMERA_H__
#include <nlohmann/json.hpp>

#include "Transform.h"

using Vector3f = Eigen::Vector3f;
using Vector2i = Eigen::Vector2i;
using Json = nlohmann::json;

class PinHoleCamera {
  public:
   PinHoleCamera();
   PinHoleCamera(const Json& json);

  public:
   // cameraToWorld : Turn a vector in camera coordinate to world coordinate
   // sampleToFilm :  Turn a point [x, y, 0] to the corresponding point on film
   //
   //      [0, 0]---------------> x
   //           |      |
   //           |------+ [x, y]
   //           |
   //           y
   Matrix4f cameraToWorld, sampleToFilm;
};
#endif