#include "Camera.h"

PinHoleCamera::PinHoleCamera(Point3f lookFrom, Point3f lookAt, Vector3f up,
                             float xFov, Vector2i resolution) {
   setActualValue(lookFrom, lookAt, up, xFov, resolution);
}

PinHoleCamera::PinHoleCamera(const Json& cameraJson) {
   const Json& transform_json = cameraJson.at("transform");
   Vector3f lookAt, lookFrom, up;
   if (transform_json.contains("position")) {
      auto position = transform_json.at("position");
      lookFrom = Vector3f(position[0], position[1], position[2]);
   } else {
      lookFrom = Vector3f(0, 0, 0);
   }

   if (transform_json.contains("look_at")) {
      auto look_at = transform_json.at("look_at");
      lookAt = Vector3f(look_at[0], look_at[1], look_at[2]);
   } else {
      lookAt = Vector3f(0, 1, 0);
   }

   if (transform_json.contains("up")) {
      auto _up = transform_json.at("up");
      up = Vector3f(_up[0], _up[1], _up[2]);
   } else {
      up = Vector3f(0, 0, 1);
   }

   float xFov = 45.f;
   if (cameraJson.contains("xfov")) {
      xFov = cameraJson.at("xfov");
   }

   Vector2i resolution;
   if (cameraJson.contains("resolution")) {
      auto _resolution = cameraJson.at("resolution");
      resolution = Vector2i(_resolution[0], _resolution[1]);
   } else {
      resolution = Vector2i(512, 512);
   }

   setActualValue(lookFrom, lookAt, up, xFov, resolution);
}

void PinHoleCamera::setActualValue(Point3f lookFrom, Point3f lookAt,
                                   Vector3f up, float xFov,
                                   Vector2i resolution) {
   float aspectRatio = float(resolution[0]) / resolution[1];
   float distToFilm = 1.0f / tan(xFov * Transform::PI_F / 360);
   view = Transform::getLookAt(lookFrom, lookAt - lookFrom, up);
   projection = Transform::getPerspective(
       Transform::AngleDegreeValue(xFov), aspectRatio, distToFilm,
       std::numeric_limits<float>::max() / 2.f);
   viewPort = Transform::getViewPort(resolution);
   worldToScreen = viewPort * projection * view;

   film = std::make_unique<RGBColorImage>(resolution[0], resolution[1]);
   zBuffer = std::make_unique<ZBuffer>(resolution[0], resolution[1]);

   right = view.row(0).head<3>();
   up = view.row(1).head(3);
   lookAt = -view.row(2).head<3>();
}