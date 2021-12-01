#pragma once

#include <vector>

#include <CBCore/Defines.h>
#include <CBGL/Buffer.h>

#include "MeshVertex.h"

namespace gfx {
  class Mesh {
  public:
    using vertices_t = std::vector<MeshVertex>;
    using indices_t = std::vector<cb::u16>;

    void add(glm::vec3 position, glm::vec3 normal = glm::vec3(), glm::vec2 texCoord = glm::vec2());
    void add(const MeshVertex& vertex);

    void prepare();

    void render();

  protected:
    vertices_t vertices;
    indices_t indices;

    std::unique_ptr<cb::gl::Buffer> vertexBuffer;
    std::unique_ptr<cb::gl::Buffer> indexBuffer;
  };
}