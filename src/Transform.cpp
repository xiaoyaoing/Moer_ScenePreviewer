#include "Transform.h"

#include <cmath>

namespace Transform {
AngleValue::AngleValue() {
   degrees = 0.0;
   radians = 0.0;
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
   Matrix4f ret = Eigen::Matrix4f::Identity();
   ret(0, 3) = x;
   ret(1, 3) = y;
   ret(2, 3) = z;
   return ret;
}

Matrix4f getRotateEuler(const AngleValue& x, const AngleValue& y,
                        const AngleValue& z, EulerType type) {
   Matrix4f retVal = Eigen::Matrix4f::Identity();
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

Matrix4f getView(const Vector3f& lookFrom, const Vector3f& veclookAt,
                 const Vector3f& up) {
   Matrix4f translateMat =
       getTranslate(-lookFrom[0], -lookFrom[1], -lookFrom[2]);
   Vector3f realLookAt = veclookAt.normalized();
   Vector3f right = realLookAt.cross(up).normalized();
   Vector3f realUp = right.cross(realLookAt).normalized();
   Matrix4f rotMat;
   rotMat << right[0], right[1], right[2], 0.0f, realUp[0], realUp[1],
       realUp[2], 0.0f, -realLookAt[0], -realLookAt[1], -realLookAt[2], 0.0f,
       0.0f, 0.0f, 0.0f, 1.0f;
   Matrix4f lookAt = rotMat * translateMat;
   return lookAt;
}

Matrix4f getOrthographic(float l, float r, float t, float b, float n, float f) {
   // represent left,right,top,buttom,near,far
   Matrix4f trans = Eigen::Matrix4f::Identity();  // translation matrix
   trans << 1, 0, 0, -(r + l) / 2, 0, 1, 0, -(t + b) / 2, 0, 0, 1, -(n + f) / 2,
       0, 0, 0, 1;

   Matrix4f scale = Eigen::Matrix4f::Identity();
   scale << 2 / (r - l), 0, 0, 0, 0, 2 / (t - b), 0, 0, 0, 0, 2 / (n - f), 0, 0,
       0, 0, 1;

   Matrix4f ortho = scale * trans;
   return ortho;
}

Matrix4f getPerspective(const AngleValue& xfov, float aspect, float near,
                        float far) {
   assert(near < 0 && far < 0);
   float left = tan(xfov.getRadians() / 2.f) * near;
   float right = -left;
   float buttom = left * 1.f / aspect;
   float top = -buttom;
   Matrix4f ortho = getOrthographic(left, right, top, buttom, near, far);
   Matrix4f perspective = Eigen::Matrix4f::Identity();
   perspective << near, 0, 0, 0, 0, near, 0, 0, 0, 0, near + far, -near * far,
       0, 0, 1, 0;
   perspective = ortho * perspective;
   return perspective;
}

// Reference: http://www.songho.ca/opengl/gl_projectionmatrix.html
Matrix4f getOpenGLPerspective(const AngleValue& xfov, float aspect, float near,
                              float far) {
   assert(near > 0 && far > 0);
   float right = tan(xfov.getRadians() / 2.f) * near;
   float top = right * 1.f / aspect;
   Matrix4f perspective = Matrix4f::Zero();
   perspective << near / right, 0, 0, 0, 0, near / top, 0, 0, 0, 0,
       -(far + near) / (far - near), -2 * (far * near) / (far - near), 0, 0, -1,
       0;
   return perspective;
}

Matrix4f getViewPort(Vector2i resolution) {
   Matrix4f viewport;
   viewport << resolution[0] / 2.f, 0, 0, resolution[0] / 2.f, 0,
       resolution[1] / 2.f, 0, resolution[1] / 2.f, 0, 0, 1, 0, 0, 0, 0, 1;
   return viewport;
}

}  // namespace Transform