#include <CBGL/Rendering.h>
#include <CBGL/Defines.h>

#include <CBGL/COpenGL.h>

#include "gfx_Mesh.h"

namespace gfx {
  Mesh Mesh::copy() const {
    auto mesh = Mesh();
    mesh.indices = indices;
    mesh.vertices = vertices;
    return mesh;
  }

  void Mesh::add(glm::vec3 position, glm::vec3 normal, glm::vec2 texCoord) {
    add(MeshVertex{ position, normal, texCoord });
  }

  void Mesh::add(const MeshVertex& vertex) {
    auto it = std::find(vertices.begin(), vertices.end(), vertex);
    auto index = cb::u16();
    if (it == vertices.end()) {
      index = static_cast<cb::u16>(vertices.size());
      vertices.push_back(vertex);
    }
    else {
      index = static_cast<cb::u16>(std::distance(vertices.begin(), it));
    }
    indices.push_back(index);
  }

  void Mesh::add(const Mesh& mesh) {
    auto baseIndex = vertices.size();
    vertices.insert(vertices.end(), mesh.vertices.begin(), mesh.vertices.end());
    indices.reserve(indices.size() + mesh.indices.size());
    for (auto& idx : mesh.indices)
      indices.push_back(baseIndex + idx);
  }

  void Mesh::prepare() {
    vertexBuffer = std::make_unique<cb::gl::Buffer>();
    {
      auto vbind = cb::gl::bind(*vertexBuffer);
      vertexBuffer->setData(vertices);
    }

    indexBuffer = std::make_unique<cb::gl::Buffer>(cb::gl::BufferTarget::ELEMENT_ARRAY);
    {
      auto bbind = cb::gl::bind(*indexBuffer);
      indexBuffer->setData(indices);
    }
  }

  void Mesh::render() {
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    auto vbind = cb::gl::bind(*vertexBuffer);
    glVertexPointer(3, GL_FLOAT, sizeof(MeshVertex), nullptr);
    glNormalPointer(GL_FLOAT, sizeof(MeshVertex), reinterpret_cast<const void*>(sizeof(glm::vec3)));
    glTexCoordPointer(2, GL_FLOAT, sizeof(MeshVertex), reinterpret_cast<const void*>(2 * sizeof(glm::vec3)));

    auto ibind = cb::gl::bind(*indexBuffer);
    cb::gl::drawElements(cb::gl::PrimitiveType::TRIANGLES, indices.size());

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
  }
}