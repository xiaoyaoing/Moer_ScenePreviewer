#ifndef __TRANSFORM_H__
#define __TRANSFORM_H__

#include <Eigen/Dense>

using Matrix4f = Eigen::Matrix4f;
using Vector3f = Eigen::Vector3f;
using Vector2i = Eigen::Vector2i;
namespace Transform {
constexpr float PI = 3.141592653589793238463;
constexpr float PI_F = 3.14159265358979f;

enum class EulerType { EULER_XYZ, EULER_ZYX, EULER_YZX };

class AngleValue {
  protected:
   float degrees;
   float radians;
   AngleValue();  // any initialization should call AngleDegreeValue or
                  // AngleRadValue

  public:
   float getDegrees() const;
   float getRadians() const;
   void setDegrees(float deg);
   void setRadians(float rad);
   static float convertDeg2Rad(float deg);
   static float convertRad2Deg(float rad);
};

class AngleDegreeValue : public AngleValue {
  public:
   AngleDegreeValue(float deg);
};

class AngleRadValue : public AngleValue {
  public:
   AngleRadValue(float rad);
};

Matrix4f getScale(float x, float y, float z);
Matrix4f getTranslate(float x, float y, float z);
Matrix4f getRotateEuler(const AngleValue& x, const AngleValue& y,
                        const AngleValue& z, EulerType type);
Matrix4f getView(const Vector3f& lookFrom, const Vector3f& lookAt_vec,
                 const Vector3f& up);
Matrix4f getPerspective(const AngleValue& fov, float aspect, float near,
                        float far);
Matrix4f getOpenGLPerspective(const AngleValue& xfov, float aspect, float near,
                              float far);
Matrix4f getViewPort(Vector2i resolution);
}  // namespace Transform

#endif