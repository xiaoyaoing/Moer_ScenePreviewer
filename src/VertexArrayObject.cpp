#include "VertexArrayObject.h"

void VertexArrayObject::create_buffers(const Mesh& mesh) {
   // Create and bind VAO
   glGenVertexArrays(1, &VAO);
   glBindVertexArray(VAO);

   // Create and bind VBO - Vertex positions
   glGenBuffers(1, &VBO);
   glBindBuffer(GL_ARRAY_BUFFER, VBO);
   glBufferData(GL_ARRAY_BUFFER, mesh.vertices.size() * sizeof(Vector3f),
                mesh.vertices.data(), GL_STATIC_DRAW);
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
   glEnableVertexAttribArray(0);

   // Create and bind VBO - Normals
   glGenBuffers(1, &VBO);
   glBindBuffer(GL_ARRAY_BUFFER, VBO);
   glBufferData(GL_ARRAY_BUFFER, mesh.normals.size() * sizeof(Vector3f),
                mesh.normals.data(), GL_STATIC_DRAW);
   glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
   glEnableVertexAttribArray(1);

   // Create and bind VBO - UV coordinates
   // Does not use this in this project for now.
   glGenBuffers(1, &VBO);
   glBindBuffer(GL_ARRAY_BUFFER, VBO);
   glBufferData(GL_ARRAY_BUFFER, mesh.uvs.size() * sizeof(Vector2f),
                mesh.uvs.data(), GL_STATIC_DRAW);
   glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
   glEnableVertexAttribArray(2);

   // Create and bind EBO - Index data
   glGenBuffers(1, &EBO);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

   // Calculate the number of indices
   indexCnt = mesh.faces.size() * 3;
   assert(indexCnt == mesh.vertices.size());

   // Copy index data to EBO
   std::vector<unsigned int> indices;
   for (const auto& face : mesh.faces) {
      for (const auto& vertexIndex : face) {
         indices.push_back(vertexIndex.x());
      }
   }
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
                indices.data(), GL_STATIC_DRAW);

   // Unbind VAO, VBO, and EBO
   glBindVertexArray(0);
   glBindBuffer(GL_ARRAY_BUFFER, 0);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void VertexArrayObject::delete_buffers() {
   glDisableVertexAttribArray(0);
   glDisableVertexAttribArray(1);
   glBindBuffer(GL_ARRAY_BUFFER, 0);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
   glDeleteBuffers(1, &EBO);
   glDeleteBuffers(1, &VBO);
   glDeleteVertexArrays(1, &VAO);
}

void VertexArrayObject::bind() { glBindVertexArray(VAO); }

void VertexArrayObject::unbind() { glBindVertexArray(0); }

void VertexArrayObject::draw() {
   bind();
   glDrawElements(GL_TRIANGLES, indexCnt, GL_UNSIGNED_INT, nullptr);
   unbind();
}

QuadVertexArrayObject::QuadVertexArrayObject() {
   // represented in (pos.x, pox.y, uv.x, uv.y)
   float quadVertices[] = {-1.0f, 1.0f,  0.0f, 1.0f, -1.0f, -1.0f, 0.0f, 0.0f,
                           1.0f,  -1.0f, 1.0f, 0.0f, -1.0f, 1.0f,  0.0f, 1.0f,
                           1.0f,  -1.0f, 1.0f, 0.0f, 1.0f,  1.0f,  1.0f, 1.0f};
   glGenVertexArrays(1, &VAO);
   glGenBuffers(1, &VBO);

   glBindVertexArray(VAO);
   glBindBuffer(GL_ARRAY_BUFFER, VBO);

   glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices,
                GL_STATIC_DRAW);

   glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
   glEnableVertexAttribArray(0);
   glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float),
                         (void*)(2 * sizeof(float)));
   glEnableVertexAttribArray(1);

   glBindBuffer(GL_ARRAY_BUFFER, 0);
   glBindVertexArray(0);
}

void QuadVertexArrayObject::draw(){
   bind();
   glDrawArrays(GL_TRIANGLES, 0, 6);
   unbind();
}