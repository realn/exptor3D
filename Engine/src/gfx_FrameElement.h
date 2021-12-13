#pragma once

#include <glm/matrix.hpp>

#include "gfx_Mesh.h"
#include "gfx_Material.h"

namespace gfx {
  class FrameElement {
  public:
    std::shared_ptr<Mesh> mesh;
    std::shared_ptr<Material> material;
    glm::mat4 modelview = glm::mat4(1.0f);
  };
}
