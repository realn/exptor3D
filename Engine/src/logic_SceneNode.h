#pragma once

#include <vector>
#include <memory>

namespace gfx {
  class Frame;
}

namespace logic {
  class SceneNode {
  public:
    using nodeptr_t = std::shared_ptr<SceneNode>;
    using nodeweakptr_t = std::weak_ptr<SceneNode>;
    using nodes_t = std::vector<nodeptr_t>;

    SceneNode(nodeptr_t parent);
    virtual ~SceneNode();

    void addNode(nodeptr_t node);
    nodes_t& getNodes();

    virtual void update(float timeDelta);
    virtual void queueRender(gfx::Frame& frame) const;

  protected:
    nodeweakptr_t parent;
    nodes_t nodes;
  };
}
