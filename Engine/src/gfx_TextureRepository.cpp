
#include "gfx_Texture.h"
#include "gfx_TextureRepository.h"

namespace gfx {
	TextureRepository::TextureRepository(const std::string& strDataDir)
		: core::Object(L"TextureManager")
		, DataDir(strDataDir) {
	}

	TextureRepository::~TextureRepository() = default;

	std::shared_ptr<Texture> TextureRepository::Get(std::string filename) {
		if (filename.empty()) {
			error("Pusty ci¹g znaków!");
			return nullptr;
		}

		auto path = DataDir + filename;

		auto it = std::find_if(textures.begin(), textures.end(), [&](textureptr_t& texture) {return texture->GetFile() == path; });
		if (it != textures.end()) {
			return *it;
		}

		auto texture = std::make_shared<Texture>(path);
		if (!texture->isLoaded()) {
			error("Nieudane za³adowanie tekstury: " + filename);
			return nullptr;
		}

		addTexture(texture);
		log("Dodano now¹ teksture: " + filename);
		return texture;
	}

	void TextureRepository::addTexture(std::shared_ptr<Texture> texture) {
		textures.push_back(std::move(texture));
	}
}