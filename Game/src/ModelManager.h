#pragma once

#include <core_object.h>
#include <gfx_Model.h>

namespace gfx {
	class ModelManager : core::Object {
	private:
		const std::string	DataDir;
		gfx::TextureRepository& TexManager;
		std::vector<Model*> List;

	public:
		ModelManager(const std::string& strDataDir, gfx::TextureRepository& texManager);
		~ModelManager();

		gfx::TextureRepository& GetTexMng();

		Model* Get(const std::string& filename);
		void AddModel(Model* Model);
		Model* GetModel(unsigned int index);
		void DeleteModel(unsigned int index);

		void Clear();

	private:
		ModelManager(const ModelManager&) = delete;
		ModelManager& operator=(const ModelManager&) = delete;
	};
}