#pragma once

#include "core_Id.h"
#include "gfx_BufferedMesh.h"

namespace gui {
  class RenderContext;
}

namespace gfx {
  class Frame;

  class RenderSystem {
  public:
    void render(const Frame& frame);
    void render(const gui::RenderContext& ctx);

  private:
    using meshptr_t = std::shared_ptr<BufferedMesh>;
    using meshes_t = std::map<core::Id, meshptr_t>;

    meshptr_t getBufferedMesh(const Mesh& mesh);

    meshes_t bufferedMeshes;
  };
}