
#include "gfx_Material.h"

namespace gfx {
  Material::Material(const cb::string& name) : name(name) {}

  Material::~Material() = default;

  void Material::setTexture(std::shared_ptr<Texture> value) {
    texture = value;
  }

  void Material::setColor(glm::vec4 value) {
    color = value;
  }

  const cb::string& Material::getName() const {
    return name;
  }

  std::shared_ptr<Texture> Material::getTexture() const {
    return texture;
  }

  glm::vec4 Material::getColor() const {
    return color;
  }
}