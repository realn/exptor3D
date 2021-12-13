
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <gfx_Frame.h>

#include "ModelViewer.h"

namespace mdlview {
  ModelViewer::ModelViewer(std::shared_ptr<gfx::TextureRepository> texRepo) : core::Object(L"ModelViewer"), texRepo(texRepo) {

    mapper.addState(L"viewer_rotate_model_left", [this](const event::EventState& s) { rotateLeft = s.getState(); });
    mapper.addState(L"viewer_rotate_model_right", [this](const event::EventState& s) { rotateRight = s.getState(); });
    mapper.addAction(L"viewer_reload_model", [this](const event::EventAction&) { reloadModel(); });
  }

  ModelViewer::~ModelViewer() = default;

  bool ModelViewer::loadModel(cb::string filename) {
    model = std::make_shared<gfx::Model>();
    loadedFile = filename;
    currentObjName = L"base";

    if (filename.empty())
      return false;

    model = std::make_shared<gfx::Model>();
    if (!model->load(*texRepo, L"Data/Models/" + filename)) {
      writeLogLines(core::LogType::Error, model->getLoadingLog());
      return false;
    }

    writeLogLines(core::LogType::Info, model->getLoadingLog());
    return true;
  }

  bool ModelViewer::reloadModel() {
    return loadModel(loadedFile);
  }

  void ModelViewer::selectObjName(cb::string objName) {
    currentObjName = objName;
  }

  void ModelViewer::update(float timeDelta) {
    if (rotateLeft)
      modelRotation += glm::radians(rotationSpeed) * timeDelta;
    if (rotateRight)
      modelRotation -= glm::radians(rotationSpeed) * timeDelta;
  }

  void ModelViewer::render(gfx::Frame& frame) const {
    if (model) {
      frame.pushMatrix();

      frame.translate({ 0.0f, 0.0f, -5.0f });
      frame.rotate(modelRotation, { 0.0f, 1.0f, 0.0f });

      model->queueRender(frame, currentObjName);

      frame.popMatrix();
    }
  }

  void ModelViewer::processEvent(const event::Event& event) {
    mapper.executeEvent(event);
  }

  cb::strvector ModelViewer::getObjectNames() const {
    if (model)
      return model->getObjectNames();
    return cb::strvector();
  }

}

