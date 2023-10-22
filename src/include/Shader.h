#ifndef __SHADER_H__
#define __SHADER_H__
#include <Eigen/Dense>

#include "RenderTarget.h"
using Vector4d = Eigen::Vector4d;
using Vector3d = Eigen::Vector3d;
using Matrix3d = Eigen::Matrix3d;
using Matrix4d = Eigen::Matrix4d;

class Shader {
  public:
   virtual Vector4d vertex(int iface, int nthvert) = 0;
   virtual bool fragment(Vector3d barycentric, RGBColor& color) = 0;
};

class GouraudShader : public Shader {
  private:
   Matrix3d varying_tri;     // triangle coordinates before Viewport transform,
                             // written by VS, read by FS
   Matrix3d varying_normal;  // the normal vector of this triangle,
                             // written by VS, read by FS
   Vector3d varying_intensity;

   Matrix4d uniform_M;        //  Projection*View
   Matrix4d uniform_MIT;      // (Projection*View).invert_transpose()

  public:
   GouraudShader();
   Vector4d vertex(int iface, int nthvert) override final;
   bool fragment(Vector3d barycentric, RGBColor& color) override final;
};

// class ShadowShader : public Shader {
//    /*

//    █▀▀▄ █▀▀ █▀▀█ █▀▀█ █▀▀ █▀▀ █▀▀█ ▀▀█▀▀ █▀▀ █▀▀▄
//    █░░█ █▀▀ █░░█ █▄▄▀ █▀▀ █░░ █▄▄█ ░░█░░ █▀▀ █░░█
//    ▀▀▀░ ▀▀▀ █▀▀▀ ▀░▀▀ ▀▀▀ ▀▀▀ ▀░░▀ ░░▀░░ ▀▀▀ ▀▀▀░

//    */
//    Matrix3d varying_tri;

//   public:
//    ShadowShader() = default;
//    Vector4d vertex(int iface, int nthvert) override final;
//    bool fragment(Vector3d barycentric, RGBColor& color) override final;
// };

// class ShadowMappingShader : public Shader {
//    /*

//       █▀▀▄ █▀▀ █▀▀█ █▀▀█ █▀▀ █▀▀ █▀▀█ ▀▀█▀▀ █▀▀ █▀▀▄
//       █░░█ █▀▀ █░░█ █▄▄▀ █▀▀ █░░ █▄▄█ ░░█░░ █▀▀ █░░█
//       ▀▀▀░ ▀▀▀ █▀▀▀ ▀░▀▀ ▀▀▀ ▀▀▀ ▀░░▀ ░░▀░░ ▀▀▀ ▀▀▀░

//    */
//   private:
//    Matrix3d varying_tri;     // triangle coordinates before Viewport
//    transform,
//                              // written by VS, read by FS
//    Matrix3d varying_normal;  // the normal vector of this triangle,
//                              // written by VS, read by FS
//    Vector3d varying_intensity;

//    Matrix4d uniform_M;        //  Projection*View
//    Matrix4d uniform_MIT;      // (Projection*View).invert_transpose()
//    Matrix4d uniform_Mshadow;  // transform framebuffer screen coordinates to
//                               // shadowbuffer screen coordinates

//   public:
//    ShadowMappingShader();
//    Vector4d vertex(int iface, int nthvert) override final;
//    bool fragment(Vector3d barycentric, RGBColor& color) override final;
// };

#endif