#include <glm/gtc/matrix_transform.hpp>

#include "GUIMesh.h"
#include "GUIRenderContext.h"

namespace gui {
  void RenderContext::setTexture(textureptr_t texture) {
    textureCurrent = texture;
  }

  void RenderContext::setColor(glm::vec4 color) {
    colorCurrent = color;
  }

  void RenderContext::pushMatrix() {
    matrixStack.push_back(matrixCurrent);
  }

  void RenderContext::pushTexture() {
    textureStack.push_back(textureCurrent);
  }

  void RenderContext::pushColor() {
    colorStack.push_back(colorCurrent);
  }

  void RenderContext::popMatrix() {
    if (matrixStack.empty())
      return;

    matrixCurrent = *matrixStack.rbegin();
    matrixStack.pop_back();
  }

  void RenderContext::popTexture() {
    if (textureStack.empty())
      return;

    textureCurrent = *textureStack.rbegin();
    textureStack.pop_back();
  }

  void RenderContext::popColor() {
    if (colorStack.empty())
      return;

    colorCurrent = *colorStack.rbegin();
    colorStack.pop_back();
  }

  void RenderContext::translate(glm::vec3 value) {
    matrixCurrent = glm::translate(matrixCurrent, value);
  }

  void RenderContext::rotate(float angleDeg, glm::vec3 axis) {
    matrixCurrent = glm::rotate(matrixCurrent, glm::radians(angleDeg), axis);
  }

  void RenderContext::scale(glm::vec3 value) {
    matrixCurrent = glm::scale(matrixCurrent, value);
  }

  void RenderContext::addTriangle(glm::vec3 v1, glm::vec2 t1, glm::vec3 v2, glm::vec2 t2, glm::vec3 v3, glm::vec2 t3) {
    auto& mesh = getMesh();

    mesh.add({ glm::vec3(matrixCurrent * glm::vec4(v1, 1.0f)), t1, colorCurrent });
    mesh.add({ glm::vec3(matrixCurrent * glm::vec4(v2, 1.0f)), t2, colorCurrent });
    mesh.add({ glm::vec3(matrixCurrent * glm::vec4(v3, 1.0f)), t3, colorCurrent });
  }

  void RenderContext::addQuad(glm::vec3 v1, glm::vec2 t1, glm::vec3 v2, glm::vec2 t2, glm::vec3 v3, glm::vec2 t3, glm::vec3 v4, glm::vec2 t4) {
    addTriangle(v1, t1, v2, t2, v3, t3);
    addTriangle(v1, t1, v3, t3, v4, t4);
  }

  void RenderContext::addRect(glm::vec2 pos, glm::vec2 size, glm::vec2 tpos, glm::vec2 tsize) {
    auto v1 = glm::vec3(pos, 0.0f);
    auto v2 = v1 + glm::vec3(size.x, 0.0f, 0.0f);
    auto v3 = v1 + glm::vec3(size, 0.0f);
    auto v4 = v1 + glm::vec3(0.0f, size.y, 0.0f);

    auto t1 = tpos;
    auto t2 = t1 + glm::vec2(tsize.x, 0.0f);
    auto t3 = t1 + tsize;
    auto t4 = t1 + glm::vec2(0.0f, tsize.y);

    addQuad(v1, t1, v2, t2, v3, t3, v4, t4);
  }

  void RenderContext::render() const {
    for (const auto& mesh : meshes) {
      mesh.render();
    }
  }

  Mesh& RenderContext::getMesh() {
    if (meshes.empty()) {
      meshes.push_back(Mesh(textureCurrent));
      return *meshes.rbegin();
    }

    auto& mesh = *meshes.rbegin();
    if (mesh.getTexture() == textureCurrent)
      return mesh;

    meshes.push_back(Mesh(textureCurrent));
    return *meshes.rbegin();
  }
}