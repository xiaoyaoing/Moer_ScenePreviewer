#include "ImGuiManager.h"

#include <ImGuiFileDialog.h>

static float cameraMovementSpd = 0.05f;
static float mouseSensitivity = 0.0005f;
static float zoomSensitivity = 0.6f;
static bool fixLookAtPoint = true;
static bool lightFollow = true;
static bool enableMovement = true;

static void HelpMarker(const char* desc) {
   ImGui::TextDisabled("(?)");
   if (ImGui::BeginItemTooltip()) {
      ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
      ImGui::TextUnformatted(desc);
      ImGui::PopTextWrapPos();
      ImGui::EndTooltip();
   }
}

static void showLoadSceneWindow(std::shared_ptr<Scene> scene);
static void showScenePropertiesWindow(std::shared_ptr<Scene> scene);
static void handleControl(std::shared_ptr<Scene> scene);
static void handleKeyboardControl(std::shared_ptr<Scene> scene);
static void handleMouseControl(std::shared_ptr<Scene> scene);

void ImGuiManager::render(std::shared_ptr<Scene> scene) {
   showLoadSceneWindow(scene);
   showScenePropertiesWindow(scene);
   handleControl(scene);
}

static void showLoadSceneWindow(std::shared_ptr<Scene> scene) {
   ImGui::Begin("Scene Manager");
   static bool openLoadDialog = false;
   static bool openSaveDialog = false;
   static std::string fullScenePath = ".";
   static std::string workingDir = ".";

   if (ImGui::Button("Load Scene")) {
      openLoadDialog = true;
   }
   if (scene->camera != nullptr) {
      ImGui::SeparatorText("Scene Actions");
      if (ImGui::Button("Save Scene")) {
         openSaveDialog = true;
      }
      ImGui::SeparatorText("Camera Actions");
      if (ImGui::Button("Reset Camera")) {
         scene->camera->resetToInitValue();
      }
      if (ImGui::Button("Reset Position")) {
         scene->camera->resetPosition();
      }
      if (ImGui::Button("Reset LookAt Point")) {
         scene->camera->resetLookAtPoint();
      }
      if (ImGui::Button("Reset xFov")) {
         scene->camera->resetXFov();
      }
      ImGui::SeparatorText("Light & Material Actions");
      if (ImGui::Button("Reset Light")) {
         scene->light = DefaultWhiteLight();
         scene->light.position = scene->camera->cameraPosition;
      }
      if (ImGui::Button("Reset Material")) {
         scene->material = DefaultWhiteMaterial();
      }
   }
   ImGui::End();
   ImGuiIO& io = ImGui::GetIO();
   ImVec2 maxSize;
   maxSize.x = io.DisplaySize.x * 0.8f;
   maxSize.y = io.DisplaySize.y * 0.8f;
   ImVec2 minSize;
   minSize.x = maxSize.x * 0.4f;
   minSize.y = maxSize.y * 0.4f;
   if (openLoadDialog) {
      enableMovement = false;
      ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Choose File",
                                              ".json", fullScenePath);
      if (ImGuiFileDialog::Instance()->Display(
              "ChooseFileDlgKey", ImGuiWindowFlags_None, minSize, maxSize)) {
         if (ImGuiFileDialog::Instance()->IsOk()) {
            fullScenePath = ImGuiFileDialog::Instance()->GetFilePathName();
            workingDir = ImGuiFileDialog::Instance()->GetCurrentPath();
            scene->loadScene(fullScenePath, workingDir);
            openLoadDialog = false;
         }

         ImGuiFileDialog::Instance()->Close();
         enableMovement = true;
         openLoadDialog = false;
      }
   }
   if (openSaveDialog) {
      enableMovement = false;
      ImGuiFileDialog::Instance()->OpenDialog(
          "ChooseFileDlgKey", " Choose a File", ".json", fullScenePath, 1,
          nullptr, ImGuiFileDialogFlags_ConfirmOverwrite);
      if (ImGuiFileDialog::Instance()->Display(
              "ChooseFileDlgKey", ImGuiWindowFlags_None, minSize, maxSize)) {
         if (ImGuiFileDialog::Instance()->IsOk()) {
            fullScenePath = ImGuiFileDialog::Instance()->GetFilePathName();
            workingDir = ImGuiFileDialog::Instance()->GetCurrentPath();
            scene->saveScene(fullScenePath);
            openSaveDialog = false;
         }

         ImGuiFileDialog::Instance()->Close();
         enableMovement = true;
         openSaveDialog = false;
      }
   }
}

static void showScenePropertiesWindow(std::shared_ptr<Scene> scene) {
   ImGui::Begin("Scene Properties");
   std::shared_ptr<PinHoleCamera> camera = scene->camera;
   if (camera == nullptr) {
      ImGui::Text("No active scene now.");
      return;
   }
   if (ImGui::TreeNode("Camera Properties")) {
      ImGui::InputFloat3("Camera Position", camera->cameraPosition.data());
      ImGui::InputFloat3("LookAt Point", camera->pointLookAt.data());
      ImGui::InputFloat3("Up Vector", camera->up.data());
      ImGui::InputFloat("xFov", &camera->xFov, 0.1f, 1.0f, "%.3f");
      ImGui::SameLine();
      HelpMarker(
          "Holding down the Ctrl key will speed up the increment and "
          "decrement");
      ImGui::TreePop();
   }
   if (ImGui::TreeNode("Light Properties")) {
      Light& light = scene->light;
      ImGui::DragFloat3("Light Ambient", light.ambient.data(), 0.01f, 0.0f,
                        1.0f);
      ImGui::DragFloat3("Light Diffuse", light.diffuse.data(), 0.01f, 0.0f,
                        1.0f);
      ImGui::DragFloat3("Light Specular", light.specular.data(), 0.01f, 0.0f,
                        1.0f);
      ImGui::TreePop();
   }
   if (ImGui::TreeNode("Material Properties")) {
      Material& material = scene->material;
      ImGui::DragFloat3("Material Ambient", material.ambient.data(), 0.01f,
                        0.0f, 1.0f);
      ImGui::DragFloat3("Material Diffuse", material.diffuse.data(), 0.01f,
                        0.0f, 1.0f);
      ImGui::DragFloat3("Material Specular", material.specular.data(), 0.01f,
                        0.0f, 1.0f);
      ImGui::InputFloat("Material Shininess", &material.shininess);
      ImGui::TreePop();
   }

   if (ImGui::TreeNode("Movement & Control Properties")) {
      ImGui::SeparatorText("Control Guide");
      ImGui::Text("Use W,A,S,D,Q,E to move camera.");
      ImGui::Text(
          "Press the middle mouse button and move the mouse to look around.");
      ImGui::Text(
          "Hold down LeftCtrl and scroll the mouse wheel to zoom in or out.");
      ImGui::SeparatorText("Values");
      ImGui::Checkbox("Fix LookAt Point", &fixLookAtPoint);
      ImGui::SameLine();
      HelpMarker(
          "Fix the point the camera look at.\n"
          "[Warn!]After using the mouse to look around, the look at point will "
          "change dramatically.");
      ImGui::Checkbox("Let Light Follow Camera", &lightFollow);
      ImGui::InputFloat("Camera Movement Speed", &cameraMovementSpd, 0.001f,
                        0.01f, "%.3f");
      ImGui::InputFloat("Mouse Sensitivity", &mouseSensitivity, 0.0001f, 0.001f,
                        "%.3f");
      ImGui::InputFloat("Zoom Sensitivity", &zoomSensitivity, 0.1f, 1.0f,
                        "%.3f");

      ImGui::TreePop();
   }

   ImGui::End();
}

static void handleControl(std::shared_ptr<Scene> scene) {
   if (enableMovement) {
      handleKeyboardControl(scene);
      handleMouseControl(scene);
   }
}

static void handleMouseControl(std::shared_ptr<Scene> scene) {
   std::shared_ptr<PinHoleCamera> camera = scene->camera;
   ImGuiIO& io = ImGui::GetIO();
   if (ImGui::IsMouseDown(ImGuiMouseButton_Middle)) {
      // FIXME: When moving the mouse in circle, the camera will rotate
      ImGui::SetMouseCursor(ImGuiMouseCursor_None);

      float yaw = mouseSensitivity * io.MouseDelta.x;
      float pitch = mouseSensitivity * io.MouseDelta.y;

      Eigen::AngleAxisf yawRotation(yaw, camera->up);
      camera->vecLookAt = yawRotation * camera->vecLookAt;

      camera->right = camera->vecLookAt.cross(camera->up);

      Eigen::AngleAxisf pitchRotation(pitch, camera->right);
      camera->vecLookAt = pitchRotation * camera->vecLookAt;

      camera->up = camera->right.cross(camera->vecLookAt);

      camera->up.normalize();
      camera->right.normalize();
      camera->vecLookAt.normalize();

      camera->pointLookAt = camera->cameraPosition + camera->vecLookAt;
   }

   if (io.MouseWheel != 0.f && ImGui::IsKeyDown(ImGuiKey_LeftCtrl)) {
      camera->xFov -= io.MouseWheel * zoomSensitivity;
   }
}

static void handleKeyboardControl(std::shared_ptr<Scene> scene) {
   std::shared_ptr<PinHoleCamera> camera = scene->camera;
   static const ImGuiKey concernedKeys[] = {ImGuiKey_W, ImGuiKey_A, ImGuiKey_S,
                                            ImGuiKey_D, ImGuiKey_Q, ImGuiKey_E};
   static const float forward[] = {1.f, 0.f, -1.f, 0.f, 0.f, 0.f};
   static const float right[] = {0.f, -1.f, 0.f, 1.f, 0.f, 0.f};
   static const float up[] = {0.f, 0.f, 0.f, 0.f, 1.f, -1.f};

   constexpr size_t concernedKeys_nr =
       sizeof(concernedKeys) / sizeof(concernedKeys[0]);
   constexpr size_t forward_nr = sizeof(forward) / sizeof(forward[0]);
   constexpr size_t right_nr = sizeof(right) / sizeof(right[0]);
   constexpr size_t up_nr = sizeof(up) / sizeof(up[0]);

   static_assert(concernedKeys_nr == forward_nr, "Arrary length not equal");
   static_assert(right_nr == up_nr, "Arrary length not equal");
   static_assert(right_nr == forward_nr, "Arrary length not equal");

   for (int i = 0; i < concernedKeys_nr; i++) {
      if (ImGui::IsKeyDown(concernedKeys[i])) {
         Vector3f rightDelta = cameraMovementSpd * right[i] * camera->right;
         Vector3f forwardDelta =
             cameraMovementSpd * forward[i] * camera->vecLookAt;
         Vector3f upDelta = cameraMovementSpd * up[i] * camera->up;
         camera->cameraPosition += rightDelta;
         camera->cameraPosition += forwardDelta;
         camera->cameraPosition += upDelta;

         if (!fixLookAtPoint) {
            camera->pointLookAt += rightDelta;
            camera->pointLookAt += forwardDelta;
            camera->pointLookAt += upDelta;
         }

         if (lightFollow) {
            scene->light.position = camera->cameraPosition;
         }
      }
   }
}