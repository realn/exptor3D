#pragma once

#include <vector>
#include <memory>

#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>

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

    void setPosition(glm::vec3 value);
    void setRotation(glm::quat value);

    glm::vec3 getPosition() const;
    glm::quat getRotation() const;
    nodes_t& getNodes();
    const nodes_t getNodes() const;

    virtual void update(float timeDelta);
    virtual void queueRender(gfx::Frame& frame) const;

  protected:
    nodeweakptr_t parent;
    nodes_t nodes;

    glm::vec3 position;
    glm::quat rotation;
  };
}
