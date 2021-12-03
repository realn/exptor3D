#include <CBCore/Defines.h>
#include <CBGL/COpenGL.h>
#include <CBGL/Buffer.h>

#include "gfx_Texture.h"
#include "GUIVertex.h"
#include "GUIMesh.h"

namespace gui {
  namespace {
    template<class _Type>
    const void* ptroffset(const _Type* ptr, size_t offset = 0) {
      auto p = reinterpret_cast<const cb::byte*>(ptr);
      return reinterpret_cast<const void*>(ptr + offset);
    }

    template<class _Type>
    const void* ptr(const _Type* data) {
      return reinterpret_cast<const void*>(data);
    }

    const void* offset(size_t off = 0) {
      return reinterpret_cast<const void*>(off);
    }
  }

  Mesh::Mesh(std::shared_ptr<gfx::Texture> texture) : texture(texture) {
  }

  Mesh::~Mesh() = default;

  void Mesh::add(Vertex vertex) {
    vertices.push_back(vertex);
  }

  void Mesh::render() const {
    if (vertices.empty())
      return;

    buffer = std::make_shared<cb::gl::Buffer>();
    buffer->setData(vertices);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);


    auto bbind = cb::gl::bind(*buffer);
    glVertexPointer(3, GL_FLOAT, sizeof(Vertex), offset());
    glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), offset(sizeof(glm::vec3)));
    glColorPointer(4, GL_FLOAT, sizeof(Vertex), offset(sizeof(glm::vec2) + sizeof(glm::vec3)));

    if (texture) {
      texture->Activate();
      glEnable(GL_TEXTURE_2D);
      cb::gl::drawArrays(cb::gl::PrimitiveType::TRIANGLES, vertices.size());
      glDisable(GL_TEXTURE_2D);
    }
    else
      cb::gl::drawArrays(cb::gl::PrimitiveType::TRIANGLES, vertices.size());

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);

  }
}