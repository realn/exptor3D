#pragma once

#include "core_object.h"

namespace gfx {
	class Texture;

	class TextureRepository : core::Object {
	public:
		TextureRepository(const std::string& strDataDir);
		~TextureRepository() override;

		std::shared_ptr<Texture> Get(std::string filename);

	private:
		using textureptr_t = std::shared_ptr<Texture>;
		using textures_t = std::vector<textureptr_t>;

		void addTexture(std::shared_ptr<Texture> texture);

		const std::string	DataDir;
		textures_t textures;
	};

}