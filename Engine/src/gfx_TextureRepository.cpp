
#include "gfx_Texture.h"
#include "gfx_TextureRepository.h"

namespace gfx {
	TextureRepository::TextureRepository(const std::string& strDataDir)
		: core::Object(L"TextureManager")
		, DataDir(strDataDir) {
	}

	TextureRepository::~TextureRepository() = default;

	Texture* TextureRepository::Get(std::string filename) {
		if (filename.empty()) {
			error("Pusty ci¹g znaków!");
			return nullptr;
		}

		auto path = DataDir + filename;

		auto it = std::find_if(textures.begin(), textures.end(), [&](textureptr_t& texture) {return texture->GetFile() == path; });
		if (it != textures.end()) {
			return it->get();
		}

		auto texture = std::make_unique<Texture>(path);
		if (!texture->isLoaded()) {
			error("Nieudane za³adowanie tekstury: " + filename);
			return nullptr;
		}

		auto result = texture.get();
		addTexture(std::move(texture));
		log("Dodano now¹ teksture: " + filename);
		return result;
	}

	void TextureRepository::addTexture(std::unique_ptr<Texture> texture) {
		textures.push_back(std::move(texture));
	}
}