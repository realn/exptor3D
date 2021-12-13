#include <glm/gtc/matrix_transform.hpp>

#include "gfx_MatrixStack.h"

namespace gfx {
  MatrixStack::MatrixStack() = default;

  MatrixStack::~MatrixStack() = default;

  void MatrixStack::setProjectionMatrix(glm::mat4 matrix) {
    matrixProjection = matrix;
  }

  void MatrixStack::setCurrentModelViewMatrix(glm::mat4 matrix) {
    matrixCurrent = matrix;
  }

  glm::mat4 MatrixStack::getProjectionMatrix() const {
    return matrixProjection;
  }

  glm::mat4 MatrixStack::getCurrentModelViewMatrix() const {
    return matrixCurrent;
  }

  void MatrixStack::pushMatrix() {
    matrixStack.push_back(matrixCurrent);
  }

  void MatrixStack::popMatrix() {
    if (matrixStack.empty())
      return;

    matrixCurrent = *matrixStack.rbegin();
    matrixStack.pop_back();
  }

  void MatrixStack::translate(glm::vec3 value) {
    matrixCurrent = glm::translate(matrixCurrent, value);
  }

  void MatrixStack::rotateDeg(float angleDeg, glm::vec3 axis) {
    matrixCurrent = glm::rotate(matrixCurrent, glm::radians(angleDeg), axis);
  }

  void MatrixStack::rotate(float angle, glm::vec3 axis) {
    matrixCurrent = glm::rotate(matrixCurrent, angle, axis);
  }

  void MatrixStack::rotate(glm::quat value) {
    matrixCurrent = matrixCurrent * glm::mat4_cast(value);
  }

  void MatrixStack::scale(glm::vec3 value) {
    matrixCurrent = glm::scale(matrixCurrent, value);
  }

  void MatrixStack::loadIdentity() {
    matrixCurrent = glm::mat4(1.0f);
  }

}
