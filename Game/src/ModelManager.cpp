
#include <gfx_Model.h>

#include "Log.h"
#include "ModelManager.h"

namespace gfx {
	ModelManager::ModelManager(const std::string& strDataDir, gfx::TextureRepository& texManager) :
		DataDir(strDataDir),
		TexManager(texManager) {
	}

	ModelManager::~ModelManager() {
		Clear();
	}

	TextureRepository& ModelManager::GetTexMng() {
		return TexManager;
	}

	Model* ModelManager::Get(const std::string& filename) {
		if (filename.empty()) {
			Log.Error("MODELMANAGER(): Pusty ci¹g znaków!");
			return nullptr;
		}

		auto path = DataDir + filename;

		{
			auto it = std::find_if(List.begin(), List.end(), [&](Model* model) { return model->GetFile() == path; });
			if (it != List.end())
				return *it;
		}

		auto model = new Model();
		if (!model->load(TexManager, path)) {
			Log.Error("MODELMANAGER(): Nieudane za³adowanie modelu: " + filename);
			delete model;
			return nullptr;
		}

		AddModel(model);
		Log.Log("MODELMANAGER(): Dodano nowy model: " + filename);
		return model;
	}

	void ModelManager::AddModel(Model* Model) {
		List.push_back(Model);
	}

	void ModelManager::DeleteModel(unsigned int index) {
		if (index >= List.size())
			return;

		delete List[index];
		List.erase(List.begin() + index);
	}

	Model* ModelManager::GetModel(unsigned int index) {
		if (index >= List.size())
			return 0;

		return List[index];
	}

	void ModelManager::Clear() {
		for (unsigned i = 0; i < List.size(); i++) {
			Model* pModel = List[i];
			delete pModel;
		}
		List.clear();
	}
}
