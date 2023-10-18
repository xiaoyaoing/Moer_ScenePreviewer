#include "Transform.h"

namespace Transform {
AngleValue::AngleValue() {
   degrees = 0.0;
   radians = 0.0;
}
double AngleValue::getDegrees() const { return degrees; }
double AngleValue::getRadians() const { return radians; }
void AngleValue::setDegrees(double deg) {
   degrees = deg;
   radians = convertDeg2Rad(deg);
}
void AngleValue::setRadians(double rad) {
   radians = rad;
   degrees = convertRad2Deg(rad);
}
double AngleValue::convertRad2Deg(double rad) {
   double deg = 0.0;
   if (rad != 0.0) {
      deg = rad * 180.0 / PI_F;
   }
   return deg;
}
double AngleValue::convertDeg2Rad(double deg) {
   double rad = 0.0;
   if (deg != 0.0) {
      rad = deg * PI_F / 180.0;
   }
   return rad;
}

AngleDegreeValue::AngleDegreeValue(double deg) { setDegrees(deg); }

AngleRadValue::AngleRadValue(double rad) { setRadians(rad); }

Matrix4d getScale(double x, double y, double z) {
   Matrix4d ret = Eigen::Matrix4d::Identity();
   ret(0, 0) = x;
   ret(1, 1) = y;
   ret(2, 2) = z;
   return ret;
}

Matrix4d getTranslate(double x, double y, double z) {
   Matrix4d ret = Eigen::Matrix4d::Identity();
   ret(0, 3) = x;
   ret(1, 3) = y;
   ret(2, 3) = z;
   return ret;
}

Matrix4d getRotateEuler(const AngleValue& x, const AngleValue& y,
                        const AngleValue& z, EulerType type) {
   Matrix4d retVal = Eigen::Matrix4d::Identity();
   Eigen::Matrix3d block;
   auto rotX = Eigen::AngleAxisd(x.getRadians(), Eigen::Vector3d::UnitX());
   auto rotY = Eigen::AngleAxisd(y.getRadians(), Eigen::Vector3d::UnitY());
   auto rotZ = Eigen::AngleAxisd(z.getRadians(), Eigen::Vector3d::UnitZ());
   if (type == EulerType::EULER_XYZ) {
      block = rotZ * rotY * rotX;
   } else if (type == EulerType::EULER_ZYX) {
      block = rotX * rotY * rotZ;
   } else if (type == EulerType::EULER_YZX) {
      block = rotX * rotZ * rotY;
   } else {
      throw std::invalid_argument("Invalid EulerType! in getRotateEuler()");
   }
   retVal.block<3, 3>(0, 0) = block;
   return retVal;
}

Matrix4d getLookAt(const Vector3d& lookFrom, const Vector3d& veclookAt,
                   const Vector3d& up) {
   Matrix4d translateMat =
       getTranslate(-lookFrom[0], -lookFrom[1], -lookFrom[2]);
   Vector3d realLookAt = veclookAt.normalized();
   Vector3d right = realLookAt.cross(up).normalized();
   Vector3d realUp = right.cross(realLookAt).normalized();
   Matrix4d rotMat;
   rotMat << right[0], right[1], right[2], 0.0f, realUp[0], realUp[1],
       realUp[2], 0.0f, -realLookAt[0], -realLookAt[1], -realLookAt[2], 0.0f,
       0.0f, 0.0f, 0.0f, 1.0f;
   Matrix4d lookAt = rotMat * translateMat;
#ifdef DEBUG
   std::cout << "Print LookAt :" << std::endl;
   std::cout << lookAt << std::endl;
#endif
   return lookAt;
}

Matrix4d getOrthographic(double l, double r, double t, double b, double n, double f) {
   // represent left,right,top,buttom,near,far
   Matrix4d trans = Eigen::Matrix4d::Identity();  // translation matrix
   trans << 1, 0, 0, -(r + l) / 2, 0, 1, 0, -(t + b) / 2, 0, 0, 1, -(n + f) / 2,
       0, 0, 0, 1;

   Matrix4d scale = Eigen::Matrix4d::Identity();
   scale << 2 / (r - l), 0, 0, 0, 0, 2 / (t - b), 0, 0, 0, 0, 2 / (n - f), 0, 0,
       0, 0, 1;

   Matrix4d ortho = scale * trans;
#ifdef DEBUG
   std::cout << "Print Orthographic :" << std::endl;
   std::cout << ortho << std::endl;
#endif
   return ortho;
}

Matrix4d getPerspective(const AngleValue& xfov, double aspect, double near,
                        double far) {
   double left = tan(xfov.getRadians() / 2.f) * near;
   double right = -left;
   double buttom = left * 1.f / aspect;
   double top = -buttom;
   Matrix4d ortho = getOrthographic(left, right, top, buttom, near, far);
   Matrix4d perspective = Eigen::Matrix4d::Identity();
   perspective << near, 0, 0, 0, 0, near, 0, 0, 0, 0, near + far, -near * far,
       0, 0, 1, 0;
   perspective = ortho * perspective;
#ifdef DEBUG
   std::cout << "Print Perspective :" << std::endl;
   std::cout << perspective << std::endl;
#endif
   return perspective;
}

Matrix4d getViewPort(Vector2i resolution) {
   Matrix4d viewport;
   viewport << resolution[0] / 2.f, 0, 0, resolution[0] / 2.f, 0,
       resolution[1] / 2.f, 0, resolution[1] / 2.f, 0, 0, 1, 0, 0, 0, 0, 1;
#ifdef DEBUG
   std::cout << "Print Viewport :" << std::endl;
   std::cout << viewport << std::endl;
#endif
   return viewport;
}

}  // namespace Transform