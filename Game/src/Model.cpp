/*///////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
Plik:	glm.cpp
Autor:	Real_Noname (real_noname@wp.pl)
(C):	CODE RULERS (Real_Noname)
WWW:	www.coderulers.prv.pl
Opis:	Patrz -> Model.h

/////////////////////////////////////////////////////////
///////////////////////////////////////////////////////*/
#include <CBCore/StringConvert.h>

#include <CBGL/COpenGL.h>

#include "MeshFuncs.h"

#include "Model.h"
#include "Log.h"
#include "StrEx.h"

const unsigned BUFFER_SIZE = 1024;

/*=====KONSTRUKTOR=====*/
CModel::CModel() :
	core::Object(L"Model"),
	loaded(false),
	file(L"-"),
	animation(false),
	playing(false)
{
	mesh = std::make_unique<gfx::Mesh>();
}

/*=====DESTRUKTOR=====*/
CModel::~CModel()
{
	// Zwolnienie pami�ci
	Free();
}

/*=====METODA NoSpace=====
Czy�ci podan� lini� ze spacji, tablulacji
i znak�w nowej linii.
*/
std::string CModel::NoSpace( const std::string &str )
{
	std::string result = "";

	for( unsigned int i = 0; i < str.length(); i++ )
	{
		if(IsWhiteSpace(str[i]))
			continue;

		result += str[i];
	}

	return result;
}

/*=====METODA GetParams=====
Pobiera dowoln� ilo�� parametr�w pomi�dzy
nawiasami ( ) odzielonymi przecinkiem i je
wpisuje do podanej tablicy.
*/
bool CModel::GetParams( const std::string &str, size_t from, std::vector<std::string>& param, const std::string &Com )
{
	if( param.size() == 0 )
		return true;

	unsigned curparam = 0;
	for( auto i = from; i < str.length(); i++ )
	{
		if( str[i] == ')' )
			break;

		if( str[i] == ',' )
		{
			curparam++;
			i++;

			if( curparam >= param.size() )
				break;
		}

		param[curparam] += str[i];
	}

	if( curparam > param.size() )
	{
		error( "Za du�o parametr�w polecenia: " + Com );
		return false;
	}
	else if( curparam != param.size() - 1 )
	{
		error( "Za ma�o parametr�w polecenia: " + Com );	
		return false;
	}

	return true;
}

gfx::MeshBuilderContext::VertListType getVertListFromStr(const std::string& str) {
	if (str == "GL_TRIANGLES")
		return gfx::MeshBuilderContext::VertListType::TRIANGLES;

	if (str == "GL_TRIANGLE_STRIP")
		return gfx::MeshBuilderContext::VertListType::TRIANGLE_STRIP;

	if (str == "GL_TRIANGLE_FAN")
		return gfx::MeshBuilderContext::VertListType::TRIANGLE_FAN;

	if (str == "GL_QUADS")
		return gfx::MeshBuilderContext::VertListType::QUADS;

	if (str == "GL_QUAD_STRIP")
		return gfx::MeshBuilderContext::VertListType::QUAD_STRIP;

	return gfx::MeshBuilderContext::VertListType::TRIANGLES;
}

/*=====METODA GetConst=====
Metoda zwraca sta�� z OpenGL, w zale�no�ci od nazwy
i nazwy polecenia, do jakiej jest potrzebna.
*/
int CModel::GetConst( const std::string &str, const std::string &Com )
{
	if( Com == "gluQuadricNormals" )
	{
		if( str == "GLU_NONE" )
			return GLU_NONE;

		if( str == "GLU_FLAT" )
			return GLU_FLAT;

		if( str == "GLU_SMOOTH" )
			return GLU_SMOOTH;
	}
	if( Com == "gluQuadricOrientation" )
	{
		if( str == "GLU_OUTSIDE" )
			return GLU_OUTSIDE;

		if( str == "GLU_INSIDE" )
			return GLU_INSIDE;
	}
	if( Com == "gluQuadricTexture" )
	{
		if( str == "GL_TRUE" )
			return GL_TRUE;

		if( str == "GL_FALSE" )
			return GL_FALSE;
	}

	error( "Nieznana sta�a ( " + str + " ) w poleceniu: " + Com );
	return atoi( str.c_str() );
}

bool	CheckParams(std::vector<std::string>& param)
{
	for(unsigned i = 0; i < param.size(); i++)
	{
		if(param[i].empty())
			return false;
	}
	return true;
}

/*=====METODA ParseGLCommand=====
Analizuje linie, szuka nazwy komendy OpenGL, pobiera
dla niej argumenty i j� wykonuje.
*/
void CModel::ParseGLCommand( const std::string &fullstr, gfx::MeshBuilderContext& ctx)
{
	std::string param[6], str;
	bool presult = false;
	std::string Com = "";
	size_t i = 0;

	str = ClearWhiteSpace( fullstr );

	i = str.find("(");
	if( i != std::string::npos )
		Com = str.substr(0, i);
	else
		Com = str;

	if( Com == "glBindTexture" )
	{
		std::vector<std::string> param(1);
		if( GetParams( str, i+1, param, Com ) && CheckParams( param ))
		{
			auto texId = StrToUInt( param[0] );
			if( texId < Textures.size() )
				Textures[texId]->Activate();
			else 
				error( "B��dny parametr polecenia: " + Com );
			return;
		}
	}
	if( Com == "glRotate" )
	{
		std::vector<std::string> param(4);
		if( GetParams( str, i+1, param, Com ) && CheckParams( param ))
		{
			ctx.rotate(StrToFloat( param[0] ), StrToFloat( param[1] ), StrToFloat( param[2] ), StrToFloat( param[3] ) );
			return;
		}
	}
	if( Com == "glTranslate" )
	{
		std::vector<std::string> param(3);
		if( GetParams( str, i+1, param, Com ) && CheckParams( param ))
		{
			ctx.translate(StrToFloat(param[0]), StrToFloat(param[1]), StrToFloat(param[2]));
			return;
		}
	}
	if( Com == "glScale" )
	{
		std::vector<std::string> param(3);
		if( GetParams( str, i+1, param, Com ) && CheckParams( param ))
		{
			ctx.scale( StrToFloat( param[0] ), StrToFloat( param[1] ), StrToFloat( param[2] ) );
			return;
		}
	}
	if( Com == "glBegin" )
	{
		std::vector<std::string> param(1);
		if( GetParams( str, i+1, param, Com ) && CheckParams( param ))
		{
			ctx.beginVertexList(getVertListFromStr(param[0]));
			return;
		}
	}
	if( Com == "glEnd" )
	{
		std::vector<std::string> param;
		if( GetParams( str, i+1, param, Com ) && CheckParams( param ))
		{
			ctx.commitVertexList(*mesh);
			return;
		}
	}
	if( Com == "glPushMatrix" )
	{
		std::vector<std::string> param;
		if( GetParams( str, i+1, param, Com ) && CheckParams( param ))
		{
			ctx.pushMatrix();
			return;
		}
	}
	if( Com == "glPopMatrix" )
	{
		std::vector<std::string> param;
		if( GetParams( str, i+1, param, Com ) && CheckParams( param ))
		{
			ctx.popMatrix();
			return;
		}
	}
	if( Com == "glNormal3" )
	{
		std::vector<std::string> param(3);
		if( GetParams( str, i+1, param, Com ) && CheckParams( param ))
		{
			ctx.setVertexNormal( StrToFloat( param[0] ), StrToFloat( param[1] ), StrToFloat( param[2] ) );
			return;
		}
	}
	if( Com == "glTexCoord2" )
	{
		std::vector<std::string> param(2);
		if( GetParams( str, i+1, param, Com ) && CheckParams( param ))
		{
			ctx.setVertexTCoord( StrToFloat( param[0] ), StrToFloat( param[1] ) );
			return;
		}
	}
	if( Com == "glVertex3" )
	{
		std::vector<std::string> param(3);
		if( GetParams( str, i+1, param, Com ) && CheckParams( param ))
		{
			ctx.addVertex( StrToFloat( param[0] ), StrToFloat( param[1] ), StrToFloat( param[2] ) );
			return;
		}
	}
	if( Com == "gluQuadricNormals" )
	{
		std::vector<std::string> param(1);
		if( GetParams( str, i+1, param, Com ) && CheckParams( param ))
		{
			//gluQuadricNormals( obj, GetConst( param[0], Com ) );
			return;
		}
	}
	if( Com == "gluQuadricOrientation" )
	{
		std::vector<std::string> param(1);
		if( GetParams( str, i+1, param, Com ) && CheckParams( param ))
		{
			ctx.setVerticesInverted(param[0] == "GLU_INSIDE");
			return;
		}
	}
	if( Com == "gluQuadricTexture" )
	{
		std::vector<std::string> param(1);
		if( GetParams( str, i+1, param, Com ) && CheckParams( param ))
		{
			//gluQuadricTexture( obj, GetConst( param[0], Com ) );
			return;
		}
	}
	if( Com == "gluSphere" )
	{
		std::vector<std::string> param(3);
		if( GetParams( str, i+1, param, Com ) && CheckParams( param ))
		{
			ctx.addSphere(*mesh, StrToFloat( param[0] ), StrToInt( param[1] ), StrToInt( param[2] ) );
			return;
		}
	}
	if( Com == "gluCylinder" )
	{
		std::vector<std::string> param(5);
		if( GetParams( str, i+1, param, Com ) && CheckParams( param ))
		{
			ctx.addCylinder( *mesh, StrToFloat( param[0] ), StrToFloat( param[1] ), StrToFloat( param[2] ), StrToInt( param[3] ), StrToInt( param[4] ) );
			return;
		}
	}
	if( Com == "gluDisk" )
	{
		std::vector<std::string> param(4);
		if( GetParams( str, i+1, param, Com ) && CheckParams( param ))
		{
			ctx.addDisk(*mesh, StrToFloat(param[0]), StrToFloat(param[1]), StrToUInt(param[2]), StrToUInt(param[3]));
			return;
		}
	}
	if( Com == "gluPartialDisk" )
	{
		std::vector<std::string> param(6);
		if( GetParams( str, i+1, param, Com ) && CheckParams( param ))
		{
			ctx.addPartialDisk( *mesh, StrToFloat( param[0] ), StrToFloat( param[1] ), StrToInt( param[2] ), StrToInt( param[3] ), StrToFloat( param[4] ), StrToFloat( param[5] ) );
			return;
		}
	}

	error( "B��dne, lub z niew�a�ciwymi parametrami polecenie: " + Com + ", oryginalny ci�g: " + str );
}

/*=====METODA RenderObject=====
Wywo�uje dan� list� wy�wietlania
*/
void CModel::RenderObject( unsigned int index )
{
	if( !loaded )
		return;

	mesh->render();
}

/*=====METODA ReadHeader=====
Czyta nag��wek pliku GLM.
*/
const bool	CModel::ReadHeader( std::fstream& fileStream )
{
	std::string str, cmd;

	while( fileStream )
	{
		str = GetLine( fileStream );
		if( str.empty() )
			continue;

		// Sprawdzamy czy to koniec nag��wka
		if( str == "END HEADER" )
			return true;

		auto pos = str.find( " " );
		if( pos == std::string::npos )
		{
			error( "Ci�g nie zawiera spacji: " + str );
			continue;
		}

		cmd = str.substr( 0, pos );
		if( cmd == "TEXCOUNT" )
		{
			unsigned texCount = 0;
			// Pobieramy liczb� tekstur
			if( !sscanf_s( str.c_str(), "TEXCOUNT %u", &texCount ) )
			{
				error( "Nie mo�na odczyta� liczby tekstur!" );
				continue;
			}
		}
		else if( cmd == "ANIMATION" )
		{
			// Pobieramy, czy plik ma zapisan� animacj�
			unsigned set = 0;
			if( !sscanf_s( str.c_str(), "ANIMATION %u", &set ) )
			{
				error( "Nie mo�na odczyta� animacji!" );
				return false;
			}
			else
				animation = set != 0 ? true : false;
		}
		else
			error( "Nierozpoznany ci�g nag��wka: " + str );
	}

	error( "Brak ko�ca nag��wka!" );
	return false;
}

const bool	CModel::ReadTextures( std::fstream& fileStream, CTexManager& texManager )
{
	std::string str;
	CTexture* pTex = nullptr;

	while( fileStream )
	{
		str = GetLine( fileStream );
		if( str.empty() )
			continue;

		if( str == "END TEXLIST" )
			return true;

		pTex = texManager.Get( str );
		if( pTex == nullptr )
			error( "B��d przy �adowaniu tekstury!" );
		Textures.push_back( pTex );

		pTex = nullptr;
	}

	error( "Brak ko�ca listy tekstur!" );
	return false;
}

const bool	CModel::ReadModel( std::fstream& fileStream, const unsigned modelIndex )
{
	std::string str;
	gfx::MeshBuilderContext context;

	while( fileStream )
	{
		str = GetLine( fileStream );
		if( str.empty() )
			continue;

		if( str == "END MODEL" )
			break;

		ParseGLCommand( str, context );
	}


	mesh->prepare();

	return true;
}

bool CModel::Load( CTexManager& texManager, const std::string& filename )
{
	// Sprawdzamy czy �a�cuch nie jest pusty
	if( filename.empty() )
	{
		error( "Pusty ci�g nazwy pliku!" );
		return false;
	}

	// zmienna tymczasowa, trzymaj�ca jedn� linie z pliku
	std::string str;

	// zmienna trzymaj�ca numer wersji pliku
	int Version = 0;

	// Pr�bujemy otworzy� plik
	std::fstream fileStream(filename, std::ios::in);

	// Sprawdzamy po��czenie
	if( !fileStream )
	{
		error( "Plik " + filename + " nie istnieje lub �cie�ka niew�a�ciwa." );
		return false;
	}

	// Pobieramy pierwsz� linie
	str = GetLine( fileStream );

	// Skanujemy lini� w poszukiwaniu numeru wersji
	if( !sscanf_s( str.c_str(), "GLM %d", &Version ) )
	{
		error( "Nieprawid�owa pierwsza linia pliku " + filename + "!" );
		return false;
	}


	// Sprawdzamy wersj�
	if( Version > GLM_FILE_VERSION )
	{
		error( "Zbyt wysoka wersja pliku!" );
		return false;
	}

	// Teraz sprawdzamy, czy ju� jaki� obiekt nie by� za�adowany
	if( loaded )
	{
		report( "Prze�adowanie pliku na " + filename );
		Free();
	}

	log( "�adowanie modelu z pliku " + filename );
	file = cb::fromUtf8(filename);

	while( fileStream )
	{
		str = GetLine( fileStream );
		if( str.empty() )
			continue;

		if( str == "END GLM" )
		{
			loaded = true;
			return true;
		}

		if( str == "HEADER" )
		{
			// Czytamy nag��wek
			if( !ReadHeader( fileStream ) )
			{
				error( "B��d w nag��wku!" );
				continue;
			}
		}
		else if( str == "TEXLIST" )
		{
			if( !ReadTextures( fileStream, texManager ) )
			{
				error( "B��d odczytu listy tekstur." );
				continue;
			}
		}
		else if( ContainsString( str, "MODEL" ) )
		{
			unsigned modelIndex = 0;
			if( !( sscanf_s( str.c_str(), "MODEL %u", &modelIndex ) && ReadModel( fileStream, modelIndex ) ) )
			{
				error( "B��d odczytu modelu." );
				continue;
			}
		}
		else
			error( "Nierozpoznany ci�g: " + str );
	}

	error( "Brak ko�ca pliku!" );
	return false;
}

void CModel::Free()
{
	if( Textures.size() > 0 )
	{
		Textures.clear();
	}

	mesh.reset();

	file.clear();
	loaded = false;
}

const std::string CModel::GetFile() const
{
	return cb::toUtf8(file);
}

unsigned int CModel::GetObjCount()
{
	return 1;
}

cb::string CModel::getLogName() const {
	return getClassName() + L"(" + file + L"):";
}
