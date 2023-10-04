#include "Transform.h"

namespace Transform {
AngleValue::AngleValue() {
   degrees = 0.f;
   radians = 0.f;
}
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

AngleDegreeValue::AngleDegreeValue(float deg) { setDegrees(deg); }

AngleRadValue::AngleRadValue(float rad) { setRadians(rad); }

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

Matrix4f getRotateEuler(const AngleValue& x, const AngleValue& y,
                        const AngleValue& z, EulerType type) {
   Matrix4f retVal;
   Eigen::Matrix3f block;
   auto rotX = Eigen::AngleAxisf(x.getRadians(), Eigen::Vector3f::UnitX());
   auto rotY = Eigen::AngleAxisf(y.getRadians(), Eigen::Vector3f::UnitY());
   auto rotZ = Eigen::AngleAxisf(z.getRadians(), Eigen::Vector3f::UnitZ());
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

Matrix4f getLookAt(const Vector3f& lookFrom, const Vector3f& veclookAt,
                   const Vector3f& up) {
   Matrix4f translateMat =
       getTranslate(-lookFrom[0], -lookFrom[1], -lookFrom[2]);
   Vector3f realLookAt = veclookAt.normalized();
   Vector3f right = up.cross(realLookAt).normalized();
   Vector3f realUp = realLookAt.cross(right).normalized();
   Matrix4f rotMat;
   rotMat << -right[0], -right[1], -right[2], 0.0f, realUp[0], realUp[1],
       realUp[2], 0.0f, realLookAt[0], realLookAt[1], realLookAt[2], 0.0f, 0.0f,
       0.0f, 0.0f, 1.0f;
   return rotMat * translateMat;
}

Matrix4f getPerspective(const AngleValue& fov, float aspect, float near,
                        float far) {
   float cotHalfFov = (1 / tan(fov.getRadians() / 2.0));
   Matrix4f retVal;
   retVal << cotHalfFov, 0, 0, 0, 0, cotHalfFov * aspect, 0, 0, 0, 0,
       far / (far - near), -far * near / (far - near), 0, 0, 1, 0;
   return retVal;
}

}  // namespace Transform