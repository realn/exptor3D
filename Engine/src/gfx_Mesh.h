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

    Mesh(const Mesh&) = delete;
    Mesh(Mesh&&) = default;

    Mesh& operator=(const Mesh&) = delete;
    Mesh& operator=(Mesh&&) = default;

    Mesh copy() const;

    void add(glm::vec3 position, glm::vec3 normal = glm::vec3(), glm::vec2 texCoord = glm::vec2());
    void add(const MeshVertex& vertex);
    void add(const Mesh& mesh);

    cb::gl::Buffer& getVertexBuffer() const { return *vertexBuffer; }
    cb::gl::Buffer& getIndexBuffer() const { return *indexBuffer; }
    size_t getNumberOfIndices() const { return indices.size(); }

    void prepare();

    void render();

  protected:
    vertices_t vertices;
    indices_t indices;

    std::unique_ptr<cb::gl::Buffer> vertexBuffer;
    std::unique_ptr<cb::gl::Buffer> indexBuffer;
  };
}
