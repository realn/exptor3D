
#include "gfx_Mesh.h"
#include "gfx_BufferedMesh.h"

namespace gfx {
  BufferedMesh::BufferedMesh(const Mesh& mesh, cb::gl::BufferUsage usage) {
    setData(mesh, usage);
  }

  void BufferedMesh::setData(const Mesh& mesh, cb::gl::BufferUsage usage) {
    vertexBuffer = std::make_unique<cb::gl::Buffer>();
    {
      auto vbind = cb::gl::bind(*vertexBuffer);
      vertexBuffer->setData(mesh.getVertices());
    }

    indexBuffer = std::make_unique<cb::gl::Buffer>(cb::gl::BufferTarget::ELEMENT_ARRAY);
    {
      auto bbind = cb::gl::bind(*indexBuffer);
      indexBuffer->setData(mesh.getIndices());
    }

    numberOfIndices = mesh.getIndices().size();
  }
}