#ifndef __MESH_H__
#define __MESH_H__
#include <tiny_obj_loader.h>

#include <Eigen/Dense>
#include <iostream>
#include <string>
#include <vector>

using Vector3d = Eigen::Vector3d;
using Vector4d = Eigen::Vector4d;
using Vector2d = Eigen::Vector2d;
using Vector3i = Eigen::Vector3i;
using Matrix4d = Eigen::Matrix4d;

class Mesh {
  protected:
   std::vector<Vector3d> vertices;
   std::vector<Vector3d> normals;
   std::vector<Vector2d> uvs;
   std::vector<std::vector<Vector3i>>
       faces;  // this Vec3i means vertex/uv/normal index
  public:
   Mesh();
   Mesh(const std::string& file_path);
   size_t vertices_nr();
   size_t faces_nr();
   Vector3d normal(size_t iface, size_t nth_vertex);
   Vector3d vertex(size_t ivertex);
   Vector3d vertex(size_t iface, size_t nth_vertex);
   void apply(Matrix4d transform);
};

class Quad : public Mesh {
  public:
   Quad();
};

class Cube : public Mesh {
  public:
   Cube();
};

#endif