#include "Light.h"

void Light::update(Shader shader) {
   shader.setVector3f("light.position", position);
   shader.setVector3f("light.ambient", ambient);
   shader.setVector3f("light.diffuse", diffuse);
   shader.setVector3f("light.specular", specular);
}

DefaultWhiteLight::DefaultWhiteLight() {
   ambient = Vector3f(0.4, 0.4, 0.4);
   diffuse = Vector3f(1.f, 1.f, 1.f);
   specular = Vector3f(1.f, 1.f, 1.f);
}