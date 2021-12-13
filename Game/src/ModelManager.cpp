
#include <CBCore/StringConvert.h>
#include <gfx_Model.h>

#include "ModelManager.h"

namespace gfx {
  ModelManager::ModelManager(const cb::string& dataDir, std::shared_ptr<gfx::TextureRepository> texRepo)
    : core::Object(L"ModelManager")
    , dataDir(dataDir)
    , textureRepository(texRepo) {
  }

  ModelManager::~ModelManager() = default;

  ModelManager::modelptr_t ModelManager::get(const cb::string& filename) {
    if (filename.empty()) {
      error(L"Pusty ci¹g znaków!");
      return nullptr;
    }

    auto path = dataDir + filename;

    {
      auto it = std::find_if(models.begin(), models.end(), [&](modelptr_t model) { return model->GetFile() == path; });
      if (it != models.end())
        return *it;
    }

    auto model = std::make_shared<Model>();
    if (!model->load(*textureRepository, path)) {
      error(L"Nieudane za³adowanie modelu: " + filename);
      return nullptr;
    }

    models.push_back(model);
    log(L"Dodano nowy model: " + filename);
    return model;
  }
}
