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
    bool reloadModel();
    void selectObjName(cb::string objName);

    void update(float timeDelta);
    void render(gfx::Frame& frame) const;

    void	processEvent(const event::Event& event) override;

    cb::strvector getObjectNames() const;

  private:
    event::Mapper mapper;
    std::shared_ptr<gfx::TextureRepository> texRepo;
    std::shared_ptr<gfx::Model> model;

    cb::string loadedFile;
    cb::string currentObjName = L"base";
    bool rotateLeft = false;
    bool rotateRight = false;
    float rotationSpeed = 60.0f;
    float modelRotation = 0.0f;
  };
}