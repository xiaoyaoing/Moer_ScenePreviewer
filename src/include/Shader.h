#ifndef __SHADER_H__
#define __SHADER_H__
#include <Eigen/Dense>

#include "Image.h"
#include "Mesh.h"

using Vector4f = Eigen::Vector4f;
using Vector3f = Eigen::Vector3f;

class Shader {
  public:
   virtual Vector4f vertex(std::shared_ptr<Mesh> mesh, int iface,
                           int nthvert) = 0;
   virtual bool fragment(Vector3f barycentric, RGBColor& color) = 0;
};

#endif