#include "Shader.h"

using Render::currentCamera;
using Render::currentLightCamera;
using Render::currentMesh;

GouraudShader::GouraudShader() {
   if (currentCamera == nullptr || currentLightCamera == nullptr) {
      throw;
   }
   uniform_M = currentCamera->projection * currentCamera->view;
   uniform_MIT = uniform_M.inverse().transpose();
}

Vector4d GouraudShader::vertex(int iface, int nthvert) {
   Vector4d gl_vertex = currentMesh->vertex(iface, nthvert).homogeneous();

   gl_vertex = currentCamera->worldToScreen * gl_vertex;
   varying_tri.col(nthvert) = (gl_vertex / gl_vertex[3]).head<3>();
   varying_normal.col(nthvert) = currentMesh->normal(iface, nthvert);
   varying_intensity[nthvert] =
       currentMesh->normal(iface, nthvert).dot(currentLightCamera->lookAt) * -1;
   // do not divide gl_vertex[3] here, delay it to triangle()
   return gl_vertex;
}

bool GouraudShader::fragment(Vector3d barycentric, RGBColor &color) {
   Vector3d normal = (uniform_MIT * varying_normal.col(0).homogeneous())
                         .head<3>()
                         .normalized();
   Vector3d light = (uniform_M * currentLightCamera->lookAt.homogeneous())
                        .head<3>()
                        .normalized();
   double intensity = varying_intensity.dot(barycentric);
   color = RGBColor(200, 200, 200);
   color = color * intensity + 30.f;
   return false;
}

/*

█▀▀▄ █▀▀ █▀▀█ █▀▀█ █▀▀ █▀▀ █▀▀█ ▀▀█▀▀ █▀▀ █▀▀▄
█░░█ █▀▀ █░░█ █▄▄▀ █▀▀ █░░ █▄▄█ ░░█░░ █▀▀ █░░█
▀▀▀░ ▀▀▀ █▀▀▀ ▀░▀▀ ▀▀▀ ▀▀▀ ▀░░▀ ░░▀░░ ▀▀▀ ▀▀▀░
Vector4d ShadowShader::vertex(int iface, int nthvert) {
   Vector4d gl_vertex = currentMesh->vertex(iface, nthvert)
                            .homogeneous();  // read the vertex from .obj file
   Vector4d after_view = currentCamera->view * gl_vertex;
   Vector4d after_projection = currentCamera->projection * after_view;
   Vector4d after_viewport = currentCamera->viewPort * after_projection;
   gl_vertex = currentCamera->worldToScreen *
               gl_vertex;  // transform it to screen coordinates
   varying_tri.col(nthvert) = (gl_vertex / gl_vertex[3]).head<3>();
   return gl_vertex;
}

bool ShadowShader::fragment(Vector3d barycentric, RGBColor &color) {
   Vector3d p = varying_tri * barycentric;
   // need to transform p.z from [-1,1] to [0,1]
   color = RGBColor(255, 255, 255) * ((p.z() + 1.) / 2.);
   return false;
}

ShadowMappingShader::ShadowMappingShader() {
   if (currentCamera == nullptr || currentLightCamera == nullptr) {
      throw;
   }
   uniform_M = currentCamera->projection * currentCamera->view;
   uniform_MIT = uniform_M.inverse().transpose();
   uniform_Mshadow = currentLightCamera->worldToScreen *
                     (currentCamera->worldToScreen.inverse());
}

Vector4d ShadowMappingShader::vertex(int iface, int nthvert) {
   Vector4d gl_vertex = currentMesh->vertex(iface, nthvert).homogeneous();

   gl_vertex = currentCamera->worldToScreen * gl_vertex;
   varying_tri.col(nthvert) = (gl_vertex / gl_vertex[3]).head<3>();
   varying_normal.col(nthvert) = currentMesh->normal(iface, nthvert);
   varying_intensity[nthvert] =
       currentMesh->normal(iface, nthvert).dot(currentLightCamera->lookAt) * -1;
   // do not divide gl_vertex[3] here, delay it to triangle()
   return gl_vertex;
}

bool ShadowMappingShader::fragment(Vector3d barycentric, RGBColor &color) {
   Vector4d shadowBuffer_point =
       uniform_Mshadow * (varying_tri * barycentric).homogeneous();
   shadowBuffer_point = shadowBuffer_point /
                        shadowBuffer_point.w();  // point on the shadowbuffer
   double shadow = .3 + .7 * ((*currentLightCamera->zBuffer)
                                  .getPixel(shadowBuffer_point.x(),
                                            shadowBuffer_point.y()) <
                              shadowBuffer_point.z() + 43.34);

   Vector3d normal = (uniform_MIT * varying_normal.col(0).homogeneous())
                         .head<3>()
                         .normalized();
   Vector3d light = (uniform_M * currentLightCamera->lookAt.homogeneous())
                        .head<3>()
                        .normalized();
   Vector3d half_vector =
       (uniform_M *
        (-(currentLightCamera->lookAt + currentCamera->lookAt).homogeneous()))
           .head<3>()
           .normalized();
   const double meshSpecular = 60.f;
   // double specular =
   //     std::pow(std::max(0.0, normal.dot(half_vector)), meshSpecular);
   // double diffuse = std::max(0.0, normal.dot(light) * -1.f);
   double intensity = varying_intensity.dot(barycentric);
   color = RGBColor(255, 255, 255);
   color = color * intensity + 50.f;
   return false;
}
*/