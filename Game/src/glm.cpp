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

/*=====KONSTRUKTOR=====*/
GLModel::GLModel()
{
	// Zerowanie wartoœci i przypisywanie wartoœci domyœlnych
	Tex = NULL;
	TexCount = 0;
	List = 0;
	ListCount = 0;
	Frame = 0;
	FrameCount = 0;
	loaded = false;
	file = "-";
	Tex = NULL;
}

/*=====DESTRUKTOR=====*/
GLModel::~GLModel()
{
	// Zwolnienie pamiêci
	Free();
}

/*=====METODA GetString=====
	Metoda pobiera jedn¹ linie z pliku
	i j¹ modyfikuje. Pozbywa siê pustej
	linii, znaków spacji i tabulacji przed
	rozpoczêciem w³aœciwych poleceñ, oraz
	czyœci z komentarzy.
*/
std::string GLModel::GetString( FILE* fp )
{
	char buf[1024];
	std::string zwrot;
	int len = 0;
	bool repeat = false;
	bool wfchar = false;

	do
	{
		len = 0;
		repeat = false;
		wfchar = false;
		fgets( buf, 1024, fp );
	
		len = strlen( buf );
		zwrot = "";

		if( buf[0] == '\n' )
		{
			repeat = true;
			continue;
		}

		for( int i = 0; i < len; i++ )
		{
			if( ( buf[i] == '#' || buf[i] == '/' ) && !wfchar )
			{
				repeat = true;
				break;
			}

			if( buf[i] != ' ' && buf[i] != '\t' && buf[i] != '\n' )
				wfchar = true;

			zwrot += buf[i];
		}
	}
	while( repeat );

	if( zwrot[zwrot.length()-1] == '\n' )
		zwrot = zwrot.substr( 0, zwrot.length() - 1 );

	return zwrot;
}

/*=====METODA NoSpace=====
	Czyœci podan¹ liniê ze spacji, tablulacji
	i znaków nowej linii.
*/
std::string GLModel::NoSpace( const std::string &str )
{
	std::string zwrot = "";

	for( unsigned int i = 0; i < str.length(); i++ )
	{
		if( str[i] == ' ' || str[i] == '\t' || str[i] == '\n' )
			continue;

		zwrot += str[i];
	}

	return zwrot;
}

/*=====METODA GetParams=====
	Pobiera dowoln¹ iloœæ parametrów pomiêdzy
	nawiasami ( ) odzielonymi przecinkiem i je
	wpisuje do podanej tablicy.
*/
bool GLModel::GetParams( const std::string &str, const int from, std::string* param, const int paramCount, const std::string &Com )
{
	if( paramCount <= 0 )
		return true;

	int curparam = 0;
	unsigned int i;
	for( i = from; i < str.length(); i++ )
	{
		if( str[i] == ')' )
			break;

		if( str[i] == ',' )
		{
			curparam++;
			i++;

			if( curparam > paramCount )
				break;
		}

		param[curparam] += str[i];
	}
	if( curparam > paramCount )
	{
		Log.Error( "GLMODEL( " + file + " ): Za du¿o parametrów polecenia: " + Com );
		return false;
	}
	else if( curparam != paramCount-1 )
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
		presult = GetParams( str, i+1, param, 1, Com );

		if( presult && param[0] != "" )
		{
			k = atoi( param[0].c_str() );
			if( k >= TexCount || k < 0 )
			{
				Log.Error( "GLMODEL( " + file + " ): B³êdny parametr polecenia: " + Com );
			}
			else Tex[k]->Activate( GUI.GetTexDLevel() );
			return;
		}
	}
	if( Com == "glCallList" )
	{
		presult = GetParams( str, i+1, param, 1, Com );

		if( presult && param[0] != "" )
		{
			k = atoi( param[0].c_str() );
			if( k >= ListCount || k < 0 )
			{
				Log.Error( "GLMODEL( " + file + " ): B³êdny parametr polecenia: " + Com );
			}
			else glCallList( List + k );
			return;
		}
	}
	if( Com == "glRotate" )
	{
		presult = GetParams( str, i+1, param, 4, Com );

		if( presult && param[0] != "" && param[1] != "" && param[2] != "" && param[3] != "" )
		{
			glRotatef( atof( param[0].c_str() ), atof( param[1].c_str() ), atof( param[2].c_str() ), atof( param[3].c_str() ) );
			return;
		}
	}
	if( Com == "glTranslate" )
	{
		presult = GetParams( str, i+1, param, 3, Com );

		if( presult && param[0] != "" && param[1] != "" && param[2] != "" )
		{
			glTranslatef( atof( param[0].c_str() ), atof( param[1].c_str() ), atof( param[2].c_str() ) );
			return;
		}
	}
	if( Com == "glScale" )
	{
		presult = GetParams( str, i+1, param, 3, Com );

		if( presult && param[0] != "" && param[1] != "" && param[2] != "" )
		{
			glScalef( atof( param[0].c_str() ), atof( param[1].c_str() ), atof( param[2].c_str() ) );
			return;
		}
	}
	if( Com == "glBegin" )
	{
		presult = GetParams( str, i+1, param, 1, Com );

		if( presult && param[0] != "" )
		{
			glBegin( GetConst( param[0], Com ) );
			return;
		}
	}
	if( Com == "glEnable" )
	{
		presult = GetParams( str, i+1, param, 1, Com );

		if( presult && param[0] != "" )
		{
			glEnable( GetConst( param[0], Com ) );
			return;
		}
	}
	if( Com == "glDisable" )
	{
		presult = GetParams( str, i+1, param, 1, Com );

		if( presult && param[0] != "" )
		{
			glDisable( GetConst( param[0], Com ) );
			return;
		}
	}
	if( Com == "glEnd" )
	{
		presult = GetParams( str, i+1, param, 0, Com );

		if( presult )
		{
			glEnd();
			return;
		}
	}
	if( Com == "glPushMatrix" )
	{
		presult = GetParams( str, i+1, param, 0, Com );

		if( presult )
		{
			glPushMatrix();
			return;
		}
	}
	if( Com == "glPopMatrix" )
	{
		presult = GetParams( str, i+1, param, 0, Com );

		if( presult )
		{
			glPopMatrix();
			return;
		}
	}
	if( Com == "glColor3" )
	{
		presult = GetParams( str, i+1, param, 3, Com );

		if( presult && param[0] != "" && param[1] != "" && param[2] != "" )
		{
			glColor3f( atof( param[0].c_str() ), atof( param[1].c_str() ), atof( param[2].c_str() ) );
			return;
		}
	}
	if( Com == "glColor4" )
	{
		presult = GetParams( str, i+1, param, 4, Com );

		if( presult && param[0] != "" && param[1] != "" && param[2] != "" && param[3] != "" )
		{
			glColor4f( atof( param[0].c_str() ), atof( param[1].c_str() ), atof( param[2].c_str() ), atof( param[3].c_str() ) );
			return;
		}
	}
	if( Com == "glNormal3" )
	{
		presult = GetParams( str, i+1, param, 3, Com );

		if( presult && param[0] != "" && param[1] != "" && param[2] != "" )
		{
			glNormal3f( atof( param[0].c_str() ), atof( param[1].c_str() ), atof( param[2].c_str() ) );
			return;
		}
	}
	if( Com == "glTexCoord2" )
	{
		presult = GetParams( str, i+1, param, 2, Com );

		if( presult && param[0] != "" && param[1] != "" )
		{
			glTexCoord2f( atof( param[0].c_str() ), atof( param[1].c_str() ) );
			return;
		}
	}
	if( Com == "glVertex3" )
	{
		presult = GetParams( str, i+1, param, 3, Com );

		if( presult && param[0] != "" && param[1] != "" && param[2] != "" )
		{
			glVertex3f( atof( param[0].c_str() ), atof( param[1].c_str() ), atof( param[2].c_str() ) );
			return;
		}
	}
	if( Com == "glBlendFunc" )
	{
		presult = GetParams( str, i+1, param, 2, Com );

		if( presult && param[0] != "" && param[1] != "" )
		{
			glBlendFunc( GetConst( param[0], Com ), GetConst( param[1], Com ) );
			return;
		}
	}
	if( Com == "glFrontFace" )
	{
		presult = GetParams( str, i+1, param, 1, Com );

		if( presult && param[0] != "" )
		{
			glFrontFace( GetConst( param[0], Com ) );
			return;
		}
	}
	if( Com == "gluQuadricDrawStyle" )
	{
		presult = GetParams( str, i+1, param, 1, Com );

		if( presult && param[0] != "" )
		{
			gluQuadricDrawStyle( obj, GetConst( param[0], Com ) );
			return;
		}
	}
	if( Com == "gluQuadricNormals" )
	{
		presult = GetParams( str, i+1, param, 1, Com );

		if( presult && param[0] != "" )
		{
			gluQuadricNormals( obj, GetConst( param[0], Com ) );
			return;
		}
	}
	if( Com == "gluQuadricOrientation" )
	{
		presult = GetParams( str, i+1, param, 1, Com );

		if( presult && param[0] != "" )
		{
			gluQuadricOrientation( obj, GetConst( param[0], Com ) );
			return;
		}
	}
	if( Com == "gluQuadricTexture" )
	{
		presult = GetParams( str, i+1, param, 1, Com );

		if( presult && param[0] != "" )
		{
			gluQuadricTexture( obj, GetConst( param[0], Com ) );
			return;
		}
	}
	if( Com == "gluSphere" )
	{
		presult = GetParams( str, i+1, param, 3, Com );

		if( presult && param[0] != "" && param[1] != "" && param[2] != "" )
		{
			gluSphere( obj, atof( param[0].c_str() ), atoi( param[1].c_str() ), atoi( param[2].c_str() ) );
			return;
		}
	}
	if( Com == "gluCylinder" )
	{
		presult = GetParams( str, i+1, param, 5, Com );

		if( presult && param[0] != "" && param[1] != "" && param[2] != "" && param[3] != "" && param[4] != "" )
		{
			gluCylinder( obj, atof( param[0].c_str() ), atof( param[1].c_str() ), atof( param[2].c_str() ), atoi( param[3].c_str() ), atoi( param[4].c_str() ) );
			return;
		}
	}
	if( Com == "gluDisk" )
	{
		presult = GetParams( str, i+1, param, 4, Com );

		if( presult && param[0] != "" && param[1] != "" && param[2] != "" && param[3] != "" )
		{
			gluDisk( obj, atof( param[0].c_str() ), atof( param[1].c_str() ), atoi( param[2].c_str() ), atoi( param[3].c_str() ) );
			return;
		}
	}
	if( Com == "gluPartialDisk" )
	{
		presult = GetParams( str, i+1, param, 6, Com );

		if( presult && param[0] != "" && param[1] != "" && param[2] != "" && param[3] != "" && param[4] != "" && param[5] != "" )
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
bool GLModel::ReadHeader( FILE* fp )
{
	std::string str;

	// Pobieramy liczbê tekstur
	str = GetString( fp );
	if( !sscanf_s( str.c_str(), "TEXCOUNT\t%u", &TexCount ) )
	{
		Log.Error( "GLMODEL( " + file + " ): Nie mo¿na odczytaæ liczby tekstur!" );
		return false;
	}

	// Pobieramy liczbê obiektów
	str = GetString( fp );
	if( !sscanf_s( str.c_str(), "LISTCOUNT\t%u", &ListCount ) )
	{
		Log.Error( "GLMODEL( " + file + " ): Nie mo¿na odczytaæ liczby obiektów!" );
		return false;
	}

	// Pobieramy, czy plik ma zapisan¹ animacjê
	str = GetString( fp );
	if( !sscanf_s( str.c_str(), "ANIMATION\t%u", &Animation ) )
	{
		Log.Error( "GLMODEL( " + file + " ): Nie mo¿na odczytaæ animacji!" );
		return false;
	}

	// Sprawdzamy czy to koniec nag³ówka
	str = GetString( fp );
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
	if( filename == "" )
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
	FILE* fp = 0;
	fopen_s( &fp, filename.c_str(), "rt" );

	// Sprawdzamy po³¹czenie
	if( !fp )
	{
		Log.Error( "GLMODEL( " + file + " ): plik " + filename + " nie istnieje lub œcie¿ka niew³aœciwa." );
		return false;
	}

	// Pobieramy pierwsz¹ linie
	str = GetString( fp );

	// Skanujemy liniê w poszukiwaniu numeru wersji
	if( !sscanf_s( str.c_str(), "GLM\t%d", &Version ) )
	{
		Log.Error( "GLMODEL( " + file + " ): Nieprawid³owa pierwsza linia pliku " + filename + "!" );
		fclose( fp );
		return false;
	}

	// Sprawdzamy wersjê
	if( Version > GLM_VERSION )
	{
		Log.Error( "GLMODEL( " + file + " ): Zbyt wysoka wersja pliku!" );
		fclose( fp );
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

	str = GetString( fp );

	// Czytamy nag³ówek
	if( str == "HEADER" )
	{
		if( !ReadHeader( fp ) )
		{
			Log.Error( "GLMODEL( " + file + " ): B³¹d w nag³ówku!" );
			fclose( fp );
			return false;
		}
	}
	else
	{
		Log.Error( "GLMODEL( " + file + " ): Brak nag³ówka!" );
		fclose( fp );
		return false;
	}

	if( Animation )
	{
		str = GetString( fp );

		// Czytamy nag³ówek
		if( str == "ANIMHEADER" )
		{
			if( !ReadAnimHeader( fp ) )
			{
				Log.Error( "GLMODEL( " + file + " ): B³¹d w nag³ówku animacji!" );
				fclose( fp );
				return false;
			}
		}
		else
		{
			Log.Error( "GLMODEL( " + file + " ): Brak nag³ówka animacji!" );
			fclose( fp );
			return false;
		}
	}
	
	// Czytamy tekstury
	if( TexCount > 0 )
	{
		Tex = new ioTexture*[TexCount];

		str = GetString( fp );

		if( str == "TEXLIST" )
		{
			for( i = 0; i < TexCount; i++ )
			{
				str = GetString( fp );

				if( !( Tex[i] = TManager.Get( str ) ) )
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
				str = GetString( fp );
				if( str != "END TEXLIST" )
				{
					Log.Error( "GLMODEL( " + file + " ): Brak koñca listy tekstur!" );
					Free();
					fclose( fp );
					return false;
				}
			}
		}
		else
		{
			Log.Error( "GLMODEL( " + file + " ): Brak listy tekstur!" );
			Free();
			fclose( fp );
			return false;
		}
	}

	// Czytamy modele
	if( ListCount > 0 )
	{
		List = glGenLists( ListCount );
		obj = gluNewQuadric();
		gluQuadricTexture( obj, GL_TRUE );

		for( i = 0; i < ListCount; i++ )
		{
			str = GetString( fp );

			int j;

			sscanf_s( str.c_str(), "MODEL\t%d", &j );
			
			if( i != j )
			{
				Log.Error( "GLMODEL( " + file + " ): B³¹d, z³a kolejnoœæ modeli!" );
				Free();
				fclose( fp );
				return false;
			}

			glNewList( List + i, GL_COMPILE );

			do
			{
				str = GetString( fp );

				if( str != "END MODEL" )
				{
					ParseGLCommand( str );
				}
			}
			while( str != "END MODEL" );

			glEndList();
		}

		gluDeleteQuadric( obj );
	}

	if( Animation )
	{
		if( FrameCount > 0 )
		{
			Frame = glGenLists( FrameCount );
			obj = gluNewQuadric();
			gluQuadricTexture( obj, GL_TRUE );

			for( i = 0; i < FrameCount; i++ )
			{
				str = GetString( fp );

				int j;

				sscanf_s( str.c_str(), "FRAME\t%d", &j );
				
				if( i != j )
				{
					Log.Error( "GLMODEL( " + file + " ): B³¹d, z³a kolejnoœæ klatek!" );
					Free();
					fclose( fp );
					return false;				
				}

				glNewList( Frame + i, GL_COMPILE );

				do
				{
					str = GetString( fp );

					if( str != "END FRAME" )
					{
						ParseGLCommand( str );
					}
				}
				while( str != "END FRAME" );

				glEndList();
			}

			gluDeleteQuadric( obj );
		}
	
	}

	str = GetString( fp );

	if( str == "END GLM" )
	{
		loaded = true;
		fclose( fp );
		return true;
	}

	Log.Error( "GLMODEL( " + file + " ): Brak koñca pliku!" );
	return false;
}

void GLModel::Free()
{
	if( Tex != NULL )
	{
		Tex = NULL;
		TexCount = 0;
	}

	if( glIsList( List ) )
	{
		glDeleteLists( List, ListCount );
		List = 0;
		ListCount = 0;
	}

	if( glIsList( Frame ) )
	{
		glDeleteLists( Frame, FrameCount );
		Frame = 0;
		FrameCount = 0;
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
bool GLModel::ReadAnimHeader( FILE* fp )
{
	std::string str;

	// Pobieramy liczbê tekstur
	str = GetString( fp );
	if( !sscanf_s( str.c_str(), "FRAMECOUNT\t%u", &FrameCount ) )
	{
		Log.Error( "GLMODEL( " + file + " ): Nie mo¿na odczytaæ liczby klatek animacji!" );
		return false;
	}

	// Sprawdzamy czy to koniec nag³ówka
	str = GetString( fp );
	if( str != "END ANIMHEADER" )
	{
		Log.Error( "GLMODEL( " + file + " ): Brak koñca nag³ówka animacji!" );
		return false;
	}

	return true;
}

void GLModel::PlayAnim( unsigned int fromframe, unsigned int toframe, bool canskip )
{
	if( !Animation )
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

void GLModel::DoEngineAnim()
{
	if( !playing )
		return;

	CurrFrame++;

	if( CurrFrame > ToFrame )
		playing = false;
}

void GLModel::DoDrawAnim( unsigned int index )
{
	if( !Animation || !playing )
	{
		CallObject( index );
		return;
	}

	glCallList( Frame + CurrFrame );
}



GLModelManager::GLModelManager()
{
}

GLModelManager::~GLModelManager()
{
	Clear();
}

GLModel* GLModelManager::Get( std::string filename )
{
	if( filename == "" )
	{
		Log.Error( "MODELMANAGER(): Pusty ci¹g znaków!" );
		return NULL;
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
		return NULL;
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
		return NULL;

	return List[index];
}

void GLModelManager::Clear()
{
	int i;
	for( i = List.size()-1; i >= 0; i-- )
	{
		DeleteModel( i );
	}
	List.clear();
}

GLModelManager GLMManager;