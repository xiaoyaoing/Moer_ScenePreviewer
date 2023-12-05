#include "Material.h"

void Material::update(Shader shader) {
   shader.setVector3f("ambient", ambient);
   shader.setVector3f("diffuse", diffuse);
   shader.setVector3f("specular", specular);
   shader.setFloat("shininess", shininess);
}

DefaultWhiteMaterial::DefaultWhiteMaterial() {
   ambient = Vector3f(0.2, 0.2, 0.2);
   diffuse = Vector3f(0.8, 0.8, 0.8);
   specular = Vector3f(1.0, 1.0, 1.0);
   shininess = 32.0;
}