#pragma once

#include "gfx_MatrixStack.h"
#include "gfx_FrameElement.h"

namespace gfx {
  class Frame : public MatrixStack {
  public:
    ~Frame() override;

    using elements_t = std::vector<FrameElement>;

    void addMesh(std::shared_ptr<BufferedMesh> mesh, std::shared_ptr<Material> material);

    glm::uvec2 size = glm::vec2(1);
    elements_t elements;

  private:

  };
}