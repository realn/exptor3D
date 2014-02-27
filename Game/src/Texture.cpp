/*///////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
Plik:	Texture.cpp
Autor:	Real_Noname (real_noname@wp.pl)
(C):	CODE RULERS (Real_Noname)
WWW:	www.coderulers.prv.pl
Opis:	Patrz -> Texture.h

/////////////////////////////////////////////////////////
///////////////////////////////////////////////////////*/
#include "Texture.h"

ioTexture::ioTexture()
{
	loaded = false;
	file = "-";
}

ioTexture::ioTexture( std::string filename )
{
	ioTexture();
	LoadTGA( filename );
}

ioTexture::~ioTexture()
{
	Free();
}

bool ioTexture::LoadTGA( std::string filename )
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
	FILE *fp = 0;
	fopen_s( &fp, filename.c_str(), "rb" );

	// Teraz sprawdzamy pare rzeczy
	if(	!fp || // Czy otwarcie pliku siê uda³o
		fread( TGAcompare, 1, sizeof(TGAcompare), fp) != sizeof(TGAcompare) ||	// Czy mo¿na odczytaæ 12 bajtów nag³ówka
		memcmp( TGAheader, TGAcompare, sizeof( TGAheader ) ) != 0 ||                    // Czy to jest nag³ówek którego szukamy?
		fread( header, 1, sizeof(header), fp ) != sizeof( header ) )			// Je¿eli tak, to czytamy nastêpne 6 bajtów
	{
		// Na wypadek gdyby coœ siê nie uda³o, to zwracamy false
		if ( !fp )
		{
			Log.Error( "TGATEX( " + file + " ): Nieprawid³owa œcie¿ka lub plik nie istnieje!" );
			return false;
		}
		else
		{
			Log.Error( "TGATEX( " + file + " ): B³¹d z nag³ówkiem pliku!" );
			fclose( fp );	
			return false;
		}
	}
	
	if( loaded )
	{
		Log.Report( "TGATEX( " + file + " ): Prze³adowanie tekstury na " + filename );

	}
	Log.Log( "TGATEX( " + file + " ): £adowanie pliku: " + filename );
	file = filename;

	width  = header[1] * 256 + header[0];		// Opracuj szerokoœæ obrazka
	height = header[3] * 256 + header[2];		// i jego wysokosc
    
 	if(	width  <= 0	||			// Je¿eli szerokoœæ
		height <= 0	||			// lub wysokoœc jest mniejsza lub równa zeru
		( header[4] != 24 && header[4] != 32 ) )		// Lub to nie jest ani obraz 24 Bity, ani 32 Bity
	{
		// To zatrzymaj
		Log.Error( "TGATEX( " + file + " ): Nieprawid³owy obraz w pliku TGA!" );
		fclose( fp );	
		return false;
	}

	// Odczytaj Bity kolorów obrazu
	bpp = header[4];

	// Oblicz ile to bajtów na pixel
	bytesPerPixel = bpp / 8;

	// Oblicz wielkoœæ tablicy
	imageSize = width * height * bytesPerPixel;

	// I zarezerwuj tyle miejsca
	imageData = (unsigned char *)malloc( imageSize );

	/*	Teraz trochê teorii
		¯eby zrobiæ teksture dla OpenGL
		potrzebna jest tablica z informajcami
		o kolorach. To jest po³owa roboty, a pierwsz
		to wydobyæ te dane z pliku :) Co ju¿ za chwile
		wykonamy.
	*/
	
	if(	imageData == NULL ||	// SprawdŸ czy pamiêæ jest zarezerwowana
		fread( imageData, 1, imageSize, fp ) != imageSize)	// SprawdŸ czy NA PEWNO odczytaliœmy tyle z pliku ile chcieliœmy
	{
		// Je¿eli coœ siê nie zgadza, to przerywamy
		if( imageData != NULL)
			free( imageData );

		Log.Error( "TGATEX( " + file + " ): B³¹d pamiêci!" );
		fclose( fp );			
		return false;	
	}

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
	fclose( fp );							

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

	// Stwórz piêæ niezdefinowanych tekstur
	glGenTextures( 3, &texture[0] );

	// I same tekstury

	//===========Najgorsza jakoœæ, bez MipMapingu==========
	glBindTexture( GL_TEXTURE_2D, texture[0] );			
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
	
	glTexImage2D( GL_TEXTURE_2D, 0, type, width, height, 0, type, GL_UNSIGNED_BYTE, imageData );

	//===========Najlepsza jakoœæ zwyk³a, bez MipMapingu=====
	glBindTexture( GL_TEXTURE_2D, texture[1] );			
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );  
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );	
	
	glTexImage2D( GL_TEXTURE_2D, 0, type, width, height, 0, type, GL_UNSIGNED_BYTE, imageData );

	//===================Najwy¿sza jakoœæ MipMapingu====================
	glBindTexture( GL_TEXTURE_2D, texture[2] );			// Bind Our Texture
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR ); 
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR );
	
	gluBuild2DMipmaps(GL_TEXTURE_2D, type, width, height, type, GL_UNSIGNED_BYTE, imageData );

	// Po wszystkim, te dane s¹ ju¿ niepotrzebne
	delete[] imageData;

	// zaznaczamy, ¿e by³ odczyt i potwierdzamy poprawne za³adowanie pliku
	loaded = true;
	return true;
}

void ioTexture::Activate( unsigned int tex )
{
	if( !loaded || tex > 2 )
		return;

	glBindTexture( GL_TEXTURE_2D, texture[tex] );
}

std::string ioTexture::GetFile()
{
	return file;
}

void ioTexture::Free()
{
	if( !loaded )
		return;

	glDeleteTextures( 3, &texture[0] );

	loaded = false;
}


ioTexManager::ioTexManager()
{
}

ioTexManager::~ioTexManager()
{
	Clear();
}

ioTexture* ioTexManager::Get( std::string filename )
{
	if( filename == "" )
	{
		Log.Error( "TEXMANAGER(): Pusty ci¹g znaków!" );
		return NULL;
	}

	unsigned int i;
	ioTexture* NewTex;
	for( i = 0; i < List.size(); i++ )
	{
		NewTex = GetTexture( i );
		if( NewTex->GetFile() == filename )
			return NewTex;
	}

	NewTex = new ioTexture;
	if( !NewTex->LoadTGA( filename ) )
	{
		Log.Error( "TEXMANAGER(): Nieudane za³adowanie tekstury: " + filename );
		delete NewTex;
		return NULL;
	}
	AddTexture( NewTex );
	Log.Log( "TEXMANAGER(): Dodano now¹ teksture: " + filename );
	return NewTex;
}

void ioTexManager::AddTexture( ioTexture *Tex )
{
	List.push_back( Tex );
}

void ioTexManager::DeleteTexture( unsigned int index )
{
	if( index >= List.size() )
		return;

	delete List[index];
	List.erase( List.begin() + index );
}

ioTexture* ioTexManager::GetTexture( unsigned int index )
{
	if( index >= List.size() )
		return NULL;

	return List[index];
}

void ioTexManager::Clear()
{
	int i;
	for( i = List.size()-1; i >= 0; i-- )
	{
		DeleteTexture( i );
	}
	List.clear();
}

ioTexManager TManager;