#pragma once

#include <vector>
#include <memory>

#include <CBCore/Defines.h>
#include <CBGL/Buffer.h>

#include "GUIVertex.h"

namespace gfx {
  class Texture;
}

namespace gui {
  class Mesh {
  public:
    using vertices_t = std::vector<Vertex>;

    Mesh(std::shared_ptr<gfx::Texture> texture);
    ~Mesh();

    void add(Vertex vertex);

    const vertices_t& getVertices() const { return vertices; }
    std::shared_ptr<gfx::Texture> getTexture() const { return texture; }

    void render() const;

  private:
    std::shared_ptr<gfx::Texture> texture;
    vertices_t vertices;
    mutable std::shared_ptr<cb::gl::Buffer> buffer;
  };
}