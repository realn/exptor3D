#pragma once

#include <memory>

#include <CBGL/Buffer.h>

namespace gfx {
  class Mesh;
  class BufferedMesh {
  public:
    BufferedMesh() = default;
    BufferedMesh(const Mesh& mesh, cb::gl::BufferUsage usage = cb::gl::BufferUsage::STATIC_DRAW);
    BufferedMesh(BufferedMesh&&) = default;
    BufferedMesh& operator=(BufferedMesh&&) = default;

    void setData(const Mesh& mesh, cb::gl::BufferUsage usage = cb::gl::BufferUsage::STATIC_DRAW);

    cb::gl::Buffer& getVertexBuffer() const { return *vertexBuffer; }
    cb::gl::Buffer& getIndexBuffer() const { return *indexBuffer; }
    size_t getNumberOfIndices() const { return numberOfIndices; }

  private:
    std::unique_ptr<cb::gl::Buffer> vertexBuffer;
    std::unique_ptr<cb::gl::Buffer> indexBuffer;
    size_t numberOfIndices = 0;
  };
}
