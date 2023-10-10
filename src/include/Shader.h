#ifndef __SHADER_H__
#define __SHADER_H__
#include <Eigen/Dense>

#include "RenderTarget.h"
using Vector4f = Eigen::Vector4f;
using Vector3f = Eigen::Vector3f;
using Matrix3f = Eigen::Matrix3f;
using Matrix4f = Eigen::Matrix4f;

class Shader {
  public:
   virtual Vector4f vertex(int iface, int nthvert) = 0;
   virtual bool fragment(Vector3f barycentric, RGBColor& color) = 0;
};

class ShadowShader : public Shader {
  private:
   Matrix3f varying_tri;

  public:
   ShadowShader() = default;
   Vector4f vertex(int iface, int nthvert) override final;
   bool fragment(Vector3f barycentric, RGBColor& color) override final;
};

class ShadowMappingShader : public Shader {
  private:
   Matrix3f varying_tri;     // triangle coordinates before Viewport transform,
                             // written by VS, read by FS
   Vector3f varying_normal;  // the normal vector of this triangle,
                             // written by VS, read by FS

   Matrix4f uniform_M;        //  Projection*View
   Matrix4f uniform_MIT;      // (Projection*View).invert_transpose()
   Matrix4f uniform_Mshadow;  // transform framebuffer screen coordinates to
                              // shadowbuffer screen coordinates

  public:
   ShadowMappingShader();
   Vector4f vertex(int iface, int nthvert) override final;
   bool fragment(Vector3f barycentric, RGBColor& color) override final;
};

#endif