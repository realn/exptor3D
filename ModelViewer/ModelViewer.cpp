
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "ModelViewer.h"

namespace mdlview {
  ModelViewer::ModelViewer(std::shared_ptr<gfx::TextureRepository> texRepo) : core::Object(L"ModelViewer"), texRepo(texRepo) {

    mapper.addState(L"viewer_rotate_model_left", [this](const event::EventState& s) { rotateLeft = s.getState(); });
    mapper.addState(L"viewer_rotate_model_right", [this](const event::EventState& s) { rotateRight = s.getState(); });
  }

  ModelViewer::~ModelViewer() = default;

  bool ModelViewer::loadModel(cb::string filename) {
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

  void ModelViewer::update(float timeDelta) {
    if (rotateLeft)
      modelRotation += glm::radians(20.0f) * timeDelta;
    if (rotateRight)
      modelRotation -= glm::radians(20.0f) * timeDelta;
  }

  void ModelViewer::render() const {
    if (model) {
      auto mat = glm::mat4(1.0f);
      
      mat = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -5.0f));
      mat = glm::rotate(mat, modelRotation, glm::vec3(0.0f, 1.0f, 0.0f));

      glLoadMatrixf(glm::value_ptr(mat));

      model->render(0);
    }
  }

  void ModelViewer::processEvent(const event::Event& event) {
    mapper.executeEvent(event);
  }

}

