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

#include "Texture.h"
#include "Log.h"

CTexture::CTexture() : core::Object(L"Texture") {}

CTexture::CTexture( std::string filename )
	: CTexture()
{
	load( filename );
}

CTexture::~CTexture() = default;

bool CTexture::load( std::string filename )
{
	file = cb::fromUtf8(filename);
	auto surface = cb::sdl::Surface::loadTGA(file);
	if (!surface) {
		loaded = false;
		error("Failed to load " + filename);
		return false;
	}

	surface = surface.convert(cb::sdl::PixelFormat::RGBA32);
	surface.flip(cb::sdl::FlipDir::Vertical);

	texture = std::make_unique<cb::gl::Texture>(surface.getSize(), cb::gl::TextureFormat::RGBA8);
	texture->setData(cb::gl::InputFormat::RGBA, surface.getPixels());

	loaded = true;
	return true;
}

void CTexture::Activate( unsigned int tex )
{
	if( !loaded )
		return;

	texture->bind();
}

std::string CTexture::GetFile()
{
	return cb::toUtf8(file);
}

cb::string CTexture::getLogName() const {
	return getClassName() + L"(" + file + L"):";
}



CTexManager::CTexManager( const std::string& strDataDir ) 
	: core::Object(L"TextureManager")
	, DataDir( strDataDir )
{
}

CTexManager::~CTexManager() = default;

CTexture* CTexManager::Get( std::string filename )
{
	if( filename.empty() )
	{
		error( "Pusty ci¹g znaków!" );
		return nullptr;
	}

	auto path = DataDir + filename;

	auto it = std::find_if(textures.begin(), textures.end(), [&](textureptr_t& texture) {return texture->GetFile() == path; });
	if (it != textures.end()) {
		return it->get();
	}

	auto texture = std::make_unique<CTexture>(path);
	if(!texture->isLoaded())
	{
		error( "Nieudane za³adowanie tekstury: " + filename );
		return nullptr;
	}

	auto result = texture.get();
	addTexture(std::move(texture));
	log( "Dodano now¹ teksture: " + filename );
	return result;
}

void CTexManager::addTexture(std::unique_ptr<CTexture> texture)
{
	textures.push_back(std::move(texture));
}