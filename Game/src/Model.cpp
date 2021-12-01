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
	List(0),
	ListCount(0),
	Frame(0),
	FrameCount(0),
	loaded(false),
	file("-"),
	CurrFrame(0),
	FromFrame(0),
	ToFrame(0),
	animation(false),
	playing(false)
	//obj(0)
{
	//obj = gluNewQuadric();
	//gluQuadricTexture( obj, GL_TRUE );

	mesh = std::make_unique<gfx::Mesh>();
}

/*=====DESTRUKTOR=====*/
CModel::~CModel()
{
	// Zwolnienie pamiêci
	Free();
}

/*=====METODA NoSpace=====
Czyœci podan¹ liniê ze spacji, tablulacji
i znaków nowej linii.
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
Pobiera dowoln¹ iloœæ parametrów pomiêdzy
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
		Log.Error( "CModel( " + file + " ): Za du¿o parametrów polecenia: " + Com );
		return false;
	}
	else if( curparam != param.size() - 1 )
	{
		Log.Error( "CModel( " + file + " ): Za ma³o parametrów polecenia: " + Com );	
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
Metoda zwraca sta³¹ z OpenGL, w zale¿noœci od nazwy
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

	Log.Error( "CModel( " + file + " ): Nieznana sta³a ( " + str + " ) w poleceniu: " + Com );
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
dla niej argumenty i j¹ wykonuje.
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
				Log.Error( "CModel( " + file + " ): B³êdny parametr polecenia: " + Com );
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

	Log.Error( "CModel( " + file + " ): B³êdne, lub z niew³aœciwymi parametrami polecenie: " + Com + ", oryginalny ci¹g: " + str );
}

/*=====METODA RenderObject=====
Wywo³uje dan¹ listê wyœwietlania
*/
void CModel::RenderObject( unsigned int index )
{
	if( !loaded )
		return;

	//if( index >= ListCount )
	//	return;

	//glCallList( List + index );

	mesh->render();
}

/*=====METODA ReadHeader=====
Czyta nag³ówek pliku GLM.
*/
const bool	CModel::ReadHeader( std::fstream& fileStream )
{
	std::string str, cmd;

	while( fileStream )
	{
		str = GetLine( fileStream );
		if( str.empty() )
			continue;

		// Sprawdzamy czy to koniec nag³ówka
		if( str == "END HEADER" )
			return true;

		auto pos = str.find( " " );
		if( pos == std::string::npos )
		{
			Log.Error( "CModel( " + file + " ): Ci¹g nie zawiera spacji: " + str );
			continue;
		}

		cmd = str.substr( 0, pos );
		if( cmd == "TEXCOUNT" )
		{
			unsigned texCount = 0;
			// Pobieramy liczbê tekstur
			if( !sscanf_s( str.c_str(), "TEXCOUNT %u", &texCount ) )
			{
				Log.Error( "CModel( " + file + " ): Nie mo¿na odczytaæ liczby tekstur!" );
				continue;
			}
		}
		else if( cmd == "LISTCOUNT" )
		{
			// Pobieramy liczbê obiektów
			if( !sscanf_s( str.c_str(), "LISTCOUNT %u", &ListCount ) )
			{
				Log.Error( "CModel( " + file + " ): Nie mo¿na odczytaæ liczby obiektów!" );
				return false;
			}
			else
				List = glGenLists( ListCount );
		}
		else if( cmd == "ANIMATION" )
		{
			// Pobieramy, czy plik ma zapisan¹ animacjê
			unsigned set = 0;
			if( !sscanf_s( str.c_str(), "ANIMATION %u", &set ) )
			{
				Log.Error( "CModel( " + file + " ): Nie mo¿na odczytaæ animacji!" );
				return false;
			}
			else
				animation = set != 0 ? true : false;
		}
		else
			Log.Error( "CModel( " + file + " ): Nierozpoznany ci¹g nag³ówka: " + str );
	}

	Log.Error( "CModel( " + file + " ): Brak koñca nag³ówka!" );
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
			Log.Error( "CModel( " + file + " ): B³¹d przy ³adowaniu tekstury!" );
		Textures.push_back( pTex );

		pTex = nullptr;
	}

	Log.Error( "CModel( " + file + " ): Brak koñca listy tekstur!" );
	return false;
}

const bool	CModel::ReadModel( std::fstream& fileStream, const unsigned modelIndex )
{
	if( List == 0 || ListCount == 0 )
	{
		Log.Error( "CModel( " + file + " ): Call list not initialized, missing LISTCOUNT in header?");
		return false;
	}

	std::string str;
	gfx::MeshBuilderContext context;
	//glNewList( List + modelIndex, GL_COMPILE );

	while( fileStream )
	{
		str = GetLine( fileStream );
		if( str.empty() )
			continue;

		if( str == "END MODEL" )
			break;

		ParseGLCommand( str, context );
	}

	//glEndList();

	mesh->prepare();

	return true;
}

bool CModel::LoadModel( CTexManager& texManager, const std::string& filename )
{
	// Sprawdzamy czy ³añcuch nie jest pusty
	if( filename.empty() )
	{
		Log.Error( "CModel( " + file + " ): Pusty ci¹g nazwy pliku!" );
		return false;
	}

	// zmienna tymczasowa, trzymaj¹ca jedn¹ linie z pliku
	std::string str;

	// zmienna trzymaj¹ca numer wersji pliku
	int Version = 0;

	// Próbujemy otworzyæ plik
	std::fstream fileStream(filename, std::ios::in);

	// Sprawdzamy po³¹czenie
	if( !fileStream )
	{
		Log.Error( "CModel( " + file + " ): plik " + filename + " nie istnieje lub œcie¿ka niew³aœciwa." );
		return false;
	}

	// Pobieramy pierwsz¹ linie
	str = GetLine( fileStream );

	// Skanujemy liniê w poszukiwaniu numeru wersji
	if( !sscanf_s( str.c_str(), "GLM %d", &Version ) )
	{
		Log.Error( "CModel( " + file + " ): Nieprawid³owa pierwsza linia pliku " + filename + "!" );
		return false;
	}


	// Sprawdzamy wersjê
	if( Version > GLM_FILE_VERSION )
	{
		Log.Error( "CModel( " + file + " ): Zbyt wysoka wersja pliku!" );
		return false;
	}

	// Teraz sprawdzamy, czy ju¿ jakiœ obiekt nie by³ za³adowany
	if( loaded )
	{
		Log.Report( "CModel( " + file + " ): Prze³adowanie pliku na " + filename );
		Free();
	}

	Log.Log( "CModel( " + file + " ): £adowanie modelu z pliku " + filename );
	file = filename;

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
			// Czytamy nag³ówek
			if( !ReadHeader( fileStream ) )
			{
				Log.Error( "CModel( " + file + " ): B³¹d w nag³ówku!" );
				continue;
			}
		}
		else if( str == "ANIMHEADER" )
		{
			if( !ReadAnimHeader( fileStream ) )
			{
				Log.Error( "CModel( " + file + " ): B³¹d w nag³ówku animacji!" );
				continue;
			}
		}
		else if( str == "TEXLIST" )
		{
			if( !ReadTextures( fileStream, texManager ) )
			{
				Log.Error( "CModel( " + file + " ): B³¹d odczytu listy tekstur." );
				continue;
			}
		}
		else if( ContainsString( str, "MODEL" ) )
		{
			unsigned modelIndex = 0;
			if( !( sscanf_s( str.c_str(), "MODEL %u", &modelIndex ) && ReadModel( fileStream, modelIndex ) ) )
			{
				Log.Error( "CModel( " + file + " ): B³¹d odczytu modelu." );
				continue;
			}
		}
		else
			Log.Error( "CModel( " + file + " ): Nierozpoznany ci¹g: " + str );
	}

	Log.Error( "CModel( " + file + " ): Brak koñca pliku!" );
	return false;
}

void CModel::Free()
{
	if( Textures.size() > 0 )
	{
		Textures.clear();
	}

	if( List != 0 && ListCount > 0 )
	{
		//glDeleteLists( List, ListCount );
		List = 0;
		ListCount = 0;
	}

	if( Frame != 0 && FrameCount > 0 )
	{
		//glDeleteLists( Frame, FrameCount );
		Frame = 0;
		FrameCount = 0;
	}

	//if(obj != 0)
	//{
	//	gluDeleteQuadric( obj );
	//	obj = 0;
	//}

	mesh.reset();

	file.clear();
	loaded = false;
}

const std::string CModel::GetFile() const
{
	return file;
}

unsigned int CModel::GetObjCount()
{
	return ListCount;
}

//===========METODY NIETESTOWANE!!!============
const bool CModel::ReadAnimHeader( std::fstream& fileStream )
{
	std::string str;

	// Pobieramy liczbê tekstur
	str = GetLine( fileStream );
	if( !sscanf_s( str.c_str(), "FRAMECOUNT %u", &FrameCount ) )
	{
		Log.Error( "CModel( " + file + " ): Nie mo¿na odczytaæ liczby klatek animacji!" );
		return false;
	}

	// Sprawdzamy czy to koniec nag³ówka
	str = GetLine( fileStream );
	if( str != "END ANIMHEADER" )
	{
		Log.Error( "CModel( " + file + " ): Brak koñca nag³ówka animacji!" );
		return false;
	}

	return true;
}

void CModel::PlayAnim( unsigned int fromframe, unsigned int toframe, bool canskip )
{
	if( !animation )
		return;

	if( fromframe >= FrameCount || toframe < fromframe || toframe > FrameCount )
		return;

	if( playing && !canskip )
		return;

	FromFrame = fromframe;
	CurrFrame = fromframe;
	ToFrame = toframe;
	playing = true;
}

void CModel::UpdateAnim()
{
	if( !playing )
		return;

	CurrFrame++;

	if( CurrFrame > ToFrame )
		playing = false;
}

void CModel::RenderAnim( unsigned int index )
{
	if( !animation || !playing )
	{
		RenderObject( index );
		return;
	}

	glCallList( Frame + CurrFrame );
}
