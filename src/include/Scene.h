#ifndef __SCENE_H__
#define __SCENE_H__
#include <fstream>
#include <iostream>
#include <memory>
#include <nlohmann/json.hpp>

#include "Camera.h"
#include "FrameBuffer.h"
#include "Light.h"
#include "Material.h"
#include "Mesh.h"
#include "Render.h"
#include "Shader.h"
#include "Transform.h"
#include "VertexArrayObject.h"

using Json = nlohmann::json;

class Scene {
  public:
   std::shared_ptr<PinHoleCamera> camera;
   std::vector<std::shared_ptr<Mesh>> meshes;

   std::vector<VertexArrayObject> VAOs;
   QuadVertexArrayObject quadVAO;

   FrameBuffer framebuffer;
   Shader shader;
   ScreenShader screenShader;
   DefaultWhiteMaterial material;
   DefaultWhiteLight light;

  public:
   Scene(std::string& working_dir);
   void render();

  private:
   void LoadMeshesFronJson(const Json& sceneJson);
   void LoadSingleMeshFromJson(const Json& entityJson);
   void LoadCameraFromJson(const Json& sceneJson);
   void createVAOsFromMeshes();
   Matrix4f getTransform(const Json& json);
   std::string workingDir;
};

#endif