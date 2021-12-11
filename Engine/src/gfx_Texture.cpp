/*///////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
Plik:	Texture.cpp
Autor:	Real_Noname (real_noname@wp.pl)
(C):	CODE RULERS (Real_Noname)
WWW:	www.coderulers.prv.pl
Opis:	Patrz -> Texture.h

/////////////////////////////////////////////////////////
///////////////////////////////////////////////////////*/

#include <CBCore/File.h>
#include <CBCore/StringConvert.h>
#include <CBSDL/Surface.h>

#include "gfx_Texture.h"

namespace gfx {
	Texture::Texture() : core::Object(L"Texture") {}

	Texture::Texture(std::string filename)
		: Texture() {
		load(filename);
	}

	Texture::~Texture() = default;

	bool Texture::load(std::string filename) {
		file = cb::fromUtf8(filename);
		auto surface = cb::sdl::Surface::loadTGA(file);
		if (!surface) {
			loaded = false;
			error(L"Failed to load " + file);
			return false;
		}

		surface = surface.convert(cb::sdl::PixelFormat::RGBA32);
		surface.flip(cb::sdl::FlipDir::Vertical);

		texture = std::make_unique<cb::gl::Texture>(surface.getSize(), cb::gl::TextureFormat::RGBA8);
		texture->setData(cb::gl::InputFormat::RGBA, surface.getPixels());

		loaded = true;
		return true;
	}

	void Texture::Activate(unsigned int tex) {
		if (!loaded)
			return;

		texture->bind();
	}

	std::string Texture::GetFile() {
		return cb::toUtf8(file);
	}

	cb::string Texture::getLogName() const {
		return getClassName() + L"(" + file + L"):";
	}
}