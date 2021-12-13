
#include "logic_SceneNode.h"

namespace logic {
  SceneNode::SceneNode(nodeptr_t parent) {
  }

  SceneNode::~SceneNode() {
  }

  void SceneNode::addNode(nodeptr_t node) {
  }

  void SceneNode::setPosition(glm::vec3 value) {
    position = value;
  }

  void SceneNode::setRotation(glm::quat value) {
    rotation = value;
  }

  glm::vec3 SceneNode::getPosition() const {
    return position;
  }

  glm::quat SceneNode::getRotation() const {
    return rotation;
  }

  SceneNode::nodes_t& SceneNode::getNodes() {
    return nodes;
  }

  const SceneNode::nodes_t SceneNode::getNodes() const {
    return nodes;
  }

  void SceneNode::update(float timeDelta) {
  }

  void SceneNode::queueRender(gfx::Frame& frame) const {
  }
}