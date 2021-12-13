#pragma once

#include <vector>
#include <memory>

#include <glm/matrix.hpp>

#include "gfx_MatrixStack.h"
#include "GUIMesh.h"

namespace gfx {
  class Texture;
}

namespace gui {
  class RenderContext : public gfx::MatrixStack {
  public:
    using textureptr_t = std::shared_ptr<gfx::Texture>;

    RenderContext() = default;
    RenderContext(const RenderContext&) = delete;
    RenderContext(RenderContext&&) = default;
    ~RenderContext() override;

    void setTexture(textureptr_t texture);
    void setColor(glm::vec4 color);

    void pushTexture();
    void pushColor();

    void popTexture();
    void popColor();

    void addTriangle(glm::vec3 v1, glm::vec2 t1, glm::vec3 v2, glm::vec2 t2, glm::vec3 v3, glm::vec2 t3);
    void addQuad(glm::vec3 v1, glm::vec2 t1, glm::vec3 v2, glm::vec2 t2, glm::vec3 v3, glm::vec2 t3, glm::vec3 v4, glm::vec2 t4);
    void addRect(glm::vec2 pos, glm::vec2 size, glm::vec2 tpos, glm::vec2 tsize);

    void render() const;

    RenderContext& operator=(const RenderContext&) = delete;
    RenderContext& operator=(RenderContext&&) = default;

  private:
    using meshes_t = std::vector<Mesh>;
    using textures_t = std::vector<textureptr_t>;
    using colors_t = std::vector<glm::vec4>;

    Mesh& getMesh();

    meshes_t meshes;
    textures_t textureStack;
    colors_t colorStack;

    std::shared_ptr<gfx::Texture> textureCurrent;
    glm::vec4 colorCurrent = glm::vec4(1.0f);
  };
}