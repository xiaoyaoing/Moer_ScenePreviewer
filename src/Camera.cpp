#include "Camera.h"

PinHoleCamera::PinHoleCamera(Point3d lookFrom, Point3d lookAt, Vector3d up,
                             double xFov, Vector2i resolution) {
   setActualValue(lookFrom, lookAt, up, xFov, resolution);
}

PinHoleCamera::PinHoleCamera(const Json& cameraJson) {
   const Json& transform_json = cameraJson.at("transform");
   Vector3d lookAt, lookFrom, up;
   if (transform_json.contains("position")) {
      auto position = transform_json.at("position");
      lookFrom = Vector3d(position[0], position[1], position[2]);
   } else {
      lookFrom = Vector3d(0, 0, 0);
   }

   if (transform_json.contains("look_at")) {
      auto look_at = transform_json.at("look_at");
      lookAt = Vector3d(look_at[0], look_at[1], look_at[2]);
   } else {
      lookAt = Vector3d(0, 1, 0);
   }

   if (transform_json.contains("up")) {
      auto _up = transform_json.at("up");
      up = Vector3d(_up[0], _up[1], _up[2]);
   } else {
      up = Vector3d(0, 0, 1);
   }

   double xFov = 45.f;
   if (cameraJson.contains("fov")) {
      xFov = cameraJson.at("fov");
   }

   Vector2i resolution;
   if (cameraJson.contains("resolution")) {
      auto _resolution = cameraJson.at("resolution");
      resolution = Vector2i(_resolution[0], _resolution[1]);
   } else {
      resolution = Vector2i(512, 512);
   }

#ifdef DEBUG
   std::cout << " Camera Info: " << std::endl;
#endif
#ifdef DEBUG
   std::cout << "    lookFrom: " << lookFrom.transpose() << std::endl;
#endif
#ifdef DEBUG
   std::cout << "    lookAt: " << lookAt.transpose() << std::endl;
#endif
#ifdef DEBUG
   std::cout << "    up: " << up.transpose() << std::endl;
#endif
#ifdef DEBUG
   std::cout << "    xfov: " << xFov << std::endl;
#endif
#ifdef DEBUG
   std::cout << "resolution: " << resolution.transpose() << std::endl;
#endif
   setActualValue(lookFrom, lookAt, up, xFov, resolution);
}

void PinHoleCamera::setActualValue(Point3d lookFrom, Point3d lookAt,
                                   Vector3d up, double xFov,
                                   Vector2i resolution) {
   double aspectRatio = double(resolution[0]) / resolution[1];
   double distToFilm = 1.0f / tan(xFov * Transform::PI_F / 360);
   view = Transform::getLookAt(lookFrom, lookAt - lookFrom, up);

   double nearPlane = -distToFilm;
   double farPlane = -100.f;
   projection = Transform::getPerspective(Transform::AngleDegreeValue(xFov),
                                          aspectRatio, nearPlane, farPlane);
   viewPort = Transform::getViewPort(resolution);
   worldToScreen = viewPort * projection * view;

   film = std::make_unique<RGBColorImage>(resolution[0], resolution[1]);
   zBuffer = std::make_unique<ZBuffer>(resolution[0], resolution[1]);

   this->right = view.row(0).head<3>();
   this->up = view.row(1).head(3);
   this->lookAt = -view.row(2).head<3>();
   this->pointLookAt = lookAt;
   this->cameraPosition = lookFrom;
}