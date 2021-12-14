#include <CBGL/Rendering.h>
#include <CBGL/Defines.h>

#include <CBGL/COpenGL.h>

#include "gfx_Mesh.h"

namespace gfx {
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
      indices.push_back(static_cast<cb::u16>(baseIndex) + idx);
  }

  const Mesh::vertices_t& Mesh::getVertices() const {
    return vertices;
  }

  const Mesh::indices_t& Mesh::getIndices() const {
    return indices;
  }
}