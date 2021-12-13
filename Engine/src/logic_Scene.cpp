
#include "gfx_Frame.h"
#include "logic_Scene.h"

namespace logic {
  Scene::~Scene() = default;

  void Scene::update(float timeDelta) {
    if (!rootNode)
      return;

    SceneNode::nodes_t nodes;
    nodes.push_back(rootNode);

    do {
      auto node = nodes.back();
      nodes.pop_back();

      node->update(timeDelta);

      nodes.insert(nodes.end(), node->getNodes().begin(), node->getNodes().end());

    } while (!nodes.empty());
  }

  void Scene::queueRender(gfx::Frame& frame) const {
    if (!rootNode)
      return;

    queueNodeRender(*rootNode, frame);
  }

  void Scene::setRootNode(SceneNode::nodeptr_t value) {
    rootNode = value;
  }

  SceneNode::nodeptr_t Scene::getRootNode() const {
    return rootNode;
  }

  void Scene::queueNodeRender(const SceneNode& node, gfx::Frame& frame) const {
    frame.pushMatrix();
    
    frame.translate(node.getPosition());
    frame.rotate(node.getRotation());

    node.queueRender(frame);

    for (auto& subNode : node.getNodes()) {
      queueNodeRender(*subNode, frame);
    }

    frame.popMatrix();
  }
}