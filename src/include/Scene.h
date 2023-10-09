#ifndef __SCENE_H__
#define __SCENE_H__
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <memory>

#include "Camera.h"
#include "Mesh.h"
#include "Transform.h"


using Json = nlohmann::json;

class Scene {
  public:
   Scene(std::string& working_dir);
   Vector3f light_dir;
   std::unique_ptr<PinHoleCamera> camera;
   std::vector<std::shared_ptr<Mesh>> meshes;

   void render();

  private:
   void load_meshes_from_json(const Json& sceneJson);
   void load_mesh_from_json(const Json& entityJson);
   Matrix4f getTransform(const Json& json);
   void load_camera_from_json(const Json& sceneJson);
};

#endif