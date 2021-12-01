/*///////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
Plik:	Model.h
Autor:	Real_Noname (real_noname@coderulers.info)
(C):	CODE RULERS (Real_Noname)
WWW:	www.coderulers.info
Opis:	Zawiera definicje klas do �adowania i zarz�dzania
		modelami z plik�w GLM.

/////////////////////////////////////////////////////////
///////////////////////////////////////////////////////*/
#pragma once

#include <fstream>

#include <CBGL/COpenGL.h>

#include "core_object.h"

#include "Texture.h"	// Nagl�wek z klasa CTexture

#include "Mesh.h"
#include "MeshFuncs.h"

// Numer wersji loader'a
#define		GLM_FILE_VERSION		100

/*	G��wna klasa modeli OpenGL
*/
class CModel : public core::Object {
private:
	// Lista tekstur
	std::vector<CTexture*>	Textures;
	// Czy model jest za�adowany?
	bool loaded;
	// Czy ma animacje?
	bool animation;
	// Czy aktualnie trwa animacja
	bool playing;
	// Ostatni za�adowany plik
	cb::string file;

	// Obiekt do rysowania kwadryk ( tymczasowy, ale jest tu, by by� dost�pny w ca�ej klasie )
	//GLUquadric* obj;

	std::unique_ptr<gfx::Mesh> mesh;

public:
	CModel();
	~CModel();

	bool Load( CTexManager& texManager, const std::string& filePath );

	void Free();
	const std::string GetFile() const;
	unsigned int GetObjCount();

	void RenderObject( unsigned int index );

protected:
	virtual cb::string getLogName() const override;

private:
	std::string NoSpace( const std::string &str );
	void ParseGLCommand( const std::string &fullstr, gfx::MeshBuilderContext& ctx );
	bool GetParams( const std::string &str, size_t from, std::vector<std::string>& param, const std::string &Com );
	int GetConst( const std::string &str, const std::string &Com );

	const bool	ReadHeader( std::fstream& fileStream );
	const bool	ReadTextures( std::fstream& fileStream, CTexManager& texManager );
	const bool	ReadModel( std::fstream& fileStream, const unsigned modelIndex );

	CModel(const CModel&) = delete;
	CModel& operator=(const CModel&) = delete;
};
