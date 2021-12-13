#pragma once

#include <logic_SceneNode.h>

namespace gfx {
  class Model;
}

namespace e3dt {
  namespace game {
    class Object : public logic::SceneNode {
    public:
      Object(SceneNode::nodeptr_t parent);
      ~Object() override;

      void setModel(std::shared_ptr<gfx::Model> value);
      void setModelObjName(cb::string value);

      void update(float timeDelta) override;
      void queueRender(gfx::Frame& frame) const override;
      
    private:
      std::shared_ptr<gfx::Model> model;
      cb::string modelObjName = L"base";
    };
  }
}