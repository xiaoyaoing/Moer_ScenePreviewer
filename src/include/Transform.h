#ifndef __TRANSFORM_H__
#define __TRANSFORM_H__

#include <Eigen/Dense>
#include <cmath>
#include <iostream>

using Matrix4d = Eigen::Matrix4d;
using Vector3d = Eigen::Vector3d;
using Vector2i = Eigen::Vector2i;
namespace Transform {
constexpr double PI = 3.141592653589793238463;
constexpr double PI_F = 3.14159265358979f;

enum class EulerType { EULER_XYZ, EULER_ZYX, EULER_YZX };

class AngleValue {
  protected:
   double degrees;
   double radians;
   AngleValue();  // any initialization should call AngleDegreeValue or
                  // AngleRadValue

  public:
   double getDegrees() const;
   double getRadians() const;
   void setDegrees(double deg);
   void setRadians(double rad);
   static double convertDeg2Rad(double deg);
   static double convertRad2Deg(double rad);
};

class AngleDegreeValue : public AngleValue {
  public:
   AngleDegreeValue(double deg);
};

class AngleRadValue : public AngleValue {
  public:
   AngleRadValue(double rad);
};

Matrix4d getScale(double x, double y, double z);
Matrix4d getTranslate(double x, double y, double z);
Matrix4d getRotateEuler(const AngleValue &x, const AngleValue &y,
                        const AngleValue &z, EulerType type);
Matrix4d getLookAt(const Vector3d& lookFrom,const Vector3d& lookAt_vec, const Vector3d& up);
Matrix4d getPerspective(const AngleValue& fov, double aspect,double near,double far);
Matrix4d getViewPort(Vector2i resolution);
}  // namespace Transform

#endif