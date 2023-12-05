#include "Light.h"

void Light::update(Shader shader) {
   shader.setVector3f("position", position);
   shader.setVector3f("ambient", ambient);
   shader.setVector3f("diffuse", diffuse);
   shader.setVector3f("specular", specular);
}

DefaultWhiteLight::DefaultWhiteLight() {
   ambient = Vector3f(0.4, 0.4, 0.4);
   diffuse = Vector3f(1.f, 1.f, 1.f);
   specular = Vector3f(1.f, 1.f, 1.f);
}