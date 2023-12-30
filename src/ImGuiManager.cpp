#include "ImGuiManager.h"

#include <ImGuiFileDialog.h>

static void showLoadSceneWindow(std::shared_ptr<Scene> scene);

void ImGuiManager::render(std::shared_ptr<Scene> scene) {
   showLoadSceneWindow(scene);
}

static void showLoadSceneWindow(std::shared_ptr<Scene> scene) {
   ImGui::Begin("Load Scene");
   static bool openFileDialog = false;
   if (ImGui::Button("Select Scene")) {
      openFileDialog = true;
   }
   if (openFileDialog) {
      // open Dialog Simple
      ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Choose File",
                                              ".json", ".");

      // display
      if (ImGuiFileDialog::Instance()->Display("ChooseFileDlgKey")) {
         // action if OK
         if (ImGuiFileDialog::Instance()->IsOk()) {
            std::string filePathName =
                ImGuiFileDialog::Instance()->GetFilePathName();
            std::string filePath =
                ImGuiFileDialog::Instance()->GetCurrentPath();
            std::cout << "FilePathName: " << filePathName << std::endl;
            std::cout << "FilePath: " << filePath << std::endl;
            scene->loadScene(filePathName, filePath);
            // action
            openFileDialog = false;
         }

         // close
         ImGuiFileDialog::Instance()->Close();
         openFileDialog = false;
      }
   }
   ImGui::End();
}