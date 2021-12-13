
#include "logic_SceneNode.h"

namespace logic {
  SceneNode::SceneNode(nodeptr_t parent) {
  }

  SceneNode::~SceneNode() {
  }

  void SceneNode::addNode(nodeptr_t node) {
  }

  SceneNode::nodes_t& SceneNode::getNodes() {
    return nodes;
  }

  void SceneNode::update(float timeDelta) {
  }

  void SceneNode::queueRender(gfx::Frame& frame) const {
  }
}