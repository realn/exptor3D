/*///////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
Plik:	glm.cpp
Autor:	Real_Noname (real_noname@wp.pl)
(C):	CODE RULERS (Real_Noname)
WWW:	www.coderulers.prv.pl
Opis:	Patrz -> glm.h

/////////////////////////////////////////////////////////
///////////////////////////////////////////////////////*/
#include "glm.h"
#include "Log.h"

const unsigned BUFFER_SIZE = 1024;

/*=====KONSTRUKTOR=====*/
GLModel::GLModel() :
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
GLModel::~GLModel()
{
	// Zwolnienie pamiêci
	Free();
}

bool	IsWhiteSpace(char Character)
{
	return Character == ' ' || Character == '\t' || Character == '\r' || Character == '\n';
}

/*=====METODA GetString=====
	Metoda pobiera jedn¹ linie z pliku
	i j¹ modyfikuje. Pozbywa siê pustej
	linii, znaków spacji i tabulacji przed
	rozpoczêciem w³aœciwych poleceñ, oraz
	czyœci z komentarzy.
*/
std::string	GLModel::GetString( std::fstream& fileStream )
{
	char	buf[BUFFER_SIZE + 1];
	int		len = 0;
	bool	repeat = false;
	bool	wfchar = false;

	unsigned pos = 0;
	std::string line;
	std::string result;

	do
	{
		repeat = false;
		wfchar = false;

		memset(buf, 0, BUFFER_SIZE + 1);
		fileStream.getline(buf, BUFFER_SIZE);
		line = buf;

		if(line.empty() && fileStream)
		{
			repeat = true;
			continue;
		}
	
		pos = 0;
		for( ; pos < line.length(); pos++ )
		{
			if(!IsWhiteSpace(line[pos]))
				break;
		}

		for( ; pos < line.length(); pos++ )
		{
			if( line[pos] == '#' )
			{
				repeat = true;
				break;
			}

			if(IsWhiteSpace(line[pos]))
			{
				if(!wfchar)
				{
					wfchar = true;
					result += ' ';
				}
				continue;
			}
			else
				wfchar = false;

			result += line[pos];
		}
	}
	while( repeat );

	return result;
}

/*=====METODA NoSpace=====
	Czyœci podan¹ liniê ze spacji, tablulacji
	i znaków nowej linii.
*/
std::string GLModel::NoSpace( const std::string &str )
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
bool GLModel::GetParams( const std::string &str, const int from, std::vector<std::string>& param, const std::string &Com )
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
		Log.Error( "GLMODEL( " + file + " ): Za du¿o parametrów polecenia: " + Com );
		return false;
	}
	else if( curparam != param.size() - 1 )
	{
		Log.Error( "GLMODEL( " + file + " ): Za ma³o parametrów polecenia: " + Com );	
		return false;
	}

	return true;
}

/*=====METODA GetConst=====
	Metoda zwraca sta³¹ z OpenGL, w zale¿noœci od nazwy
	i nazwy polecenia, do jakiej jest potrzebna.
*/
int GLModel::GetConst( const std::string &str, const std::string &Com )
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

	Log.Error( "GLMODEL( " + file + " ): Nieznana sta³a ( " + str + " ) w poleceniu: " + Com );
	return atoi( str.c_str() );
}

bool	CheckParams(std::vector<std::string>& param)
{
	for(unsigned i = 0; i < param.size(); i++)
	{
		if(param[i] == "" || param.empty())
			return false;
	}
	return true;
}

/*=====METODA ParseGLCommand=====
	Analizuje linie, szuka nazwy komendy OpenGL, pobiera
	dla niej argumenty i j¹ wykonuje.
*/
void GLModel::ParseGLCommand( const std::string &fullstr )
{
	std::string param[6], str;
	bool presult = false;
	std::string Com = "";
	int i = 0, k;

	str = NoSpace( fullstr );

	for( i = 0; i < str.length(); i++ )
	{
		if( str[i] == '(' )
			break;

		Com += str[i];
	}

	if( Com == "glBindTexture" )
	{
		std::vector<std::string> param(1);
		if( GetParams( str, i+1, param, Com ) && CheckParams( param ))
		{
			k = atoi( param[0].c_str() );
			if( k >= 0 && k < Textures.size() )
				Textures[k]->Activate();
			else 
				Log.Error( "GLMODEL( " + file + " ): B³êdny parametr polecenia: " + Com );
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
				Log.Error( "GLMODEL( " + file + " ): B³êdny parametr polecenia: " + Com );
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

	Log.Error( "GLMODEL( " + file + " ): B³êdne, lub z niew³aœciwymi parametrami polecenie: " + Com + ", oryginalny ci¹g: " + str );
}

/*=====METODA CallObject=====
	Wywo³uje dan¹ listê wyœwietlania
*/
void GLModel::CallObject( unsigned int index )
{

	if( !loaded )
		return;

	if( index >= ListCount )
		return;

	glCallList( List + index );
}

/*=====METODA ReadHeader=====
	Czyta nag³ówek pliku GLM.
*/
bool GLModel::ReadHeader( std::fstream& fileStream, unsigned& texCount )
{
	std::string str;

	// Pobieramy liczbê tekstur
	str = GetString( fileStream );
	if( !sscanf_s( str.c_str(), "TEXCOUNT %u", &texCount ) )
	{
		Log.Error( "GLMODEL( " + file + " ): Nie mo¿na odczytaæ liczby tekstur!" );
		return false;
	}

	// Pobieramy liczbê obiektów
	str = GetString( fileStream );
	if( !sscanf_s( str.c_str(), "LISTCOUNT %u", &ListCount ) )
	{
		Log.Error( "GLMODEL( " + file + " ): Nie mo¿na odczytaæ liczby obiektów!" );
		return false;
	}

	// Pobieramy, czy plik ma zapisan¹ animacjê
	str = GetString( fileStream );
	if( !sscanf_s( str.c_str(), "ANIMATION %u", &animation ) )
	{
		Log.Error( "GLMODEL( " + file + " ): Nie mo¿na odczytaæ animacji!" );
		return false;
	}

	// Sprawdzamy czy to koniec nag³ówka
	str = GetString( fileStream );
	if( str != "END HEADER" )
	{
		Log.Error( "GLMODEL( " + file + " ): Brak koñca nag³ówka!" );
		return false;
	}

	return true;
}

bool GLModel::LoadModel( std::string filename )
{
	// Sprawdzamy czy ³añcuch nie jest pusty
	if( filename.empty() )
	{
		Log.Error( "GLMODEL( " + file + " ): Pusty ci¹g nazwy pliku!" );
		return false;
	}
	
	// zmienna tymczasowa, trzymaj¹ca jedn¹ linie z pliku
	std::string str;

	// zmienna trzymaj¹ca numer wersji pliku
	int Version = 0;

	int i;

	// Próbujemy otworzyæ plik
	std::fstream fileStream(filename, std::ios::in);

	// Sprawdzamy po³¹czenie
	if( !fileStream )
	{
		Log.Error( "GLMODEL( " + file + " ): plik " + filename + " nie istnieje lub œcie¿ka niew³aœciwa." );
		return false;
	}

	// Pobieramy pierwsz¹ linie
	str = GetString( fileStream );

	// Skanujemy liniê w poszukiwaniu numeru wersji
	if( !sscanf_s( str.c_str(), "GLM %d", &Version ) )
	{
		Log.Error( "GLMODEL( " + file + " ): Nieprawid³owa pierwsza linia pliku " + filename + "!" );
		return false;
	}

	// Sprawdzamy wersjê
	if( Version > GLM_VERSION )
	{
		Log.Error( "GLMODEL( " + file + " ): Zbyt wysoka wersja pliku!" );
		return false;
	}

	// Teraz sprawdzamy, czy ju¿ jakiœ obiekt nie by³ za³adowany
	if( loaded )
	{
		Log.Report( "GLMODEL( " + file + " ): Prze³adowanie pliku na " + filename );
		Free();
	}

	Log.Log( "GLMODEL( " + file + " ): £adowanie modelu z pliku " + filename );
	file = filename;

	str = GetString( fileStream );

	unsigned texCount = 0;

	// Czytamy nag³ówek
	if( str == "HEADER" )
	{
		if( !ReadHeader( fileStream, texCount ) )
		{
			Log.Error( "GLMODEL( " + file + " ): B³¹d w nag³ówku!" );
			return false;
		}
	}
	else
	{
		Log.Error( "GLMODEL( " + file + " ): Brak nag³ówka!" );
		return false;
	}

	if( animation )
	{
		str = GetString( fileStream );

		// Czytamy nag³ówek
		if( str == "ANIMHEADER" )
		{
			if( !ReadAnimHeader( fileStream ) )
			{
				Log.Error( "GLMODEL( " + file + " ): B³¹d w nag³ówku animacji!" );
				return false;
			}
		}
		else
		{
			Log.Error( "GLMODEL( " + file + " ): Brak nag³ówka animacji!" );
			return false;
		}
	}
	
	// Czytamy tekstury
	if( texCount > 0 )
	{
		Textures.resize(texCount);
		memset(&Textures[0], 0, sizeof(CTexture*) * texCount);

		str = GetString( fileStream );

		if( str == "TEXLIST" )
		{
			for( i = 0; i < Textures.size(); i++ )
			{
				str = GetString( fileStream );

				if( !( Textures[i] = TManager.Get( str ) ) )
				{
					Log.Error( "GLMODEL( " + file + " ): B³¹d przy ³adowaniu tekstury!" );
				}

				if( str == "END TEXLIST" )
				{
					Log.Error( "GLMODEL( " + file + " ): Przedwczesny koniec listy tekstur!" );
					break;
				}
			}
			
			if( str != "END TEXLIST" )
			{
				str = GetString( fileStream );
				if( str != "END TEXLIST" )
				{
					Log.Error( "GLMODEL( " + file + " ): Brak koñca listy tekstur!" );
					Free();
					return false;
				}
			}
		}
		else
		{
			Log.Error( "GLMODEL( " + file + " ): Brak listy tekstur!" );
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
			str = GetString( fileStream );

			int j;

			sscanf_s( str.c_str(), "MODEL %d", &j );
			
			if( i != j )
			{
				Log.Error( "GLMODEL( " + file + " ): B³¹d, z³a kolejnoœæ modeli!" );
				Free();
				return false;
			}

			glNewList( List + i, GL_COMPILE );

			do
			{
				str = GetString( fileStream );

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
				str = GetString( fileStream );

				int j;

				sscanf_s( str.c_str(), "FRAME %d", &j );
				
				if( i != j )
				{
					Log.Error( "GLMODEL( " + file + " ): B³¹d, z³a kolejnoœæ klatek!" );
					Free();
					return false;				
				}

				glNewList( Frame + i, GL_COMPILE );

				do
				{
					str = GetString( fileStream );

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

	str = GetString( fileStream );

	if( str == "END GLM" )
	{
		loaded = true;
		return true;
	}

	Log.Error( "GLMODEL( " + file + " ): Brak koñca pliku!" );
	return false;
}

void GLModel::Free()
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

std::string GLModel::GetFile()
{
	return file;
}

unsigned int GLModel::GetObjCount()
{
	return ListCount;
}

//===========METODY NIETESTOWANE!!!============
bool GLModel::ReadAnimHeader( std::fstream& fileStream )
{
	std::string str;

	// Pobieramy liczbê tekstur
	str = GetString( fileStream );
	if( !sscanf_s( str.c_str(), "FRAMECOUNT %u", &FrameCount ) )
	{
		Log.Error( "GLMODEL( " + file + " ): Nie mo¿na odczytaæ liczby klatek animacji!" );
		return false;
	}

	// Sprawdzamy czy to koniec nag³ówka
	str = GetString( fileStream );
	if( str != "END ANIMHEADER" )
	{
		Log.Error( "GLMODEL( " + file + " ): Brak koñca nag³ówka animacji!" );
		return false;
	}

	return true;
}

void GLModel::PlayAnim( unsigned int fromframe, unsigned int toframe, bool canskip )
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

void GLModel::UpdateAnim()
{
	if( !playing )
		return;

	CurrFrame++;

	if( CurrFrame > ToFrame )
		playing = false;
}

void GLModel::RenderAnim( unsigned int index )
{
	if( !animation || !playing )
	{
		CallObject( index );
		return;
	}

	glCallList( Frame + CurrFrame );
}


GLModelManager GLMManager;

GLModelManager::GLModelManager()
{
}

GLModelManager::~GLModelManager()
{
	//Clear();
}

GLModel* GLModelManager::Get( std::string filename )
{
	if( filename.empty() )
	{
		Log.Error( "MODELMANAGER(): Pusty ci¹g znaków!" );
		return 0;
	}

	int i;
	GLModel* Model;
	for( i = 0; i < List.size(); i++ )
	{
		Model = GetModel( i );
		if( Model->GetFile() == filename )
			return Model;
	}

	Model = new GLModel;
	if( !Model->LoadModel( filename ) )
	{
		Log.Error( "MODELMANAGER(): Nieudane za³adowanie modelu: " + filename );
		delete Model;
		return 0;
	}

	AddModel( Model );
	Log.Log( "MODELMANAGER(): Dodano nowy model: " + filename );
	return Model;
}

void GLModelManager::AddModel( GLModel *Model )
{
	List.push_back( Model );
}

void GLModelManager::DeleteModel( unsigned int index )
{
	if( index >= List.size() )
		return;

	delete List[index];
	List.erase( List.begin() + index );
}

GLModel* GLModelManager::GetModel( unsigned int index )
{
	if( index >= List.size() )
		return 0;

	return List[index];
}

void GLModelManager::Clear()
{
	for(unsigned i = 0; i < List.size(); i++)
	{
		delete List[i];
	}
	List.clear();
}

