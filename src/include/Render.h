#ifndef __RENDER_H__
#define __RENDER_H__
#include <Eigen/Dense>

#include "Image.h"
#include "Shader.h"
#include "ZBuffer.h"

using Matrix4d = Eigen::Matrix4d;
using Vector3d = Eigen::Vector3d;
using Vector4d = Eigen::Vector4d;
using Vector2i = Eigen::Vector2i;
using Vector2d = Eigen::Vector2d;

namespace Render {

Vector3d barycentric(std::vector<Vector4d>& TriangleVertices,
                     Vector4d queryPoint);
void triangle(std::vector<Vector4d>& pointsWithWeight, Shader& shader,
              ZBuffer& zbuffer, RGBColorImage& image);

};  // namespace Render

#endif