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
	/*	¯eby odczytaæ plik TGA
		potrzebne s¹ odpowiednie
		zmienne pomocnicze.
	*/
	//	Ta tablica jest do porównania i trzyma nag³ówek nieskompresowanego pliku TGA
	unsigned char TGAheader[12] = {0,0,2,0,0,0,0,0,0,0,0,0};

	// W³aœciwa tablica, która bêdzie trzymac nag³ówek z pliku
	unsigned char TGAcompare[12];				
	
	// Nag³ówek opisuj¹cy zapisany obraz
	unsigned char header[6];

	// Zmienna bêdzie trzymaæ iloœæ bajtów na pixel
	unsigned int bytesPerPixel;				

	// Rozmiar tablicy trzymaj¹cej dane z pliku
	unsigned int imageSize;		

	// Tablica trzymaj¹ca dane ( a raczej wskaŸnik na ni¹ )
	unsigned char *imageData;

	// Zmienna pomocnicza
	unsigned int temp;	
	
	// Typ zapisanego obrazu
	unsigned int type = GL_RGBA;

	// Iloœæ bitów kolorów obrazu
	unsigned int bpp;

	// Jego wysokoœæ i szerokoœæ
	unsigned int width;
	unsigned int height;


	// Otwieramy plik
	std::fstream fileStream(filename, std::ios::in | std::ios::binary);
	
	Log.Log("TGATEX( " + file + " ): £adowanie pliku " + filename + "...");
	// Teraz sprawdzamy pare rzeczy
	if(	fileStream.bad() || // Czy otwarcie pliku siê uda³o
		!fileStream.read( (char*)TGAcompare, sizeof(char) * 12 ) ||	// Czy mo¿na odczytaæ 12 bajtów nag³ówka
		memcmp( TGAheader, TGAcompare, sizeof( TGAheader ) ) != 0 ||                    // Czy to jest nag³ówek którego szukamy?
		!fileStream.read( (char*)header, sizeof(char) * 6 ) )			// Je¿eli tak, to czytamy nastêpne 6 bajtów
	{
		// Na wypadek gdyby coœ siê nie uda³o, to zwracamy false
		if ( fileStream.bad() )
		{
			Log.Error( "TGATEX( " + file + " ): Nieprawid³owa œcie¿ka lub plik nie istnieje!" );
			return false;
		}
		else
		{
			Log.Error( "TGATEX( " + file + " ): B³¹d z nag³ówkiem pliku!" );
			return false;
		}
	}
	
	if( loaded )
	{
		glDeleteTextures( 1, &texture );
		loaded = false;
		glGenTextures( 1, &texture );
		Log.Report( "TGATEX( " + file + " ): Prze³adowanie tekstury na " + filename );
	}
	file = filename;

	width  = header[1] * 256 + header[0];		// Opracuj szerokoœæ obrazka
	height = header[3] * 256 + header[2];		// i jego wysokosc
    
 	if(	width  <= 0	||			// Je¿eli szerokoœæ
		height <= 0	||			// lub wysokoœc jest mniejsza lub równa zeru
		( header[4] != 24 && header[4] != 32 ) )		// Lub to nie jest ani obraz 24 Bity, ani 32 Bity
	{
		// To zatrzymaj
		Log.Error( "TGATEX( " + file + " ): Nieprawid³owy obraz w pliku TGA!" );
		return false;
	}

	// Odczytaj Bity kolorów obrazu
	bpp = header[4];

	// Oblicz ile to bajtów na pixel
	bytesPerPixel = bpp / 8;

	// Oblicz wielkoœæ tablicy
	imageSize = width * height * bytesPerPixel;

	// I zarezerwuj tyle miejsca
	imageData = new unsigned char[imageSize];
	memset(imageData, 0, sizeof(unsigned char) * imageSize);

	/*	Teraz trochê teorii
		¯eby zrobiæ teksture dla OpenGL
		potrzebna jest tablica z informajcami
		o kolorach. To jest po³owa roboty, a pierwsz
		to wydobyæ te dane z pliku :) Co ju¿ za chwile
		wykonamy.
	*/
	
	if(	imageData == NULL ||	// SprawdŸ czy pamiêæ jest zarezerwowana
		!fileStream.read( (char*)imageData, imageSize * sizeof(char) ))	// SprawdŸ czy NA PEWNO odczytaliœmy tyle z pliku ile chcieliœmy
	{
		// Je¿eli coœ siê nie zgadza, to przerywamy
		if( imageData != NULL)
			delete[] imageData;

		Log.Error( "TGATEX( " + file + " ): B³¹d pamiêci!" );
		return false;	
	}
	;

	/*	A teraz ma³y kruczek. Jak zapewne
		wiadomo, s¹ trzy kolory sk³adowe:
		czerwony (Red), zielony (Green) oraz
		niebieski (Blue), czyli poprawny kolotów format powinien
		byæ RGB. Ale z niewiadomych przyczyn w pliku TGA jest... BGR?
		Musimy to poprawiæ dlatego teraz zamieniamy
		miejscami dwa feralne bajty.
	*/
	for( unsigned int i = 0; i < int( imageSize ); i += bytesPerPixel )	
	{								
		temp = imageData[i];				// Wk³adamy pierwszy bajt ( Blue ) do zmiennej pomocniczej
		imageData[i] = imageData[i + 2];	// Przek³adamy ostatni ( Red ) na pierwsze miejsce
		imageData[i + 2] = temp;			// I zastêpujemy ostatni wartoœci¹ ze zmiennej pomocniczej
	}

	// Zamykamy plik
	fileStream.close();

	// Musimy jeszcze zmieniæ format je¿eli to nie jest obraz 32 Bity, tylko 24 Bity
	if ( bpp == 24 )
		type = GL_RGB;

	/*	Teraz to dopiero jaja...
		Konwertujemy uzyskane dane na
		teksture(y). Ale dlaczego a¿ 3?
		By mieæ mo¿liwoœæ wyboru jakoœci tekstury :)
		W zale¿noœci od parametrów jak ma siê zachowywaæ
		tekstura, kiedy jest wiêksza, lub mniejsza ni¿
		w oryginale, to zachowuje ró¿n¹ jakoœæ.
		Dodatkowo w najwy¿szej jakoœci u¿yty jest mipmaping, 
		który daje lepszy wygl¹d gdy tekstura jest pod du¿ym k¹tem.
		Po wiêcej informacji zapraszam do dokumentacji OpenGL (RTFM!) :) 
	*/
	//===================Najwy¿sza jakoœæ MipMapingu====================
	glBindTexture( GL_TEXTURE_2D, texture );			// Bind Our Texture
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR ); 
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, type, GL_UNSIGNED_BYTE, imageData);
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA8, width, height, type, GL_UNSIGNED_BYTE, imageData );
	glBindTexture( GL_TEXTURE_2D, 0 );

	// Po wszystkim, te dane s¹ ju¿ niepotrzebne
	delete[] imageData;

	// zaznaczamy, ¿e by³ odczyt i potwierdzamy poprawne za³adowanie pliku
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
		Log.Error( "TEXMANAGER(): Pusty ci¹g znaków!" );
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
		Log.Error( "TEXMANAGER(): Nieudane za³adowanie tekstury: " + filename );
		delete NewTex;
		return nullptr;
	}

	AddTexture( NewTex );
	Log.Log( "TEXMANAGER(): Dodano now¹ teksture: " + filename );
	
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