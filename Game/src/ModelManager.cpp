
#include <CBCore/StringConvert.h>
#include <gfx_Model.h>

#include "ModelManager.h"

namespace gfx {
	ModelManager::ModelManager(const std::string& strDataDir, gfx::TextureRepository& texManager) :
		core::Object(L"ModelManager"),
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
			error(L"Pusty ci¹g znaków!");
			return nullptr;
		}

		auto path = DataDir + filename;

		{
			auto it = std::find_if(List.begin(), List.end(), [&](Model* model) { return model->GetFile() == path; });
			if (it != List.end())
				return *it;
		}

		auto model = new Model();
		if (!model->load(TexManager, cb::fromUtf8(path))) {
			error(L"Nieudane za³adowanie modelu: " + cb::fromUtf8(filename));
			delete model;
			return nullptr;
		}

		AddModel(model);
		log(L"Dodano nowy model: " + cb::fromUtf8(filename));
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
