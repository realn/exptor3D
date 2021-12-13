#pragma once

#include <core_object.h>
#include <gfx_Model.h>

namespace gfx {
  class ModelManager : core::Object {
  public:
    using modelptr_t = std::shared_ptr<Model>;

    ModelManager(const cb::string& dataDir, std::shared_ptr<TextureRepository> texRepo);
    ~ModelManager() override;

    modelptr_t get(const cb::string& filename);

  private:
    using models_t = std::vector<modelptr_t>;

    ModelManager(const ModelManager&) = delete;
    ModelManager& operator=(const ModelManager&) = delete;

    const cb::string	dataDir;
    std::shared_ptr<TextureRepository> textureRepository;
    models_t models;
  };
}