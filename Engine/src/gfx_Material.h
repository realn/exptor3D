#pragma once

#include <glm/vec4.hpp>

#include <CBCore/Defines.h>

namespace gfx {
  class Texture;

  class Material {
  public:
    Material(const cb::string& name);
    ~Material();

    void setTexture(std::shared_ptr<Texture> value);
    void setColor(glm::vec4 value);

    const cb::string& getName() const;
    std::shared_ptr<Texture> getTexture() const;
    glm::vec4 getColor() const;

  private:
    cb::string name;
    std::shared_ptr<Texture> texture;
    glm::vec4 color = glm::vec4(1.0f);
  };
}