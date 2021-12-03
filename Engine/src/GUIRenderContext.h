#pragma once

#include <vector>
#include <memory>

#include <glm/matrix.hpp>

namespace gfx {
  class Texture;
}

namespace gui {
  class Mesh;
  class RenderContext {
  public:
    using textureptr_t = std::shared_ptr<gfx::Texture>;

    void setTexture(textureptr_t texture);
    void setColor(glm::vec4 color);

    void pushMatrix();
    void pushTexture();
    void pushColor();

    void popMatrix();
    void popTexture();
    void popColor();

    void translate(glm::vec3 value);
    void rotate(float angleDeg, glm::vec3 axis);
    void scale(glm::vec3 value);

    void addTriangle(glm::vec3 v1, glm::vec2 t1, glm::vec3 v2, glm::vec2 t2, glm::vec3 v3, glm::vec2 t3);
    void addQuad(glm::vec3 v1, glm::vec2 t1, glm::vec3 v2, glm::vec2 t2, glm::vec3 v3, glm::vec2 t3, glm::vec3 v4, glm::vec2 t4);
    void addRect(glm::vec2 pos, glm::vec2 size, glm::vec2 tpos, glm::vec2 tsize);

    void render() const;

  private:
    using meshes_t = std::vector<Mesh>;
    using matrices_t = std::vector<glm::mat4>;
    using textures_t = std::vector<textureptr_t>;
    using colors_t = std::vector<glm::vec4>;

    Mesh& getMesh();

    meshes_t meshes;
    matrices_t matrixStack;
    textures_t textureStack;
    colors_t colorStack;

    glm::mat4 matrixCurrent = glm::mat4(1.0f);
    std::shared_ptr<gfx::Texture> textureCurrent;
    glm::vec4 colorCurrent = glm::vec4(1.0f);
  };
}