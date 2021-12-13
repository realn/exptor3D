#pragma once

#include "gfx_FrameElement.h"

namespace gfx {
  class Frame {
  public:

    void addMesh(glm::mat4 modelview, std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> material);

    using elements_t = std::vector<FrameElement>;

    glm::uvec2 size = glm::vec2(1);
    glm::mat4 projection = glm::mat4(1.0f);
    elements_t elements;
  };
}