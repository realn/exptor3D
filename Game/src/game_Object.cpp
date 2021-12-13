#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <gfx_Frame.h>
#include <gfx_Model.h>

#include "game_Object.h"

namespace e3dt {
  namespace game {
    Object::Object(SceneNode::nodeptr_t parent)
      : logic::SceneNode(parent) {
    }

    Object::~Object() = default;

    void Object::update(float timeDelta) {
      rotation = glm::rotate(rotation, glm::radians(50.0f) * timeDelta, { 0.0f, 1.0f, 0.0f });
    }

    void Object::setModel(std::shared_ptr<gfx::Model> value) {
      model = value;
    }

    void Object::setModelObjName(cb::string value) {
      modelObjName = value;
    }

    void Object::queueRender(gfx::Frame& frame) const {
      if (model) {
        model->queueRender(frame, modelObjName);
      }
    }
  }
}