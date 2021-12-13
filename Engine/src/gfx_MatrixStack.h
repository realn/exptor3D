#pragma once

#include <vector>
#include <glm/matrix.hpp>
#include <glm/gtc/quaternion.hpp>

namespace gfx {
  class MatrixStack {
  public:
    MatrixStack();
    virtual ~MatrixStack();

    void setProjectionMatrix(glm::mat4 matrix);
    void setCurrentModelViewMatrix(glm::mat4 matrix);

    glm::mat4 getProjectionMatrix() const;
    glm::mat4 getCurrentModelViewMatrix() const;

    void pushMatrix();
    void popMatrix();

    void translate(glm::vec3 value);
    void rotate(float angleDeg, glm::vec3 axis);
    void rotate(glm::quat value);
    void scale(glm::vec3 value);
    void loadIdentity();

  protected:
    using matrices_t = std::vector<glm::mat4>;

    matrices_t matrixStack;
    glm::mat4 matrixProjection = glm::mat4(1.0f);
    glm::mat4 matrixCurrent = glm::mat4(1.0f);
  };
}
