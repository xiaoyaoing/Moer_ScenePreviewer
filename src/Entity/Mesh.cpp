#include "Mesh.h"

void components_to_vec3f(const std::vector<tinyobj::real_t>& components,
                         std::vector<Vector3f>& dst);
void components_to_vec2f(const std::vector<tinyobj::real_t>& components,
                         std::vector<Vector2f>& dst);
void load_triangle_faces(const tinyobj::shape_t& shape,
                         std::vector<std::vector<Vector3i>>& faces);

Mesh::Mesh() {}

Mesh::Mesh(std::string file_path) {
   tinyobj::attrib_t attrib;
   std::vector<tinyobj::shape_t> shapes;
   std::vector<tinyobj::material_t> objmaterials;  // not used
   std::string warn, err;
   bool success = tinyobj::LoadObj(&attrib, &shapes, &objmaterials, &warn, &err,
                                   file_path.c_str());
   if (!err.empty()) {
      std::cerr << err << std::endl;
   } else if (!warn.empty()) {
      std::cerr << warn << std::endl;
   }
   if (!success) {
      exit(1);
   }
   components_to_vec3f(attrib.vertices, this->vertices);
   components_to_vec3f(attrib.normals, this->normals);
   components_to_vec2f(attrib.texcoords, this->uvs);
   for (auto shape = shapes.begin(); shape != shapes.end(); shape++) {
      load_triangle_faces(*shape, this->faces);
   }
   std::cout << "Load model: " << file_path << " sucessfully." << std::endl;
}

size_t Mesh::faces_nr() { return faces.size(); }

size_t Mesh::vertices_nr() { return vertices.size(); }

Vector3f Mesh::normal(size_t iface, size_t nth_vertex) {
   return normals[faces[iface][nth_vertex][2]].normalized();
}

Vector3f Mesh::vertex(size_t iface, size_t nth_vertex) {
   return vertices[faces[iface][nth_vertex][0]].normalized();
}

Vector3f Mesh::vertex(size_t ivertex) { return vertices[ivertex]; }

void components_to_vec3f(const std::vector<tinyobj::real_t>& components,
                         std::vector<Vector3f>& dst) {
   if (components.size() % 3 != 0) {
      throw "Load Error";
   }
   size_t component_nr = components.size() / 3;
   dst.reserve(component_nr);
   for (size_t i = 0; i < component_nr; i++) {
      dst[i] = Vector3f(components[i * 3], components[i * 3 + 1],
                        components[i * 3 + 2]);
   }
}

void components_to_vec2f(const std::vector<tinyobj::real_t>& components,
                         std::vector<Vector2f>& dst) {
   if (components.size() % 2 != 0) {
      throw "Load Error";
   }
   size_t component_nr = components.size() / 2;
   dst.reserve(component_nr);
   for (size_t i = 0; i < component_nr; i++) {
      dst[i] = Vector2f(components[i * 2], components[i * 2 + 1]);
   }
}

void load_triangle_faces(const tinyobj::shape_t& shape,
                         std::vector<std::vector<Vector3i>>& faces) {
   const std::vector<tinyobj::index_t>& indices = shape.mesh.indices;
   if (indices.size() % 3 != 0) {
      throw "Load Error";
   }
   for (size_t i = 0; i < indices.size(); i++) {
      std::vector<Vector3i> vertices;
      vertices.reserve(3);
      for (size_t j = 0; j < 3; j++) {
         vertices[j] = Vector3i(indices[3 * i + j].vertex_index,
                                indices[3 * i + j].texcoord_index,
                                indices[3 * i + j].normal_index);
      }
      faces.push_back(vertices);
   }
}