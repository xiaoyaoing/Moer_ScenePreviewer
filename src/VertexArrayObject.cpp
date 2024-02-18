#include "VertexArrayObject.h"

void VertexArrayObject::create_buffers(std::shared_ptr<Mesh> mesh) {
   // Create and bind VAO
   glGenVertexArrays(1, &VAO);

   glBindVertexArray(VAO);
   // Create and bind VBO - Vertex positions
   glGenBuffers(1, &VBO);
   glBindBuffer(GL_ARRAY_BUFFER, VBO);
   glBufferData(GL_ARRAY_BUFFER, mesh->openglVertices.size() * sizeof(Vector3f),
                mesh->openglVertices.data(), GL_STATIC_DRAW);
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
   glEnableVertexAttribArray(0);

   // Create and bind VBO - Normals
   glGenBuffers(1, &VBO_normal);
   glBindBuffer(GL_ARRAY_BUFFER, VBO_normal);
   glBufferData(GL_ARRAY_BUFFER, mesh->openglNormals.size() * sizeof(Vector3f),
                mesh->openglNormals.data(), GL_STATIC_DRAW);
   glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
   glEnableVertexAttribArray(1);

   // Create and bind EBO - Index data
   glGenBuffers(1, &EBO);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

   indexCnt = mesh->openglIndex.size();
   glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                mesh->openglIndex.size() * sizeof(unsigned int),
                mesh->openglIndex.data(), GL_STATIC_DRAW);
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
   glDeleteBuffers(1, &VBO_normal);
   glDeleteVertexArrays(1, &VAO);
}

void VertexArrayObject::bind() { glBindVertexArray(VAO); }

void VertexArrayObject::unbind() { glBindVertexArray(0); }

void VertexArrayObject::draw() {
   bind();
   glDrawElements(GL_TRIANGLES, indexCnt, GL_UNSIGNED_INT, nullptr);
   unbind();
}

void QuadVertexArrayObject::create_buffers() {
   // represented in (pos.x, pox.y, uv.x, uv.y)
   float quadVertices[] = {-1.0f, 1.0f,  0.0f, 1.0f, -1.0f, -1.0f, 0.0f, 0.0f,
                           1.0f,  -1.0f, 1.0f, 0.0f, -1.0f, 1.0f,  0.0f, 1.0f,
                           1.0f,  -1.0f, 1.0f, 0.0f, 1.0f,  1.0f,  1.0f, 1.0f};
   glGenVertexArrays(1, &VAO);
   glGenBuffers(1, &VBO);
   glBindVertexArray(VAO);
   glBindBuffer(GL_ARRAY_BUFFER, VBO);
   glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices,
                GL_STATIC_DRAW);
   glEnableVertexAttribArray(0);
   glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
   glEnableVertexAttribArray(1);
   glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float),
                         (void*)(2 * sizeof(float)));
}

void QuadVertexArrayObject::delete_buffers() {
   glDisableVertexAttribArray(0);
   glDisableVertexAttribArray(1);
   glBindBuffer(GL_ARRAY_BUFFER, 0);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
   glDeleteBuffers(1, &VBO);
   glDeleteVertexArrays(1, &VAO);
}

void QuadVertexArrayObject::draw() {
   bind();
   glDrawArrays(GL_TRIANGLES, 0, 6);
   unbind();
}