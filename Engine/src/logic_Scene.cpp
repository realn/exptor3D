
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

    SceneNode::nodes_t nodes;
    nodes.push_back(rootNode);

    do {
      auto node = nodes.back();
      nodes.pop_back();

      node->queueRender(frame);

      nodes.insert(nodes.end(), node->getNodes().begin(), node->getNodes().end());

    } while (!nodes.empty());
  }
}