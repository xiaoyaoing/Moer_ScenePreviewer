#ifndef __MESH_H__
#define __MESH_H__
#include <tiny_obj_loader.h>

#include <Eigen/Dense>
#include <iostream>
#include <string>
#include <vector>

using Vector3f = Eigen::Vector3f;
using Vector2f = Eigen::Vector2f;
using Vector3i = Eigen::Vector3i;

class Mesh {
  private:
   std::vector<Vector3f> vertices;
   std::vector<Vector3f> normals;
   std::vector<Vector2f> uvs;
   std::vector<std::vector<Vector3i>>
       faces;  // this Vec3i means vertex/uv/normal index
  public:
   Mesh();
   Mesh(std::string file_path);
   int vertices_nr();
   int faces_nr();
   Vector3f normal(size_t iface, size_t nth_vertex);
   Vector3f vertex(size_t ivertex);
   Vector3f vertex(size_t iface, size_t nth_vertex);
};

#endif