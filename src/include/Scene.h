#ifndef __SCENE_H__
#define __SCENE_H__
#include <fstream>
#include <iostream>
#include <memory>
#include <nlohmann/json.hpp>

#include "Camera.h"
#include "Mesh.h"
#include "Render.h"
#include "Shader.h"
#include "Transform.h"


using Json = nlohmann::json;

class Scene {
  public:
   Scene(std::string& working_dir);
   Vector3f light_dir;
   std::shared_ptr<PinHoleCamera> camera;
   std::vector<std::shared_ptr<Mesh>> meshes;
   std::unique_ptr<Shader> shader;
   void render();

  private:
   void load_meshes_from_json(const Json& sceneJson);
   void load_mesh_from_json(const Json& entityJson);
   Matrix4f getTransform(const Json& json);
   void load_camera_from_json(const Json& sceneJson);
};

#endif