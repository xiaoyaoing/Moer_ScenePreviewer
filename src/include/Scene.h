#ifndef __SCENE_H__
#define __SCENE_H__
#include <nlohmann/json.hpp>

#include "Camera.h"
#include "Mesh.h"
#include "Transform.h"

using Json = nlohmann::json;

class Scene {
  public:
   Scene();
   Scene(std::string& working_dir);
   Vector3f light_dir;
   PinHoleCamera camera;
   std::vector<std::shared_ptr<Mesh>> meshes;

  private:
   void load_meshes_from_json(const Json& json);
   void load_mesh_from_json(const Json& json);
   Matrix4f getTransform(const Json& json);
   void load_camera_from_json(const Json& json);
};

#endif