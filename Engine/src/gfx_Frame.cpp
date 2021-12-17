#include "gfx_Frame.h"

namespace gfx {
  Frame::~Frame() = default;

  void Frame::addMesh(std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> material) {
    elements.push_back({mesh, material, matrixCurrent});
  }
}