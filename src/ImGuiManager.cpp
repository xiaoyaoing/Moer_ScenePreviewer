#include "ImGuiManager.h"
#ifndef IMGUI_DEFINE_MATH_OPERATORS
#define IMGUI_DEFINE_MATH_OPERATORS
#endif
#include <ImGuiFileDialog.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include "CMakeConfig.h"
#include "IconsFontAwesome6.h"
#include "imgui.h"
#include "imgui_internal.h"

#define ICON_FA_FILE_IMPORT_ "\xee\x89\xbd"  // U+e27d
#define ICON_FA_CLOSE "\xee\xa9\xb6"         // U+ea76
#define ICON_FA_GITHUB "\xEF\x82\x9B"        // U+f09b

static const char* uiName_Scene = ICON_FA_CUBES " Scene";
static const char* uiName_Open = ICON_FA_FOLDER " Open";
static const char* uiName_Save = ICON_FA_FILE_IMPORT_ " Save";
static const char* uiName_Quit = ICON_FA_CLOSE " Quit";
static const char* uiName_Camera = ICON_FA_CAMERA " Camera";
static const char* uiName_Light = ICON_FA_LIGHTBULB " Light";
static const char* uiName_Material = ICON_FA_CUBE " Material";

static float cameraMovementSpd = 0.05f;
static float mouseSensitivity = 0.0005f;
static float zoomSensitivity = 0.6f;
static bool fixLookAtPoint = true;
static bool lightFollow = true;
static bool enableMovement = false;

static void HelpMarker(const char* desc) {
   ImGui::TextDisabled("(?)");
   if (ImGui::BeginItemTooltip()) {
      ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
      ImGui::TextUnformatted(desc);
      ImGui::PopTextWrapPos();
      ImGui::EndTooltip();
   }
}

enum class FileDialogAction { CLOSE, OPEN, SAVE, SAVE_AS };
// ---------------------Main Menu Bar-----------------------------
static void showMainMenuBar(std::shared_ptr<Scene> scene);
static void showFileMenu(std::shared_ptr<Scene> scene);
static void showFileDialog(std::shared_ptr<Scene> scene);
// --------------------Windows------------------------------------
static void showResultWindow(std::shared_ptr<Scene> scene,
                             GLuint renderResultTextureId);
static void showScenePropertiesWindow(std::shared_ptr<Scene> scene);
static void showLightPropertiesWindow(std::shared_ptr<Scene> scene);
static void showMaterialPropertiesWindow(std::shared_ptr<Scene> scene);
static void showCameraPropertiesWindow(std::shared_ptr<Scene> scene);

static void showHelpWindow();
static void showAboutWindw();
static FileDialogAction fileDialogCurrentAction;
static bool openHelpWindow = false;
static bool openAboutWindow = false;

static std::string fullScenePath = ".";

static std::string workingDir = ".";
static void handleControl(std::shared_ptr<Scene> scene);
static void handleKeyboardControl(std::shared_ptr<Scene> scene);
static void handleMouseControl(std::shared_ptr<Scene> scene);

static ImGuiID mainDockSpace, leftDockSpace, rightDockSpace;

static void setupDefaultLayout() {
   ImGui::DockBuilderSplitNode(mainDockSpace, ImGuiDir_Left, 0.4,
                               &leftDockSpace, &rightDockSpace);
   ImGui::DockBuilderDockWindow(uiName_Scene, leftDockSpace);
   ImGui::DockBuilderDockWindow(uiName_Camera, leftDockSpace);
   ImGui::DockBuilderDockWindow(uiName_Light, leftDockSpace);
   ImGui::DockBuilderDockWindow(uiName_Material, leftDockSpace);
   ImGui::DockBuilderDockWindow("Result", rightDockSpace);
}

void ImGuiManager::render(std::shared_ptr<Scene> scene,
                          GLuint renderResultTextureId) {
   static bool first_time = true;
   mainDockSpace = ImGui::DockSpaceOverViewport(0,ImGui::GetMainViewport());
   bool isSettingLoaded = ImGui::GetCurrentContext()->SettingsIniData.empty();
   if (first_time && isSettingLoaded) {
      setupDefaultLayout();
      first_time = false;
   }
   showMainMenuBar(scene);
   showScenePropertiesWindow(scene);
   showCameraPropertiesWindow(scene);
   showLightPropertiesWindow(scene);
   showMaterialPropertiesWindow(scene);
   showResultWindow(scene, renderResultTextureId);
   showHelpWindow();
   showAboutWindw();
   if (fileDialogCurrentAction != FileDialogAction::CLOSE) {
      showFileDialog(scene);
   }
   handleControl(scene);
}

static void showFileDialog(std::shared_ptr<Scene> scene) {
   ImGuiIO& io = ImGui::GetIO();
   ImVec2 maxSize, minSize;
   maxSize.y = io.DisplaySize.y * 0.8f;
   maxSize.x = io.DisplaySize.x * 0.8f;
   minSize.x = maxSize.x * 0.4f;
   minSize.y = maxSize.y * 0.4f;

   if (fileDialogCurrentAction == FileDialogAction::OPEN) {
      ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Choose File",
                                              ".json", fullScenePath);
   } else if (fileDialogCurrentAction == FileDialogAction::SAVE) {
      ImGuiFileDialog::Instance()->OpenDialog(
          "ChooseFileDlgKey", " Choose a File", ".json", fullScenePath, 1,
          nullptr, ImGuiFileDialogFlags_ConfirmOverwrite);
   } else if (fileDialogCurrentAction == FileDialogAction::SAVE_AS) {
      std::string newFullScenePath = fullScenePath;
      size_t pos = newFullScenePath.rfind('.');
      if (pos != std::string::npos) {
         newFullScenePath.insert(pos, "_new");
      }
      ImGuiFileDialog::Instance()->OpenDialog(
          "ChooseFileDlgKey", " Choose a File", ".json", newFullScenePath, 1,
          nullptr, ImGuiFileDialogFlags_ConfirmOverwrite);
   }

   enableMovement = false;

   if (ImGuiFileDialog::Instance()->Display(
           "ChooseFileDlgKey", ImGuiWindowFlags_None, minSize, maxSize)) {
      if (ImGuiFileDialog::Instance()->IsOk()) {
         fullScenePath = ImGuiFileDialog::Instance()->GetFilePathName();
         workingDir = ImGuiFileDialog::Instance()->GetCurrentPath();
         if (fileDialogCurrentAction == FileDialogAction::SAVE ||
             fileDialogCurrentAction == FileDialogAction::SAVE_AS) {
            scene->saveScene(fullScenePath);
         } else if (fileDialogCurrentAction == FileDialogAction::OPEN) {
            scene->loadScene(fullScenePath, workingDir);
         } else {
            assert(0);
         }
      }
      ImGuiFileDialog::Instance()->Close();
      fileDialogCurrentAction = FileDialogAction::CLOSE;
      enableMovement = true;
   }
}

static void showMainMenuBar(std::shared_ptr<Scene> scene) {
   if (ImGui::BeginMainMenuBar()) {
      if (ImGui::BeginMenu("File")) {
         showFileMenu(scene);
         ImGui::EndMenu();
      }
      if (ImGui::MenuItem("Help")) {
         openHelpWindow = true;
      }
      if (ImGui::MenuItem("About")) {
         openAboutWindow = true;
      }
      ImGui::EndMainMenuBar();
   }
}

static void showHelpWindow() {
   if (openHelpWindow) {
      ImGui::Begin("Help", &openHelpWindow);
      ImGui::Text("Use W,A,S,D,Q,E to move camera.");
      ImGui::Text(
          "Press the middle mouse button and move the mouse to look around.");
      ImGui::Text(
          "Hold down LeftCtrl and scroll the mouse wheel to zoom in or out.");
      ImGui::End();
   }
}

static void showAboutWindw() {
   if (openAboutWindow) {
      ImGui::Begin("About", &openAboutWindow);
      ImGui::Text("╔╦╗┌─┐┌─┐┬─┐  ╔═╗┌─┐┌─┐┌┐┌┌─┐  ╔═╗┬─┐┌─┐┬  ┬┬┌─┐┬ ┬┌─┐┬─┐");
      ImGui::Text("║║║│ │├┤ ├┬┘  ╚═╗│  ├┤ │││├┤   ╠═╝├┬┘├┤ └┐┌┘│├┤ │││├┤ ├┬┘");
      ImGui::Text("╩ ╩└─┘└─┘┴└─  ╚═╝└─┘└─┘┘└┘└─┘  ╩  ┴└─└─┘ └┘ ┴└─┘└┴┘└─┘┴└─");
      ImGui::Text("Moer Scene Previewer");
      ImGui::Text("Version %d.%d", VERSION_MAJOR, VERSION_MINOR);
      ImGui::Text(ICON_FA_GITHUB
                  " https://github.com/r1ckhu/Moer_ScenePreviewer");
      ImGui::SeparatorText("Developer");
      ImGui::Text("Ruike Hu");
      ImGui::End();
   }
}

static void showCameraPropertiesWindow(std::shared_ptr<Scene> scene) {
   ImGui::Begin(uiName_Camera);
   auto camera = scene->camera;
   if (camera != nullptr) {
      ImGui::SeparatorText("Camera Actions");
      if (ImGui::Button("Reset Camera")) {
         camera->resetToInitValue();
      }
      if (ImGui::Button("Reset Position")) {
         camera->resetPosition();
      }
      if (ImGui::Button("Reset LookAt Point")) {
         camera->resetLookAtPoint();
      }
      if (ImGui::Button("Reset xFov")) {
         camera->resetXFov();
      }
      ImGui::SeparatorText("Camera Properties");
      ImGui::InputFloat3("Camera Position", camera->cameraPosition.data());
      ImGui::InputFloat3("LookAt Point", camera->pointLookAt.data());
      ImGui::InputFloat3("Up Vector", camera->up.data());
      ImGui::InputFloat("xFov", &camera->xFov, 0.1f, 1.0f, "%.3f");
      ImGui::SameLine();
      HelpMarker(
          "Holding down the Ctrl key will speed up the increment and "
          "decrement");

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
   } else {
      ImGui::Text("No active scene now.");
   }
   ImGui::End();
}

static void showLightPropertiesWindow(std::shared_ptr<Scene> scene) {
   ImGui::Begin(uiName_Light);
   if (scene->camera != nullptr) {
      ImGui::SeparatorText("Light Actions");
      if (ImGui::Button("Reset Light")) {
         scene->light = DefaultWhiteLight();
         scene->light.position = scene->camera->cameraPosition;
      }
      Light& light = scene->light;
      ImGui::ColorEdit3("Light Ambient", (float*)light.ambient.data(),
                        ImGuiColorEditFlags_Float);
      ImGui::SameLine();
      HelpMarker(
          "Click on the color square to open a color picker.\n"
          "Click and hold to use drag and drop.\n"
          "Right-click on the color square to show options.\n"
          "CTRL+click on individual component to input value.\n");
      ImGui::ColorEdit3("Light Diffuese", (float*)light.diffuse.data(),
                        ImGuiColorEditFlags_Float);
      ImGui::ColorEdit3("Light Specular", (float*)light.specular.data(),
                        ImGuiColorEditFlags_Float);
   } else {
      ImGui::Text("No active scene now.");
   }
   ImGui::End();
}

static void showMaterialPropertiesWindow(std::shared_ptr<Scene> scene) {
   ImGui::Begin(uiName_Material);
   if (scene->camera != nullptr) {
      ImGui::SeparatorText("Material Actions");
      if (ImGui::Button("Reset Material")) {
         scene->material = DefaultWhiteMaterial();
      }
      Material& material = scene->material;
      ImGui::ColorEdit3("Material Ambient", (float*)material.ambient.data(),
                        ImGuiColorEditFlags_Float);
      ImGui::SameLine();
      HelpMarker(
          "Click on the color square to open a color picker.\n"
          "Click and hold to use drag and drop.\n"
          "Right-click on the color square to show options.\n"
          "CTRL+click on individual component to input value.\n");
      ImGui::ColorEdit3("Material Diffuese", (float*)material.diffuse.data(),
                        ImGuiColorEditFlags_Float);
      ImGui::ColorEdit3("Material Specular", (float*)material.specular.data(),
                        ImGuiColorEditFlags_Float);
      ImGui::InputFloat("Material Shininess", &material.shininess);
   } else {
      ImGui::Text("No active scene now.");
   }
   ImGui::End();
}

static void showFileMenu(std::shared_ptr<Scene> scene) {
   if (ImGui::MenuItem(uiName_Open)) {
      if (fileDialogCurrentAction == FileDialogAction::CLOSE) {
         fileDialogCurrentAction = FileDialogAction::OPEN;
      }
   }
   bool hasActiveScene = scene->camera != nullptr;
   if (ImGui::MenuItem(uiName_Save, NULL, (bool*)NULL, hasActiveScene)) {
      if (fileDialogCurrentAction == FileDialogAction::CLOSE) {
         fileDialogCurrentAction = FileDialogAction::SAVE;
      }
   }
   if (ImGui::MenuItem("  Save As...", NULL, (bool*)NULL, hasActiveScene)) {
      if (fileDialogCurrentAction == FileDialogAction::CLOSE) {
         fileDialogCurrentAction = FileDialogAction::SAVE_AS;
      }
   }
   if (ImGui::MenuItem(uiName_Quit)) {
      exit(0);
   }
}

static void showResultWindow(std::shared_ptr<Scene> scene,
                             GLuint renderResultTextureId) {
   ImGui::Begin("Result");
   ImVec2 availableSize = ImGui::GetContentRegionAvail();
   float aspectRatio = static_cast<float>(scene->width) / scene->height;
   float minWidth = std::min(availableSize.x, availableSize.y * aspectRatio);
   float minHeight = minWidth / aspectRatio;
   ImGui::Image(reinterpret_cast<void*>(renderResultTextureId),
                ImVec2{minWidth, minHeight}, ImVec2{0, 1}, ImVec2{1, 0});
   ImGui::End();
}
static void showScenePropertiesWindow(std::shared_ptr<Scene> scene) {
   ImGui::Begin(uiName_Scene);
   std::shared_ptr<PinHoleCamera> camera = scene->camera;
   if (camera == nullptr) {
      ImGui::Text("No active scene now.");
      ImGui::End();
      return;
   }

   ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
               1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
   ImGui::Text("Current scene: %s", fullScenePath.c_str());
   ImGui::Text("Load time: %lld ms", scene->loadDurationMs.count());
   ImGui::Text("%d vertices, %d indices (%d trangles)",
               scene->getTotalVertices(), scene->getTotalIndices(),
               scene->getTotalIndices() / 3);
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
      // FIXME: When moving the mouse in circle, the camera will roll
      ImGui::SetMouseCursor(ImGuiMouseCursor_None);
      float xoffset = io.MouseDelta.x;
      float yoffset = io.MouseDelta.y;
      float deltaYaw = -xoffset * mouseSensitivity;
      float deltaPitch = -yoffset * mouseSensitivity;

      camera->adjustYawAndPitch(deltaYaw, deltaPitch);
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

   if (scene->camera) {
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
}