#pragma once
#include <Eigen/Dense>

#include "Shader.h"

using Vector3f = Eigen::Vector3f;
class Light {
  public:
   Vector3f position;
   Vector3f ambient;
   Vector3f diffuse;
   Vector3f specular;

   void update(Shader shader);
};

class DefaultWhiteLight : public Light {
  public:
   DefaultWhiteLight();
};