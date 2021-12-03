#include <CBCore/Defines.h>
#include <CBGL/COpenGL.h>
#include <CBGL/Buffer.h>

#include "GUIVertex.h"
#include "GUIMesh.h"

namespace gui {
  namespace {
    template<class _Type>
    const void* ptroffset(const _Type* ptr, size_t offset = 0) {
      auto p = reinterpret_cast<const cb::byte*>(ptr);
      return reinterpret_cast<const void*>(ptr + offset);
    }
  }

  Mesh::Mesh(std::shared_ptr<gfx::Texture> texture) {
  }

  Mesh::~Mesh() = default;

  void Mesh::add(Vertex vertex) {
    vertices.push_back(vertex);
  }

  void Mesh::render() const {
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    auto ptr = vertices.data();

    glVertexPointer(3, GL_FLOAT, sizeof(Vertex), ptroffset(ptr));
    glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), ptroffset(ptr, sizeof(glm::vec3)));
    glColorPointer(4, GL_FLOAT, sizeof(Vertex), ptroffset(ptr, sizeof(glm::vec3) + sizeof(glm::vec2)));

    if (texture) {
      auto tbind = cb::gl::bind(*texture);
      cb::gl::drawArrays(cb::gl::PrimitiveType::TRIANGLES, vertices.size());
    }
    else
      cb::gl::drawArrays(cb::gl::PrimitiveType::TRIANGLES, vertices.size());

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);

  }
}