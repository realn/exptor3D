
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <CBGL/COpenGL.h>
#include <CBGL/State.h>

#include "GUIRenderContext.h"
#include "gfx_Texture.h"
#include "gfx_Frame.h"
#include "gfx_FrameElement.h"
#include "gfx_RenderSystem.h"

namespace gfx {
  void RenderSystem::render(const Frame& frame) {

    auto proj = frame.getProjectionMatrix();

    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(glm::value_ptr(proj));
    glMatrixMode(GL_MODELVIEW);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    for (auto element : frame.elements) {
      glLoadMatrixf(glm::value_ptr(element.modelview));

      if (element.material->getTexture()) {
        element.material->getTexture()->Activate();
      }

      auto vbind = cb::gl::bind(element.mesh->getVertexBuffer());
      glVertexPointer(3, GL_FLOAT, sizeof(MeshVertex), nullptr);
      glNormalPointer(GL_FLOAT, sizeof(MeshVertex), reinterpret_cast<const void*>(sizeof(glm::vec3)));
      glTexCoordPointer(2, GL_FLOAT, sizeof(MeshVertex), reinterpret_cast<const void*>(2 * sizeof(glm::vec3)));

      auto ibind = cb::gl::bind(element.mesh->getIndexBuffer());
      cb::gl::drawElements(cb::gl::PrimitiveType::TRIANGLES, element.mesh->getNumberOfIndices());
    }

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
  }
  void RenderSystem::render(const gui::RenderContext& ctx) {
    auto state = cb::gl::getBlendState();
    state.enabled = true;
    state.setFunc(cb::gl::BlendFactor::SRC_ALPHA, cb::gl::BlendFactor::ONE_MINUS_SRC_ALPHA);
    cb::gl::setState(state);

    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(glm::value_ptr(ctx.getProjectionMatrix()));
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    ctx.render();
  }
}