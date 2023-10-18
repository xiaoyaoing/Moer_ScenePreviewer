#include "Render.h"

namespace Render {
Vector3d barycentric(std::vector<Vector4d>& TriangleVertices,
                     Vector4d queryPoint) {
   Vector3d x{(double)(TriangleVertices[1] - TriangleVertices[0]).x(),
              (double)(TriangleVertices[2] - TriangleVertices[0]).x(),
              (double)(TriangleVertices[0] - queryPoint).x()};
   Vector3d y{(double)(TriangleVertices[1] - TriangleVertices[0]).y(),
              (double)(TriangleVertices[2] - TriangleVertices[0]).y(),
              (double)(TriangleVertices[0] - queryPoint).y()};
   Vector3d temp = x.cross(y);   // result = k[u, v, 1];
   if (std::abs(temp[2]) < 1) {  // which means temp.z is zero
      // in this case the triangle is degenerate to a line
      // we will assume the point p is not in the triangle
      // so return any Vector3d that has a negative value in it
      return Vector3d(-1, 1, 1);
   }
   // the return value should be [1-u-v, u, v]
   double u = temp[0] / temp[2];
   double v = temp[1] / temp[2];
   Vector3d ret = Vector3d{1.f - u - v, u, v};
   return ret;
}

void updateBoundingBox(const Vector4d& point, Vector2d& bboxmin,
                       Vector2d& bboxmax, const Vector2d& clamp) {
   bboxmin[0] = std::max(0.0, std::min(bboxmin.x(), point.x()));
   bboxmin[1] = std::max(0.0, std::min(bboxmin.y(), point.y()));

   bboxmax[0] = std::min(clamp.x(), std::max(bboxmax.x(), point.x()));
   bboxmax[1] = std::min(clamp.y(), std::max(bboxmax.y(), point.y()));
}

Vector2d initializeClamp(const RGBColorImage& image) {
   return Vector2d(image.getWidth() - 1, image.getHeight() - 1);
}

Vector2d initializeBBoxMin() {
   return Vector2d(std::numeric_limits<double>::max(),
                   std::numeric_limits<double>::max());
}

Vector2d initializeBBoxMax() {
   return Vector2d(-std::numeric_limits<double>::max(),
                   -std::numeric_limits<double>::max());
}

Vector3d calculateBarycentricClip(
    const Vector3d& barycentricScreen,
    const std::vector<Vector4d>& pointsWithWeight) {
   Vector3d barycentricClip =
       Vector3d(barycentricScreen.x() / pointsWithWeight[0][3],
                barycentricScreen.y() / pointsWithWeight[1][3],
                barycentricScreen.z() / pointsWithWeight[2][3]);
   barycentricClip =
       barycentricClip /
       (barycentricClip.x() + barycentricClip.y() + barycentricClip.z());
   return barycentricClip;
}

double interpolateZValue(const std::vector<Vector4d>& points,
                        const Vector3d& barycentricClip) {
   return Vector3d(points[0].z(), points[1].z(), points[2].z())
       .dot(barycentricClip);
}

void rasterizePixel(const Vector2i& pointInBBox,
                    const std::vector<Vector4d>& points,
                    const Vector3d& barycentricClip, ZBuffer& zbuffer,
                    Shader& shader, RGBColorImage& image) {
   double pointInBBox_z = interpolateZValue(points, barycentricClip);

   if (zbuffer.setPixel(pointInBBox.x(), pointInBBox.y(), pointInBBox_z)) {
      RGBColor color;
      bool discard = shader.fragment(barycentricClip, color);
      if (!discard) {
         image.setPixel(pointInBBox.x(), pointInBBox.y(), color);
      }
   }
}

void triangle(std::vector<Vector4d>& pointsWithWeight, Shader& shader,
              ZBuffer& zbuffer, RGBColorImage& image) {
   Vector2d BBoxMin = initializeBBoxMin();
   Vector2d BBoxMax = initializeBBoxMax();
   Vector2d clamp = initializeClamp(image);

   std::vector<Vector4d> points = pointsWithWeight;
   for (int i = 0; i < 3; i++) {
      std::cout << points[i].transpose() << std::endl;
      points[i] = points[i] / points[i].w();
      std::cout << points[i].transpose() << std::endl;
   }
   std::cout << std::endl;
   for (int i = 0; i < 3; i++) {
      updateBoundingBox(points[i], BBoxMin, BBoxMax, clamp);
   }

   Vector2i samplePoint;
   Vector2i sample_x_range(std::floor(BBoxMin.x()), std::ceil(BBoxMax.x()));
   Vector2i sample_y_range(std::floor(BBoxMin.y()), std::ceil(BBoxMax.y()));

   std::cout << sample_x_range.transpose() << std::endl;
   std::cout << sample_y_range.transpose() << std::endl;
   std::cout << std::endl;
   std::cout << std::endl;

   for (samplePoint.x() = sample_x_range[0];
        samplePoint.x() < sample_x_range[1]; samplePoint.x()++) {
      for (samplePoint.y() = sample_y_range[0];
           samplePoint.y() < sample_y_range[1]; samplePoint.y()++) {
         Vector4d queryPoint(samplePoint.x(), samplePoint.y(), 1, 1);
         Vector3d barycentricScreen = barycentric(points, queryPoint);
         Vector3d barycentricClip =
             calculateBarycentricClip(barycentricScreen, pointsWithWeight);
         if (barycentricClip.x() < 0 || barycentricClip.y() < 0 ||
             barycentricClip.z() < 0) {
            continue;
         }
         rasterizePixel(samplePoint, points, barycentricClip, zbuffer, shader,
                        image);
      }
   }
}
};  // namespace Render
