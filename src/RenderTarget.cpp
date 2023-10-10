#include "RenderTarget.h"
namespace Render {
std::shared_ptr<Mesh> currentMesh;
std::shared_ptr<PinHoleCamera> currentCamera;
std::shared_ptr<PinHoleCamera> currentLightCamera;
void setRenderTarget(std::shared_ptr<Mesh> mesh,
                     std::shared_ptr<PinHoleCamera> camera,
                     std::shared_ptr<PinHoleCamera> lightCamera) {
   currentMesh = mesh;
   currentCamera = camera;
   currentLightCamera = lightCamera;
}

};  // namespace Render