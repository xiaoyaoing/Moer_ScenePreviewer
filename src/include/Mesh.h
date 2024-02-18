#ifndef __MESH_H__
#define __MESH_H__
#include <tiny_obj_loader.h>

#include <Eigen/Dense>
#include <string>
#include <vector>

using Vector3f = Eigen::Vector3f;
using Vector4f = Eigen::Vector4f;
using Vector2f = Eigen::Vector2f;
using Vector3i = Eigen::Vector3i;
using Matrix4f = Eigen::Matrix4f;

class Mesh {
  public:
   std::vector<Vector3f> vertices;
   std::vector<Vector3f> normals;
   std::vector<Vector2f> uvs;
   std::vector<std::vector<Vector3i>>
       faces;  // this Vec3i means vertex/uv/normal index
   // for creating VAO, VBO and EBO
   // FIXME: ? will create huge data redundancy
   std::vector<Vector3f> openglVertices;
   std::vector<Vector3f> openglNormals;
   std::vector<unsigned int> openglIndex;

  public:
   Mesh();
   Mesh(const std::string& file_path);
   size_t vertices_nr();
   size_t faces_nr();
   Vector3f normal(size_t iface, size_t nth_vertex);
   Vector3f vertex(size_t ivertex);
   Vector3f vertex(size_t iface, size_t nth_vertex);
   void apply(Matrix4f transform);

  protected:
   void createOpenGLData(const tinyobj::shape_t& shape);
   // TODO: this is only a temporary solution to accommodate Quad and Cube
   void createOpenGLDataFromFaces();
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