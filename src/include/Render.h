#ifndef __RENDER_H__
#define __RENDER_H__
#include <Eigen/Dense>

#include "Image.h"
#include "Shader.h"
#include "ZBuffer.h"

using Matrix4f = Eigen::Matrix4f;
using Vector3f = Eigen::Vector3f;
using Vector4f = Eigen::Vector4f;
using Vector2i = Eigen::Vector2i;
using Vector2f = Eigen::Vector2f;

namespace Render {

Vector3f barycentric(std::vector<Vector4f>& TriangleVertices,
                     Vector4f queryPoint);
void triangle(std::vector<Vector4f>& pointsWithWeight, Shader& shader,
              ZBuffer& zbuffer, RGBColorImage& image);

};  // namespace Render

#endif