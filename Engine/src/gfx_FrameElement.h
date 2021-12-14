#pragma once

#include <glm/matrix.hpp>

#include "gfx_BufferedMesh.h"
#include "gfx_Material.h"

namespace gfx {
  class FrameElement {
  public:
    std::shared_ptr<BufferedMesh> mesh;
    std::shared_ptr<Material> material;
    glm::mat4 modelview = glm::mat4(1.0f);
  };
}
