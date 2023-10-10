#include "Shader.h"

using Render::currentCamera;
using Render::currentLightCamera;
using Render::currentMesh;

Vector4f ShadowShader::vertex(int iface, int nthvert) {
   Vector4f gl_vertex = currentMesh->vertex(iface, nthvert)
                            .homogeneous();  // read the vertex from .obj file
   Vector4f after_view = currentCamera->view * gl_vertex;
   Vector4f after_projection = currentCamera->projection * after_view;
   Vector4f after_viewport = currentCamera->viewPort * after_projection;
   gl_vertex = currentCamera->worldToScreen *
               gl_vertex;  // transform it to screen coordinates
   varying_tri.col(nthvert) = (gl_vertex / gl_vertex[3]).head<3>();
   return gl_vertex;
}

bool ShadowShader::fragment(Vector3f barycentric, RGBColor &color) {
   Vector3f p = varying_tri * barycentric;
   // need to transform p.z from [-1,1] to [0,1]
   color = RGBColor(255, 255, 255) * ((p.z() + 1.) / 2.);
   return false;
}

ShadowMappingShader::ShadowMappingShader() {
   uniform_M = currentCamera->projection * currentCamera->view;
   uniform_MIT = uniform_M.inverse().transpose();
   uniform_Mshadow = currentLightCamera->worldToScreen *
                     (currentCamera->worldToScreen.inverse());
}

Vector4f ShadowMappingShader::vertex(int iface, int nthvert) {
   Vector4f gl_vertex = currentMesh->vertex(iface, nthvert).homogeneous();
   gl_vertex = currentCamera->worldToScreen * gl_vertex;
   varying_tri.col(nthvert) = (gl_vertex / gl_vertex[3]).head<3>();
   // do not divide gl_vertex[3] here, delay it to triangle()
   return gl_vertex;
}

bool ShadowMappingShader::fragment(Vector3f barycentric, RGBColor &color) {
   Vector4f shadowBuffer_point =
       uniform_Mshadow * (varying_tri * barycentric).homogeneous();
   shadowBuffer_point = shadowBuffer_point /
                        shadowBuffer_point.w();  // point on the shadow buffer
   float shadow = .3 + .7 * ((*currentLightCamera->zBuffer)
                                 .getPixel(shadowBuffer_point.x(),
                                           shadowBuffer_point.y()) <
                             shadowBuffer_point.z() + 43.34);

   Vector3f normal =
       (uniform_MIT * varying_normal.homogeneous()).head<3>().normalized();
   Vector3f light = (uniform_M * currentLightCamera->lookAt.homogeneous())
                        .head<3>()
                        .normalized();
   Vector3f half_vector =
       (uniform_M *
        (-(currentLightCamera->lookAt + currentCamera->lookAt).homogeneous()))
           .head<3>()
           .normalized();
   const float meshSpecular = 60.f;
   float specular =
       std::pow(std::max(0.f, normal.dot(half_vector)), meshSpecular);
   float diffuse = std::max(0.f, normal.dot(light) * -1);

   color = RGBColor(255, 255, 255);
   color = color * shadow * (1.2 * diffuse + 0.8 * specular) + 20;
   return false;
}
