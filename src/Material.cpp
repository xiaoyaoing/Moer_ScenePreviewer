#include "Material.h"

void Material::update(Shader shader) {
   shader.setVector3f("material.ambient", ambient);
   shader.setVector3f("material.diffuse", diffuse);
   shader.setVector3f("material.specular", specular);
   shader.setFloat("material.shininess", shininess);
}

DefaultWhiteMaterial::DefaultWhiteMaterial() {
   ambient = Vector3f(0.24, 0.24, 0.24);
   diffuse = Vector3f(0.8, 0.8, 0.8);
   specular = Vector3f(1.0, 1.0, 1.0);
   shininess = 24.0;
}