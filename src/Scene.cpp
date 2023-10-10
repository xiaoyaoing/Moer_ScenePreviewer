#include "Scene.h"

Scene::Scene(std::string& working_dir) {
   std::string jsonPath = working_dir + "/scene.json";
   std::ifstream file(jsonPath);
   Json SceneJson;
   if (file.is_open()) {
      file >> SceneJson;
      file.close();
   }
   load_meshes_from_json(SceneJson);
   load_camera_from_json(SceneJson);
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
         mesh = std::make_shared<Mesh>(file_path);
      } else if (type == "quad") {
         mesh = std::make_shared<Quad>();
      } else if (type == "cube") {
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

Matrix4f Scene::getTransform(const Json& json) {
   Matrix4f ret = Matrix4f::Identity();
   try {
      if (json.contains("translate")) {
         auto translate = json.at("translate");
         ret *=
             Transform::getTranslate(translate[0], translate[1], translate[2]);
      }
      if (json.contains("scale")) {
         auto scale = json.at("scale");
         ret *= Transform::getScale(scale[0], scale[1], scale[2]);
      }
      if (json.contains("rotation")) {
         auto rotation = json.at("rotation");
         float x = rotation[0], y = rotation[1], z = rotation[2];
         ret *= Transform::getRotateEuler(
             Transform::AngleDegreeValue(x), Transform::AngleDegreeValue(y),
             Transform::AngleDegreeValue(z), Transform::EulerType::EULER_YZX);
      }
      return ret;
   } catch (const std::out_of_range& e) {
      std::cerr
          << "Missing essential property when loading entity's transform:  "
          << e.what() << std::endl;
   }
}

void Scene::load_camera_from_json(const Json& sceneJson) {
   Json cameraJson = sceneJson.at("camera");
   camera = std::make_shared<PinHoleCamera>(cameraJson);
}

void Scene::render() {
   for (auto& mesh : meshes) {
      for (int i = 0; i < mesh->faces_nr(); i++) {
         std::vector<Vector4f> screen_coords(3);
         for (int j = 0; j < 3; j++) {
            screen_coords[j] = shader->vertex(mesh, i, j);
         }
         Render::triangle(screen_coords, *shader, *camera->zBuffer, *camera->film);
      }
   }
}