#pragma once

#include <CBCore/Defines.h>

#include <core_object.h>
#include <gfx_Model.h>
#include <event_Observer.h>
#include <event_Mapper.h>

namespace mdlview {
  class ModelViewer 
    : public core::Object
    , public event::IObserver
  {
  public:
    ModelViewer(std::shared_ptr<gfx::TextureRepository> texRepo);
    ~ModelViewer() override;

    bool loadModel(cb::string filename);

    void update(float timeDelta);
    void render() const;

    void	processEvent(const event::Event& event) override;

  private:
    event::Mapper mapper;
    std::shared_ptr<gfx::TextureRepository> texRepo;
    std::shared_ptr<gfx::Model> model;

    bool rotateLeft = false;
    bool rotateRight = false;
    float modelRotation = 0.0f;
  };
}