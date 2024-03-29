#pragma once
#include <atomic>
#include <filesystem>
#include <nlohmann/json.hpp>
#include <optional>

#include "OpenGLHeader.h"

using Json = nlohmann::json;

enum class MOER_STATE { NONE, RENDERING, RENDER_DONE };

class MoerHandler {
  public:
   MoerHandler();
   bool isRendering();
   bool isRenderCompleted();
   int getRenderProgress();
   int executeMoer(const std::filesystem::path& moerPath,
                   const std::filesystem::path& scenePath);
   void setRenderResultPicture(const std::filesystem::path picturePath);
   void setRenderResultPicture();
   std::optional<unsigned int> getRenderResultTextureID();
   std::string getMoerPath();
   void setMoerPathAndWrite(const std::string& moerPath);
   std::filesystem::path getLatestHdrFile(
       const std::filesystem::path& directory);

  private:
   void handleMoerStdOutput(struct subprocess_s* moerProcess);

  private:
   std::atomic<MOER_STATE> moerState;
   std::atomic<int> renderProgress;
   GLuint imageTextureID;
   Json moerSettings;
};