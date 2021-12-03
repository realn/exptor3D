#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

namespace gui {
  class Vertex {
  public:
    glm::vec3 position;
    glm::vec2 texCoord;
    glm::vec4 color;
  };
}