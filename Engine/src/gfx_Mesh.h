#pragma once

#include <vector>

#include <CBCore/Defines.h>
#include <CBGL/Buffer.h>

#include "gfx_MeshVertex.h"

namespace gfx {
  class Mesh {
  public:
    using vertices_t = std::vector<MeshVertex>;
    using indices_t = std::vector<cb::u16>;

    Mesh() = default;
    ~Mesh() = default;

    void add(glm::vec3 position, glm::vec3 normal = glm::vec3(), glm::vec2 texCoord = glm::vec2());
    void add(const MeshVertex& vertex);
    void add(const Mesh& mesh);

    const vertices_t& getVertices() const;
    const indices_t& getIndices() const;

  protected:
    vertices_t vertices;
    indices_t indices;
  };
}
