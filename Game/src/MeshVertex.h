#pragma once

#include <glm/vec4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

namespace gfx {
  class MeshVertex {
  public:
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 coord;

    bool operator==(const MeshVertex& other) {
      return position == other.position && normal == other.normal && coord == other.coord;
    }
  };
}