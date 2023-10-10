#ifndef __RENDER_TARGET_H__
#define __RENDER_TARGET_H__
#include "Camera.h"
#include "Mesh.h"

namespace Render {
extern std::shared_ptr<Mesh> currentMesh;
extern std::shared_ptr<PinHoleCamera> currentCamera;
extern std::shared_ptr<PinHoleCamera> currentLightCamera;
void setRenderTarget(std::shared_ptr<Mesh> mesh,
                     std::shared_ptr<PinHoleCamera> camera,
                     std::shared_ptr<PinHoleCamera> lightCamera);
};  // namespace Render

#endif
