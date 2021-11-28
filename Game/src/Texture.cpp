/*///////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
Plik:	Texture.cpp
Autor:	Real_Noname (real_noname@wp.pl)
(C):	CODE RULERS (Real_Noname)
WWW:	www.coderulers.prv.pl
Opis:	Patrz -> Texture.h

/////////////////////////////////////////////////////////
///////////////////////////////////////////////////////*/

#include <CBGL/COpenGL.h>
#include <fstream>

#include "Texture.h"
#include "Log.h"

CTexture::CTexture() :
	loaded(false),
	file("-"),
	texture(0)
{
	glGenTextures( 1, &texture );
}

CTexture::CTexture( std::string filename ) :
	loaded(false),
	file("-"),
	texture(0)
{
	glGenTextures( 1, &texture );
	LoadTGA( filename );
}

CTexture::~CTexture()
{
	glDeleteTextures( 1, &texture );
}

bool	CTexture::LoadTGA( std::string filename )
{
	/*	�eby odczyta� plik TGA
		potrzebne s� odpowiednie
		zmienne pomocnicze.
	*/
	//	Ta tablica jest do por�wnania i trzyma nag��wek nieskompresowanego pliku TGA
	unsigned char TGAheader[12] = {0,0,2,0,0,0,0,0,0,0,0,0};

	// W�a�ciwa tablica, kt�ra b�dzie trzymac nag��wek z pliku
	unsigned char TGAcompare[12];				
	
	// Nag��wek opisuj�cy zapisany obraz
	unsigned char header[6];

	// Zmienna b�dzie trzyma� ilo�� bajt�w na pixel
	unsigned int bytesPerPixel;				

	// Rozmiar tablicy trzymaj�cej dane z pliku
	unsigned int imageSize;		

	// Tablica trzymaj�ca dane ( a raczej wska�nik na ni� )
	unsigned char *imageData;

	// Zmienna pomocnicza
	unsigned int temp;	
	
	// Typ zapisanego obrazu
	unsigned int type = GL_RGBA;

	// Ilo�� bit�w kolor�w obrazu
	unsigned int bpp;

	// Jego wysoko�� i szeroko��
	unsigned int width;
	unsigned int height;


	// Otwieramy plik
	std::fstream fileStream(filename, std::ios::in | std::ios::binary);
	
	Log.Log("TGATEX( " + file + " ): �adowanie pliku " + filename + "...");
	// Teraz sprawdzamy pare rzeczy
	if(	fileStream.bad() || // Czy otwarcie pliku si� uda�o
		!fileStream.read( (char*)TGAcompare, sizeof(char) * 12 ) ||	// Czy mo�na odczyta� 12 bajt�w nag��wka
		memcmp( TGAheader, TGAcompare, sizeof( TGAheader ) ) != 0 ||                    // Czy to jest nag��wek kt�rego szukamy?
		!fileStream.read( (char*)header, sizeof(char) * 6 ) )			// Je�eli tak, to czytamy nast�pne 6 bajt�w
	{
		// Na wypadek gdyby co� si� nie uda�o, to zwracamy false
		if ( fileStream.bad() )
		{
			Log.Error( "TGATEX( " + file + " ): Nieprawid�owa �cie�ka lub plik nie istnieje!" );
			return false;
		}
		else
		{
			Log.Error( "TGATEX( " + file + " ): B��d z nag��wkiem pliku!" );
			return false;
		}
	}
	
	if( loaded )
	{
		glDeleteTextures( 1, &texture );
		loaded = false;
		glGenTextures( 1, &texture );
		Log.Report( "TGATEX( " + file + " ): Prze�adowanie tekstury na " + filename );
	}
	file = filename;

	width  = header[1] * 256 + header[0];		// Opracuj szeroko�� obrazka
	height = header[3] * 256 + header[2];		// i jego wysokosc
    
 	if(	width  <= 0	||			// Je�eli szeroko��
		height <= 0	||			// lub wysoko�c jest mniejsza lub r�wna zeru
		( header[4] != 24 && header[4] != 32 ) )		// Lub to nie jest ani obraz 24 Bity, ani 32 Bity
	{
		// To zatrzymaj
		Log.Error( "TGATEX( " + file + " ): Nieprawid�owy obraz w pliku TGA!" );
		return false;
	}

	// Odczytaj Bity kolor�w obrazu
	bpp = header[4];

	// Oblicz ile to bajt�w na pixel
	bytesPerPixel = bpp / 8;

	// Oblicz wielko�� tablicy
	imageSize = width * height * bytesPerPixel;

	// I zarezerwuj tyle miejsca
	imageData = new unsigned char[imageSize];
	memset(imageData, 0, sizeof(unsigned char) * imageSize);

	/*	Teraz troch� teorii
		�eby zrobi� teksture dla OpenGL
		potrzebna jest tablica z informajcami
		o kolorach. To jest po�owa roboty, a pierwsz
		to wydoby� te dane z pliku :) Co ju� za chwile
		wykonamy.
	*/
	
	if(	imageData == NULL ||	// Sprawd� czy pami�� jest zarezerwowana
		!fileStream.read( (char*)imageData, imageSize * sizeof(char) ))	// Sprawd� czy NA PEWNO odczytali�my tyle z pliku ile chcieli�my
	{
		// Je�eli co� si� nie zgadza, to przerywamy
		if( imageData != NULL)
			delete[] imageData;

		Log.Error( "TGATEX( " + file + " ): B��d pami�ci!" );
		return false;	
	}
	;

	/*	A teraz ma�y kruczek. Jak zapewne
		wiadomo, s� trzy kolory sk�adowe:
		czerwony (Red), zielony (Green) oraz
		niebieski (Blue), czyli poprawny kolot�w format powinien
		by� RGB. Ale z niewiadomych przyczyn w pliku TGA jest... BGR?
		Musimy to poprawi� dlatego teraz zamieniamy
		miejscami dwa feralne bajty.
	*/
	for( unsigned int i = 0; i < int( imageSize ); i += bytesPerPixel )	
	{								
		temp = imageData[i];				// Wk�adamy pierwszy bajt ( Blue ) do zmiennej pomocniczej
		imageData[i] = imageData[i + 2];	// Przek�adamy ostatni ( Red ) na pierwsze miejsce
		imageData[i + 2] = temp;			// I zast�pujemy ostatni warto�ci� ze zmiennej pomocniczej
	}

	// Zamykamy plik
	fileStream.close();

	// Musimy jeszcze zmieni� format je�eli to nie jest obraz 32 Bity, tylko 24 Bity
	if ( bpp == 24 )
		type = GL_RGB;

	/*	Teraz to dopiero jaja...
		Konwertujemy uzyskane dane na
		teksture(y). Ale dlaczego a� 3?
		By mie� mo�liwo�� wyboru jako�ci tekstury :)
		W zale�no�ci od parametr�w jak ma si� zachowywa�
		tekstura, kiedy jest wi�ksza, lub mniejsza ni�
		w oryginale, to zachowuje r�n� jako��.
		Dodatkowo w najwy�szej jako�ci u�yty jest mipmaping, 
		kt�ry daje lepszy wygl�d gdy tekstura jest pod du�ym k�tem.
		Po wi�cej informacji zapraszam do dokumentacji OpenGL (RTFM!) :) 
	*/
	//===================Najwy�sza jako�� MipMapingu====================
	glBindTexture( GL_TEXTURE_2D, texture );			// Bind Our Texture
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR ); 
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, type, GL_UNSIGNED_BYTE, imageData);
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA8, width, height, type, GL_UNSIGNED_BYTE, imageData );
	glBindTexture( GL_TEXTURE_2D, 0 );

	// Po wszystkim, te dane s� ju� niepotrzebne
	delete[] imageData;

	// zaznaczamy, �e by� odczyt i potwierdzamy poprawne za�adowanie pliku
	loaded = true;
	return true;
}

void CTexture::Activate( unsigned int tex )
{
	if( !loaded )
		return;

	glBindTexture( GL_TEXTURE_2D, texture );
}

std::string CTexture::GetFile()
{
	return file;
}




CTexManager::CTexManager( const std::string& strDataDir ) :
	DataDir( strDataDir )
{
}

CTexManager::~CTexManager()
{
	Clear();
}

CTexture* CTexManager::Get( std::string filename )
{
	if( filename.empty() )
	{
		Log.Error( "TEXMANAGER(): Pusty ci�g znak�w!" );
		return nullptr;
	}

	auto path = DataDir + filename;

	CTexture* NewTex;
	for( unsigned i = 0; i < List.size(); i++ )
	{
		NewTex = GetTexture( i );
		if( NewTex->GetFile() == path )
			return NewTex;
	}

	NewTex = new CTexture;
	if( !NewTex->LoadTGA( path ) )
	{
		Log.Error( "TEXMANAGER(): Nieudane za�adowanie tekstury: " + filename );
		delete NewTex;
		return nullptr;
	}

	AddTexture( NewTex );
	Log.Log( "TEXMANAGER(): Dodano now� teksture: " + filename );
	
	return NewTex;
}

void CTexManager::AddTexture( CTexture *Tex )
{
	List.push_back( Tex );
}

void CTexManager::DeleteTexture( unsigned int index )
{
	if( index >= List.size() )
		return;

	delete List[index];
	List.erase( List.begin() + index );
}

CTexture* CTexManager::GetTexture( unsigned int index )
{
	if( index >= List.size() )
		return 0;

	return List[index];
}

void CTexManager::Clear()
{
	for(unsigned i = 0; i < List.size(); i++)
	{
		delete GetTexture( i );
	}
	List.clear();
}