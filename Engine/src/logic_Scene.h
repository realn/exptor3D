#pragma once

#include "logic_SceneNode.h"

namespace logic {
  class Scene {
  public:
    virtual ~Scene();

    void update(float timeDelta);
    void queueRender(gfx::Frame& frame) const;

  protected:
    SceneNode::nodeptr_t rootNode;
  };
}
