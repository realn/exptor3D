/*///////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
Plik:	glm.cpp
Autor:	Real_Noname (real_noname@wp.pl)
(C):	CODE RULERS (Real_Noname)
WWW:	www.coderulers.prv.pl
Opis:	Patrz -> Model.h

/////////////////////////////////////////////////////////
///////////////////////////////////////////////////////*/
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
	playing(false),
	obj(0)
{
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
bool CModel::GetParams( const std::string &str, const int from, std::vector<std::string>& param, const std::string &Com )
{
	if( param.size() == 0 )
		return true;

	unsigned curparam = 0;
	for( unsigned i = from; i < str.length(); i++ )
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
		Log.Error( "CModel( " + file + " ): Za du�o parametr�w polecenia: " + Com );
		return false;
	}
	else if( curparam != param.size() - 1 )
	{
		Log.Error( "CModel( " + file + " ): Za ma�o parametr�w polecenia: " + Com );	
		return false;
	}

	return true;
}

/*=====METODA GetConst=====
	Metoda zwraca sta�� z OpenGL, w zale�no�ci od nazwy
	i nazwy polecenia, do jakiej jest potrzebna.
*/
int CModel::GetConst( const std::string &str, const std::string &Com )
{
	if( Com == "glEnable" || Com == "glDisable" )
	{
		if( str == "GL_DEPTH_TEST" )
			return GL_DEPTH_TEST;

		if( str == "GL_BLEND" )
			return GL_BLEND;

		if( str == "GL_TEXTURE_2D" )
			return GL_TEXTURE_2D;

		if( str == "GL_NORMALIZE" )
			return GL_NORMALIZE;

		if( str == "GL_CULL_FACE" )
			return GL_CULL_FACE;

	}
	if( Com == "glBegin" )
	{
		if( str == "GL_POINTS" )
			return GL_POINTS;

		if( str == "GL_LINES" )
			return GL_LINES;

		if( str == "GL_LINE_STRIP" )
			return GL_LINE_STRIP;

		if( str == "GL_LINE_LOOP" )
			return GL_LINE_LOOP;

		if( str == "GL_TRIANGLES" )
			return GL_TRIANGLES;

		if( str == "GL_TRIANGLE_STRIP" )
			return GL_TRIANGLE_STRIP;

		if( str == "GL_TRIANGLE_FAN" )
			return GL_TRIANGLE_FAN;

		if( str == "GL_QUADS" )
			return GL_QUADS;

		if( str == "GL_QUAD_STRIP" )
			return GL_QUAD_STRIP;

		if( str == "GL_POLYGON" )
			return GL_POLYGON;
	}
	if( Com == "glBlendFunc" )
	{
		if( str == "GL_ZERO" )
			return GL_ZERO;

		if( str == "GL_ONE" )
			return GL_ONE;
		
		if( str == "GL_DST_COLOR" )
			return GL_DST_COLOR;
		
		if( str == "GL_ONE_MINUS_DST_COLOR" )
			return GL_ONE_MINUS_DST_COLOR;
		
		if( str == "GL_SRC_ALPHA" )
			return GL_SRC_ALPHA;
		
		if( str == "GL_ONE_MINUS_SRC_ALPHA" )
			return GL_ONE_MINUS_SRC_ALPHA;
		
		if( str == "GL_DST_ALPHA" )
			return GL_DST_ALPHA;
			
		if( str == "GL_ONE_MINUS_DST_ALPHA" )
			return GL_ONE_MINUS_DST_ALPHA;
		
		if( str == "GL_SRC_ALPHA_SATURATE" )
			return GL_SRC_ALPHA_SATURATE;
	} 
	if( Com == "glFrontFace" )
	{
		if( str == "GL_CW" )
			return GL_CW;

		if( str == "GL_CCW" )
			return GL_CCW;
	}
	if( Com == "gluQuadricDrawStyle" )
	{
		if( str == "GLU_FILL" )
			return GLU_FILL;
		
		if( str == "GLU_LINE" )
			return GLU_LINE;
		
		if( str == "GLU_SILHOUETTE" )
			return GLU_SILHOUETTE;

		if( str == "GLU_POINT" )
			return GLU_POINT;
	}
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

	Log.Error( "CModel( " + file + " ): Nieznana sta�a ( " + str + " ) w poleceniu: " + Com );
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
void CModel::ParseGLCommand( const std::string &fullstr )
{
	std::string param[6], str;
	bool presult = false;
	std::string Com = "";
	int i = 0, k;

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
			k = atoi( param[0].c_str() );
			if( k >= 0 && k < Textures.size() )
				Textures[k]->Activate();
			else 
				Log.Error( "CModel( " + file + " ): B��dny parametr polecenia: " + Com );
			return;
		}
	}
	if( Com == "glCallList" )
	{
		std::vector<std::string> param(1);
		if( GetParams( str, i+1, param, Com ) && CheckParams( param ))
		{
			k = atoi( param[0].c_str() );
			if( k >= 0 && k < ListCount )
				glCallList( List + k );
			else 
				Log.Error( "CModel( " + file + " ): B��dny parametr polecenia: " + Com );
			return;
		}
	}
	if( Com == "glRotate" )
	{
		std::vector<std::string> param(4);
		if( GetParams( str, i+1, param, Com ) && CheckParams( param ))
		{
			glRotatef( atof( param[0].c_str() ), atof( param[1].c_str() ), atof( param[2].c_str() ), atof( param[3].c_str() ) );
			return;
		}
	}
	if( Com == "glTranslate" )
	{
		std::vector<std::string> param(3);
		if( GetParams( str, i+1, param, Com ) && CheckParams( param ))
		{
			glTranslatef( atof( param[0].c_str() ), atof( param[1].c_str() ), atof( param[2].c_str() ) );
			return;
		}
	}
	if( Com == "glScale" )
	{
		std::vector<std::string> param(3);
		if( GetParams( str, i+1, param, Com ) && CheckParams( param ))
		{
			glScalef( atof( param[0].c_str() ), atof( param[1].c_str() ), atof( param[2].c_str() ) );
			return;
		}
	}
	if( Com == "glBegin" )
	{
		std::vector<std::string> param(1);
		if( GetParams( str, i+1, param, Com ) && CheckParams( param ))
		{
			glBegin( GetConst( param[0], Com ) );
			return;
		}
	}
	if( Com == "glEnable" )
	{
		std::vector<std::string> param(1);
		if( GetParams( str, i+1, param, Com ) && CheckParams( param ))
		{
			glEnable( GetConst( param[0], Com ) );
			return;
		}
	}
	if( Com == "glDisable" )
	{
		std::vector<std::string> param(1);
		if( GetParams( str, i+1, param, Com ) && CheckParams( param ))
		{
			glDisable( GetConst( param[0], Com ) );
			return;
		}
	}
	if( Com == "glEnd" )
	{
		std::vector<std::string> param;
		if( GetParams( str, i+1, param, Com ) && CheckParams( param ))
		{
			glEnd();
			return;
		}
	}
	if( Com == "glPushMatrix" )
	{
		std::vector<std::string> param;
		if( GetParams( str, i+1, param, Com ) && CheckParams( param ))
		{
			glPushMatrix();
			return;
		}
	}
	if( Com == "glPopMatrix" )
	{
		std::vector<std::string> param;
		if( GetParams( str, i+1, param, Com ) && CheckParams( param ))
		{
			glPopMatrix();
			return;
		}
	}
	if( Com == "glColor3" )
	{
		std::vector<std::string> param(3);
		if( GetParams( str, i+1, param, Com ) && CheckParams( param ))
		{
			glColor3f( atof( param[0].c_str() ), atof( param[1].c_str() ), atof( param[2].c_str() ) );
			return;
		}
	}
	if( Com == "glColor4" )
	{
		std::vector<std::string> param(4);
		if( GetParams( str, i+1, param, Com ) && CheckParams( param ))
		{
			glColor4f( atof( param[0].c_str() ), atof( param[1].c_str() ), atof( param[2].c_str() ), atof( param[3].c_str() ) );
			return;
		}
	}
	if( Com == "glNormal3" )
	{
		std::vector<std::string> param(3);
		if( GetParams( str, i+1, param, Com ) && CheckParams( param ))
		{
			glNormal3f( atof( param[0].c_str() ), atof( param[1].c_str() ), atof( param[2].c_str() ) );
			return;
		}
	}
	if( Com == "glTexCoord2" )
	{
		std::vector<std::string> param(2);
		if( GetParams( str, i+1, param, Com ) && CheckParams( param ))
		{
			glTexCoord2f( atof( param[0].c_str() ), atof( param[1].c_str() ) );
			return;
		}
	}
	if( Com == "glVertex3" )
	{
		std::vector<std::string> param(3);
		if( GetParams( str, i+1, param, Com ) && CheckParams( param ))
		{
			glVertex3f( atof( param[0].c_str() ), atof( param[1].c_str() ), atof( param[2].c_str() ) );
			return;
		}
	}
	if( Com == "glBlendFunc" )
	{
		std::vector<std::string> param(2);
		if( GetParams( str, i+1, param, Com ) && CheckParams( param ))
		{
			glBlendFunc( GetConst( param[0], Com ), GetConst( param[1], Com ) );
			return;
		}
	}
	if( Com == "glFrontFace" )
	{
		std::vector<std::string> param(1);
		if( GetParams( str, i+1, param, Com ) && CheckParams( param ))
		{
			glFrontFace( GetConst( param[0], Com ) );
			return;
		}
	}
	if( Com == "gluQuadricDrawStyle" )
	{
		std::vector<std::string> param(1);
		if( GetParams( str, i+1, param, Com ) && CheckParams( param ))
		{
			gluQuadricDrawStyle( obj, GetConst( param[0], Com ) );
			return;
		}
	}
	if( Com == "gluQuadricNormals" )
	{
		std::vector<std::string> param(1);
		if( GetParams( str, i+1, param, Com ) && CheckParams( param ))
		{
			gluQuadricNormals( obj, GetConst( param[0], Com ) );
			return;
		}
	}
	if( Com == "gluQuadricOrientation" )
	{
		std::vector<std::string> param(1);
		if( GetParams( str, i+1, param, Com ) && CheckParams( param ))
		{
			gluQuadricOrientation( obj, GetConst( param[0], Com ) );
			return;
		}
	}
	if( Com == "gluQuadricTexture" )
	{
		std::vector<std::string> param(1);
		if( GetParams( str, i+1, param, Com ) && CheckParams( param ))
		{
			gluQuadricTexture( obj, GetConst( param[0], Com ) );
			return;
		}
	}
	if( Com == "gluSphere" )
	{
		std::vector<std::string> param(3);
		if( GetParams( str, i+1, param, Com ) && CheckParams( param ))
		{
			gluSphere( obj, atof( param[0].c_str() ), atoi( param[1].c_str() ), atoi( param[2].c_str() ) );
			return;
		}
	}
	if( Com == "gluCylinder" )
	{
		std::vector<std::string> param(5);
		if( GetParams( str, i+1, param, Com ) && CheckParams( param ))
		{
			gluCylinder( obj, atof( param[0].c_str() ), atof( param[1].c_str() ), atof( param[2].c_str() ), atoi( param[3].c_str() ), atoi( param[4].c_str() ) );
			return;
		}
	}
	if( Com == "gluDisk" )
	{
		std::vector<std::string> param(4);
		if( GetParams( str, i+1, param, Com ) && CheckParams( param ))
		{
			gluDisk( obj, atof( param[0].c_str() ), atof( param[1].c_str() ), atoi( param[2].c_str() ), atoi( param[3].c_str() ) );
			return;
		}
	}
	if( Com == "gluPartialDisk" )
	{
		std::vector<std::string> param(6);
		if( GetParams( str, i+1, param, Com ) && CheckParams( param ))
		{
			gluPartialDisk( obj, atof( param[0].c_str() ), atof( param[1].c_str() ), atoi( param[2].c_str() ), atoi( param[3].c_str() ), atof( param[4].c_str() ), atof( param[5].c_str() ) );
			return;
		}
	}

	Log.Error( "CModel( " + file + " ): B��dne, lub z niew�a�ciwymi parametrami polecenie: " + Com + ", oryginalny ci�g: " + str );
}

/*=====METODA CallObject=====
	Wywo�uje dan� list� wy�wietlania
*/
void CModel::CallObject( unsigned int index )
{

	if( !loaded )
		return;

	if( index >= ListCount )
		return;

	glCallList( List + index );
}

/*=====METODA ReadHeader=====
	Czyta nag��wek pliku GLM.
*/
bool CModel::ReadHeader( std::fstream& fileStream, unsigned& texCount )
{
	std::string str;

	// Pobieramy liczb� tekstur
	str = GetLine( fileStream );
	if( !sscanf_s( str.c_str(), "TEXCOUNT %u", &texCount ) )
	{
		Log.Error( "CModel( " + file + " ): Nie mo�na odczyta� liczby tekstur!" );
		return false;
	}

	// Pobieramy liczb� obiekt�w
	str = GetLine( fileStream );
	if( !sscanf_s( str.c_str(), "LISTCOUNT %u", &ListCount ) )
	{
		Log.Error( "CModel( " + file + " ): Nie mo�na odczyta� liczby obiekt�w!" );
		return false;
	}

	// Pobieramy, czy plik ma zapisan� animacj�
	str = GetLine( fileStream );
	if( !sscanf_s( str.c_str(), "ANIMATION %u", &animation ) )
	{
		Log.Error( "CModel( " + file + " ): Nie mo�na odczyta� animacji!" );
		return false;
	}

	// Sprawdzamy czy to koniec nag��wka
	str = GetLine( fileStream );
	if( str != "END HEADER" )
	{
		Log.Error( "CModel( " + file + " ): Brak ko�ca nag��wka!" );
		return false;
	}

	return true;
}

bool CModel::LoadModel( CTexManager& texManager, std::string filename )
{
	// Sprawdzamy czy �a�cuch nie jest pusty
	if( filename.empty() )
	{
		Log.Error( "CModel( " + file + " ): Pusty ci�g nazwy pliku!" );
		return false;
	}
	
	// zmienna tymczasowa, trzymaj�ca jedn� linie z pliku
	std::string str;

	// zmienna trzymaj�ca numer wersji pliku
	int Version = 0;

	int i;

	// Pr�bujemy otworzy� plik
	std::fstream fileStream(filename, std::ios::in);

	// Sprawdzamy po��czenie
	if( !fileStream )
	{
		Log.Error( "CModel( " + file + " ): plik " + filename + " nie istnieje lub �cie�ka niew�a�ciwa." );
		return false;
	}

	// Pobieramy pierwsz� linie
	str = GetLine( fileStream );

	// Skanujemy lini� w poszukiwaniu numeru wersji
	if( !sscanf_s( str.c_str(), "GLM %d", &Version ) )
	{
		Log.Error( "CModel( " + file + " ): Nieprawid�owa pierwsza linia pliku " + filename + "!" );
		return false;
	}

	// Sprawdzamy wersj�
	if( Version > GLM_VERSION )
	{
		Log.Error( "CModel( " + file + " ): Zbyt wysoka wersja pliku!" );
		return false;
	}

	// Teraz sprawdzamy, czy ju� jaki� obiekt nie by� za�adowany
	if( loaded )
	{
		Log.Report( "CModel( " + file + " ): Prze�adowanie pliku na " + filename );
		Free();
	}

	Log.Log( "CModel( " + file + " ): �adowanie modelu z pliku " + filename );
	file = filename;

	str = GetLine( fileStream );

	unsigned texCount = 0;

	// Czytamy nag��wek
	if( str == "HEADER" )
	{
		if( !ReadHeader( fileStream, texCount ) )
		{
			Log.Error( "CModel( " + file + " ): B��d w nag��wku!" );
			return false;
		}
	}
	else
	{
		Log.Error( "CModel( " + file + " ): Brak nag��wka!" );
		return false;
	}

	if( animation )
	{
		str = GetLine( fileStream );

		// Czytamy nag��wek
		if( str == "ANIMHEADER" )
		{
			if( !ReadAnimHeader( fileStream ) )
			{
				Log.Error( "CModel( " + file + " ): B��d w nag��wku animacji!" );
				return false;
			}
		}
		else
		{
			Log.Error( "CModel( " + file + " ): Brak nag��wka animacji!" );
			return false;
		}
	}
	
	// Czytamy tekstury
	if( texCount > 0 )
	{
		Textures.resize(texCount);
		memset(&Textures[0], 0, sizeof(CTexture*) * texCount);

		str = GetLine( fileStream );

		if( str == "TEXLIST" )
		{
			for( i = 0; i < Textures.size(); i++ )
			{
				str = GetLine( fileStream );

				if( !( Textures[i] = texManager.Get( str ) ) )
				{
					Log.Error( "CModel( " + file + " ): B��d przy �adowaniu tekstury!" );
				}

				if( str == "END TEXLIST" )
				{
					Log.Error( "CModel( " + file + " ): Przedwczesny koniec listy tekstur!" );
					break;
				}
			}
			
			if( str != "END TEXLIST" )
			{
				str = GetLine( fileStream );
				if( str != "END TEXLIST" )
				{
					Log.Error( "CModel( " + file + " ): Brak ko�ca listy tekstur!" );
					Free();
					return false;
				}
			}
		}
		else
		{
			Log.Error( "CModel( " + file + " ): Brak listy tekstur!" );
			Free();
			return false;
		}
	}

	obj = gluNewQuadric();
	gluQuadricTexture( obj, GL_TRUE );

	// Czytamy modele
	if( ListCount > 0 )
	{
		List = glGenLists( ListCount );

		for( i = 0; i < ListCount; i++ )
		{
			str = GetLine( fileStream );

			int j;

			sscanf_s( str.c_str(), "MODEL %d", &j );
			
			if( i != j )
			{
				Log.Error( "CModel( " + file + " ): B��d, z�a kolejno�� modeli!" );
				Free();
				return false;
			}

			glNewList( List + i, GL_COMPILE );

			do
			{
				str = GetLine( fileStream );

				if( str != "END MODEL" )
				{
					ParseGLCommand( str );
				}
			}
			while( str != "END MODEL" );

			glEndList();
		}
	}

	if( animation )
	{
		if( FrameCount > 0 )
		{
			Frame = glGenLists( FrameCount );

			for( i = 0; i < FrameCount; i++ )
			{
				str = GetLine( fileStream );

				int j;

				sscanf_s( str.c_str(), "FRAME %d", &j );
				
				if( i != j )
				{
					Log.Error( "CModel( " + file + " ): B��d, z�a kolejno�� klatek!" );
					Free();
					return false;				
				}

				glNewList( Frame + i, GL_COMPILE );

				do
				{
					str = GetLine( fileStream );

					if( str != "END FRAME" )
					{
						ParseGLCommand( str );
					}
				}
				while( str != "END FRAME" );

				glEndList();
			}
		}	
	}

	str = GetLine( fileStream );

	if( str == "END GLM" )
	{
		loaded = true;
		return true;
	}

	Log.Error( "CModel( " + file + " ): Brak ko�ca pliku!" );
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
		glDeleteLists( List, ListCount );
		List = 0;
		ListCount = 0;
	}

	if( Frame != 0 && FrameCount > 0 )
	{
		glDeleteLists( Frame, FrameCount );
		Frame = 0;
		FrameCount = 0;
	}

	if(obj != 0)
	{
		gluDeleteQuadric( obj );
		obj = 0;
	}

	loaded = false;
}

std::string CModel::GetFile()
{
	return file;
}

unsigned int CModel::GetObjCount()
{
	return ListCount;
}

//===========METODY NIETESTOWANE!!!============
bool CModel::ReadAnimHeader( std::fstream& fileStream )
{
	std::string str;

	// Pobieramy liczb� tekstur
	str = GetLine( fileStream );
	if( !sscanf_s( str.c_str(), "FRAMECOUNT %u", &FrameCount ) )
	{
		Log.Error( "CModel( " + file + " ): Nie mo�na odczyta� liczby klatek animacji!" );
		return false;
	}

	// Sprawdzamy czy to koniec nag��wka
	str = GetLine( fileStream );
	if( str != "END ANIMHEADER" )
	{
		Log.Error( "CModel( " + file + " ): Brak ko�ca nag��wka animacji!" );
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
		CallObject( index );
		return;
	}

	glCallList( Frame + CurrFrame );
}
