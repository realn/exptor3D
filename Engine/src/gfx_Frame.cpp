#include "gfx_Frame.h"

namespace gfx {
  void Frame::addMesh(glm::mat4 modelview, std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> material) {
    elements.push_back({mesh, material, modelview});
  }
}