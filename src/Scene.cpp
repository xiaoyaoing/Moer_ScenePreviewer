#include "Scene.h"

Scene::Scene(std::string& working_dir) {
   workingDir = working_dir;
   std::string jsonPath = workingDir + "scene.json";
   std::ifstream file(jsonPath);
   Json SceneJson;
   if (file.is_open()) {
      file >> SceneJson;
      file.close();
   }
   load_meshes_from_json(SceneJson);
   load_camera_from_json(SceneJson);
   create_light_camera();
   Render::currentCamera = camera;
   Render::currentLightCamera = lightCamera;
   shader = std::make_unique<ShadowMappingShader>();
}

void Scene::load_meshes_from_json(const Json& SceneJson) {
   for (auto& entity : SceneJson.at("entities")) {
      load_mesh_from_json(entity);
   }
}

void Scene::load_mesh_from_json(const Json& entityJson) {
   try {
      std::string type = entityJson.at("type");
      std::shared_ptr<Mesh> mesh = nullptr;
      if (type == "mesh") {
         std::string file_path = entityJson.at("file");
#ifdef DEBUG
         std::cout << "Load mesh at " << file_path << std::endl;
#endif
         mesh = std::make_shared<Mesh>(workingDir + file_path);
      } else if (type == "quad") {
#ifdef DEBUG
         std::cout << "Load quad." << std::endl;
#endif
         mesh = std::make_shared<Quad>();
      } else if (type == "cube") {
#ifdef DEBUG
         std::cout << "Load cube." << std::endl;
#endif
         mesh = std::make_shared<Cube>();
      } else {
         throw std::invalid_argument("Unknown value for key 'type': " + type);
      }

      if (!entityJson.at("transform").empty()) {
         auto transform = getTransform(entityJson.at("transform"));
         mesh->apply(transform);
      }
      meshes.push_back(mesh);

   } catch (const std::out_of_range& e) {
      std::cerr << "Missing essential property when loading entity:  "
                << e.what() << std::endl;
   } catch (const std::invalid_argument& e) {
      std::cerr << "Invalid argument for keys in entity: " << e.what()
                << std::endl;
   }
}

Matrix4f Scene::getTransform(const Json& transformJson) {
#ifdef DEBUG
   std::cout << "Getting Transform for this entity..." << std::endl;
#endif
   Matrix4f ret = Matrix4f::Identity();
   try {
      if (transformJson.contains("translate")) {
         auto translate = transformJson.at("translate");
         ret *=
             Transform::getTranslate(translate[0], translate[1], translate[2]);
#ifdef DEBUG
         std::cout << "Translate: " << translate[0] << ' ' << translate[1]
                   << ' ' << translate[2] << std::endl;
#endif
      }
      if (transformJson.contains("scale")) {
         auto scale = transformJson.at("scale");
         ret *= Transform::getScale(scale[0], scale[1], scale[2]);
#ifdef DEBUG
         std::cout << "Scale: " << scale[0] << ' ' << scale[1] << ' '
                   << scale[2] << std::endl;
#endif
      }
      if (transformJson.contains("rotation")) {
         auto rotation = transformJson.at("rotation");
         float x = rotation[0], y = rotation[1], z = rotation[2];
         ret *= Transform::getRotateEuler(
             Transform::AngleDegreeValue(x), Transform::AngleDegreeValue(y),
             Transform::AngleDegreeValue(z), Transform::EulerType::EULER_YZX);
#ifdef DEBUG
         std::cout << "Roatation: " << rotation[0] << ' ' << rotation[1] << ' '
                   << rotation[2] << std::endl;
#endif
      }
      return ret;
   } catch (const std::out_of_range& e) {
      std::cerr
          << "Missing essential property when loading entity's transform:  "
          << e.what() << std::endl;
   }
}

void Scene::load_camera_from_json(const Json& sceneJson) {
#ifdef DEBUG
   std::cout << "Loading Camera..." << std::endl;
#endif
   Json cameraJson = sceneJson.at("camera");
   camera = std::make_shared<PinHoleCamera>(cameraJson);
#ifdef DEBUG
   std::cout << "Load camera successfully." << std::endl;
#endif
}

void Scene::create_light_camera() {
#ifdef DEBUG
   std::cout << "Creating lightCamera..." << std::endl;
#endif
   Point3f lookAt, lookFrom;
   Vector3f up;
   lookFrom = Point3f(0, 20.f, 0);
   lookAt = Point3f(0, 0, 0);
   up = Vector3f(0, 0, -1);

   float xFov = 45.f;
   Vector2i resolution;
   resolution.x() = camera->film->getWidth();
   resolution.y() = camera->film->getHeight();
   lightCamera =
       std::make_shared<PinHoleCamera>(lookFrom, lookAt, up, xFov, resolution);
#ifdef DEBUG
   std::cout << "Create lightCamera successfully." << std::endl;
#endif
}

void Scene::render() {
   for (auto& mesh : meshes) {
      Render::setRenderTarget(mesh, camera, lightCamera);
      for (int i = 0; i < mesh->faces_nr(); i++) {
         std::vector<Vector4f> screen_coords(3);
         for (int j = 0; j < 3; j++) {
            screen_coords[j] = shader->vertex(i, j);
         }
         Render::triangle(screen_coords, *shader, *camera->zBuffer,
                          *camera->film);
      }
   }
}