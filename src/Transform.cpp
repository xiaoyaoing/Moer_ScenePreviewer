#include "Transform.h"

namespace Transform {

float AngleValue::getDegrees() const { return degrees; }
float AngleValue::getRadians() const { return radians; }
void AngleValue::setDegrees(float deg) {
   degrees = deg;
   radians = convertDeg2Rad(deg);
}
void AngleValue::setRadians(float rad) {
   radians = rad;
   degrees = convertRad2Deg(rad);
}
float AngleValue::convertRad2Deg(float rad) {
   float deg = 0.0;
   if (rad != 0.0) {
      deg = rad * 180.0 / PI_F;
   }
   return deg;
}
float AngleValue::convertDeg2Rad(float deg) {
   float rad = 0.0;
   if (deg != 0.0) {
      rad = deg * PI_F / 180.0;
   }
   return rad;
}

Matrix4f getScale(float x, float y, float z) {
   Matrix4f ret = Eigen::Matrix4f::Identity();
   ret(0, 0) = x;
   ret(1, 1) = y;
   ret(2, 2) = z;
   return ret;
}

Matrix4f getTranslate(float x, float y, float z) {
   Matrix4f ret;
   ret(0, 3) = x;
   ret(1, 3) = y;
   ret(2, 3) = z;
   return ret;
}

Matrix4f getRotateEuler(const AngleValue &x, const AngleValue &y,
                        const AngleValue &z, EulerType type) {
   Matrix4f retVal;
   Eigen::Matrix3f block;
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

}  // namespace Transform