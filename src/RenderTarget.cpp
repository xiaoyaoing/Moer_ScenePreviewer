#include "RenderTarget.h"

namespace Render {
void setRenderTarget(std::shared_ptr<Mesh> mesh,
                     std::shared_ptr<PinHoleCamera> camera,
                     std::shared_ptr<PinHoleCamera> lightCamera){
   currentMesh = mesh;
   currentCamera = camera;
   currentLightCamera = lightCamera;
}

};  // namespace Render