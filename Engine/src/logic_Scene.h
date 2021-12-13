#pragma once

#include "logic_SceneNode.h"

namespace logic {
  class Scene {
  public:
    virtual ~Scene();

    void update(float timeDelta);
    void queueRender(gfx::Frame& frame) const;

    void setRootNode(SceneNode::nodeptr_t value);
    SceneNode::nodeptr_t getRootNode() const;

  protected:
    SceneNode::nodeptr_t rootNode;

    void queueNodeRender(const SceneNode& node, gfx::Frame& frame) const;
  };
}
