#pragma once
#include <Eigen/Dense>

#include "Shader.h"

using Vector3f = Eigen::Vector3f;
class Material {
  public:
   Vector3f ambient;
   Vector3f diffuse;
   Vector3f specular;
   float shininess;

   void update(Shader shader);
};

class DefaultWhiteMaterial : public Material {
  public:
   DefaultWhiteMaterial();
};