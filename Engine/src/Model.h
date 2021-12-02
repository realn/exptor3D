/*///////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
Plik:	Model.h
Autor:	Real_Noname (real_noname@coderulers.info)
(C):	CODE RULERS (Real_Noname)
WWW:	www.coderulers.info
Opis:	Zawiera definicje klas do ³adowania i zarz¹dzania
		modelami z plików GLM.

/////////////////////////////////////////////////////////
///////////////////////////////////////////////////////*/
#pragma once

#include <fstream>

#include <CBGL/COpenGL.h>

#include "core_object.h"

namespace core {
	class FileParser;
}
namespace gfx {
	class Mesh;
	class TextureRepository;
}

// Numer wersji loader'a
#define		GLM_FILE_VERSION		100

/*	G³ówna klasa modeli OpenGL
*/
class CModel : public core::Object {

public:
	CModel();
	~CModel() override;

	CModel(const CModel&) = delete;
	CModel(CModel&&) = default;
	CModel& operator=(const CModel&) = delete;
	CModel& operator=(CModel&&) = default;

	bool load( gfx::TextureRepository& texManager, const std::string& filePath );

	void Free();
	const std::string GetFile() const;
	unsigned int GetObjCount();

	void render( unsigned int index );

protected:
	virtual cb::string getLogName() const override;

private:
	struct Material;
	struct Object;
	struct Mesh;

	using materials_t = std::vector<Material>;
	using objects_t = std::vector<Object>;

	bool loadMaterial(core::FileParser& parser, gfx::TextureRepository& texManager);
	bool loadObject(core::FileParser& parser);
	bool loadMesh(core::FileParser& parser, Object& obj);

	materials_t materials;
	objects_t objects;

	// Czy model jest za³adowany?
	bool loaded = false;
	// Ostatni za³adowany plik
	cb::string file = L"-";
};
