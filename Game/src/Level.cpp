/*///////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
Plik:	Level.cpp
Autor:	Real_Noname (real_noname@wp.pl)
(C):	CODE RULERS (Real_Noname)
WWW:	www.coderulers.prv.pl
Opis:	Patrz -> Level.h

/////////////////////////////////////////////////////////
///////////////////////////////////////////////////////*/
#include "Level.h"

#include "GamePlayer.h"

#include "ItemAmmo.h"
#include "ItemArmor.h"
#include "ItemHealth.h"
#include "ItemWeapon.h"

CLevel* pGLevel = nullptr;

/*	KLASA CLevel
Patrz -> definicja klasy
*/
/*	KONSTRUKTOR	*/
CLevel::CLevel( CTexManager& texManager, CModelManager& modelManager ) :
	TexManager( texManager ),
	ModelManager( modelManager ),
	RenderList( texManager, modelManager ),
	blockWidth( 10.0f ),
	blockHeight( 10.0f ),
	blockDepth( 10.0f ),
	Rows(0),
	Cols(0),
	LevName( "UNKNOWN" ),
	file( "-" ),
	loaded( false )
{
	Version = 0;

	// I zerujemy list� wy�wietlania
	Floor = 0;
	Top = 0;
	Wall = 0;
	Reflect = 0;

	PlayerStartBlock = 0;
	PlayerStartAngle = 0;

	AllWin = 0;
	AllLose = 0;

	// Zerujemy inne warto�ci
	WeapCount = 0;
	StatObjCount = 0;
	BonusCount = 0;
	EnemyCount = 0;
	EnemyTypeCount = 0;

	// Zerujemy liste typ�w wroga
	EnemyType = NULL;

	Tex[0] = NULL;
	Tex[1] = NULL;
	Tex[2] = NULL;
}

/*	DESTRUKTOR	*/
CLevel::~CLevel()
{
	/*	Przy destrukcji wypada�o
	by zwolni� pami��
	*/
	Free();
}

void	CLevel::Update( const float fTD )
{
	UpdateList.Update( fTD );

	FlushDeleted();

	CollisionMng.Solve();

	RenderList.PreLoad();
	RenderList.Sort();
}

void	CLevel::Render()
{
	DrawLevel();
	RenderList.Render( true );
	
	glDepthMask( 0 );

	RenderList.Render( false );

	glDepthMask( 1 );
}

const bool	CLevel::AddEntity( ISceneEntity* pEntity )
{
	if( CScene::AddEntity( pEntity ) )
	{
		IRenderable* pRender = dynamic_cast<IRenderable*>( pEntity );
		IUpdateable* pUpdate = dynamic_cast<IUpdateable*>( pEntity );
		CObject* pObject = dynamic_cast<CObject*>( pEntity );

		if( pRender != nullptr )
			RenderList.Add( pRender );

		if( pUpdate != nullptr )
			UpdateList.Add( pUpdate );

		if( pObject != nullptr )
			CollisionMng.AddObject( pObject );

		return true;
	}

	return false;
}

const bool	CLevel::RemoveEntity( ISceneEntity* pEntity )
{
	if( CScene::RemoveEntity( pEntity ) )
	{
		IRenderable* pRender = dynamic_cast<IRenderable*>( pEntity );
		IUpdateable* pUpdate = dynamic_cast<IUpdateable*>( pEntity );
		CObject* pObject = dynamic_cast<CObject*>( pEntity );

		if( pRender != nullptr )
			RenderList.Remove( pRender );

		if( pUpdate != nullptr )
			UpdateList.Remove( pUpdate );

		if( pObject != nullptr )
			CollisionMng.DeleteObject( pObject );

		return true;
	}

	return false;
}

// Zwraca, czy poziom jest za�adowany i gotowy
bool CLevel::GetLoaded()
{
	return loaded;
}

// Zwraca ilo�� przeciwnik�w odczytanych z pliku
unsigned int CLevel::GetEnemyCount()
{
	return EnemyCount;
}

// Zwraca ilo�� obiekt�w statycznych odczytanych z pliku
unsigned int CLevel::GetSObjCount()
{
	return StatObjCount;
}

/*	METODA WCZYTUJ�CA DANE
Za�o�enia pliku s� proste.
Na pocz�tku podstawowe dane, 
a dalej jedziemy ze �cianami
i ca�� reszt� danych.
*/
bool CLevel::LoadLevel( const std::string &filename )
{
	if( filename == "" )
	{
		Log.Error( "GLEVEL( " + file + " ): �a�cuch znak�w jest pusty!" );
		return false;
	}

	// Najpierw definiujemy wska�nik na plik i go otwieramy
	std::fstream stream( filename, std::ios::in );

	if( !stream )
	{
		Log.Error( "GLEVEL( " + file + " ): Plik nie istnieje, lub podana �cie�ka jest b��dna: " + filename );
		return false;
	}

	// definiujemy zmienne pomocnicze
	std::string str;
	int i, j, k, l;

	// Pobieramy wersje poziomu oraz j� sprawdzamy
	str = GetClearLine( stream );
	if( !sscanf_s( str.c_str(), "E3DTLEV=%u", &Version ) )
	{
		Log.Error( "GLEVEL( " + file + " ): Nieprawid�owy plik poziomu!" );
		return false;
	}

	if( Version > GAME_VERSION )
	{
		Log.Error( "GLEVEL( " + file + " ): Zbyt wysoka wersja pliku!" );
		return false;
	}

	/*	Wcze�nijsze if'y by�y, by uzyska� pewno��,
	�e wszystko z nowym plikiem jest w porz�dku.
	Dlaczego tyle zachodu? By unikn�� sytuacji,
	kiedy �adujemy plik, na ju� istniej�cy level.
	Level istnieje, wi�c go kasujemy i czytamy plik.
	A teraz Zonk! Nie ma pliku! I co? zostajemy na
	lodzie bez levelu :). Teraz wiecie o co chodzi :)
	*/
	if( loaded )
	{
		Log.Report( "GLEVEL( " + file + " ): Prze�adowanie poziomu na : " + filename );
		Free();
		MainPlayer.Reset();
	}

	Log.Log( "GLEVEL( " + file + " ): Wczytywanie poziomu: " + filename );
	GUI.SendConMsg( "Wczytywanie poziomu: " + filename, false );
	file = filename;

	while( stream )
	{
		str = GetLine( stream );

		if( str == "END E3DTLEV" )
			break;

		str = ClearWhiteSpace( str );

		if( str == "HEADER" )
		{
			if( !LoadHeader( stream ) )
			{
				Log.Error( "GLEVEL( " + file + " ): B��d odczytu nag��wka!" );
				continue;
			}
		}
		else if( str == "TEXTURES" )
		{
			if( !LoadTextures( stream ) )
			{
				Log.Error( "GLEVEL( " + file + " ): B��d odczytu tekstur!" );
				continue;
			}
		}
		else if( str == "WALLS" )
		{
			if( !LoadWalls( stream ) )
			{
				Log.Error( "GLEVEL( " + file + " ): B��d odczytu �cian!" );
				continue;
			}
		}
		else if( str == "ITEMLIST" )
		{
			if( !LoadItemList( stream ) )
			{
				Log.Error( "GLEVEL( " + file + " ): B��d odczytu listy przedmiot�w!" );
				continue;
			}
		}
		else
			Log.Error( "GLEVEL( " + file + " ): Nierozpoznany ci�g " + str + "!" );
	}

	/*
	if( WeapCount > 0 )
	{
		str = GetLine( stream );

		if( str != "WEAPLIST" )
		{
			Log.Error( "GLEVEL( " + file + " ): Brak listy broni!" );
			Free();
			return false;
		}

		for( i = 0; i < WeapCount; i++ )
		{
			str = GetLine( stream );

			sscanf_s( str.c_str(), "%d=%d", &j, &k );

			CWeapon* Weap;
			switch( k )
			{
			case WEAPON_TYPE::SAW :
				Weap = new weSaw;
				break;

			case WEAPON_TYPE::PISTOL :
				Weap = new wePistol;
				break;

			case WEAPON_TYPE::MINIPHAZER :
				Weap = new weMiniPhazer;
				break;

			case WEAPON_TYPE::MINIGUN :
				Weap = new weMiniGun;
				break;

			case WEAPON_TYPE::ROCKET_LUNCHER :
				Weap = new CWeaponRocketLuncher;
				break;

				//case GAME_WEAP_PICK_A_BOO	:
			case WEAPON_TYPE::PHAZER :
				Weap = new wePhazer;
				break;

				//case GAME_WEAP_MINE :
			case WEAPON_TYPE::ATOM_BOMB :
				Weap = new weATOM_BOMBb;
				break;
			default:
				continue;
			}
			Weap->Init( ModelManager );
			Weap->Pos = GetBlockPos( j );
			Weap->NextPos = Weap->Pos;
			WManager.AddWeapon( ModelManager, Weap );
		}

		str = GetLine( stream );
		if( str != "END WEAPLIST" )
		{
			Log.Error( "GLEVEL( " + file + " ): Brak ko�ca listy broni!" );
			Free();
			return false;
		}
	}

	if( EnemyTypeCount > 0 )
	{
		str = GetLine( stream );
		EnemyType = new std::string[EnemyTypeCount];

		if( str != "ENEMYTYPELIST" )
		{
			Log.Error( "GLEVEL( " + file + " ): Brak listy typ�w wroga!" );
			Free();
			return false;
		}

		for( i = 0; i < EnemyTypeCount; i++ )
		{
			str = GetLine( stream );
			EnemyType[i] = str;
		}

		str = GetLine( stream );
		if( str != "END ENEMYTYPELIST" )
		{
			Log.Error( "GLEVEL( " + file + " ): Brak ko�ca listy typ�w wroga!" );
			Free();
			return false;
		}
	}

	if( EnemyCount > 0 && EnemyTypeCount > 0)
	{
		str = GetLine( stream );

		if( str != "ENEMYLIST" )
		{
			Log.Error( "GLEVEL( " + file + " ): Brak listy wrog�w!" );
			Free();
			return false;
		}

		for( i = 0; i < EnemyCount; i++ )
		{
			str = GetLine( stream );

			sscanf_s( str.c_str(), "%d=%d,%d", &j, &k, &l );

			CEnemy* Enemy = new CEnemy( ModelManager );
			Enemy->SetStartPos( this->GetBlockPos( j ) );
			Enemy->SetStartAngle( (float)l );
			Enemy->LoadEnemy( EnemyType[k] );
			Enemy->Reset();
			ThingManager.AddThing( Enemy );
		}

		str = GetLine( stream );
		if( str != "END ENEMYLIST" )
		{
			Log.Error( "GLEVEL( " + file + " ): Brak ko�ca listy wrog�w!" );
			Free();
			return false;
		}
	}

	if( StatObjCount > 0 )
	{
		str = GetLine( stream );

		if( str != "STATOBJLIST" )
		{
			Log.Error( "GLEVEL( " + file + " ): Brak listy statycznych objekt�w!" );
			Free();
			return false;
		}

		for( i = 0; i < StatObjCount; i++ )
		{
			str = GetLine( stream );

			sscanf_s( str.c_str(), "%d,%d=", &k, &l );

			//gameStatObj *obj = new gameStatObj;
			//obj->LoadObj( GetParamStr( str ) );
			//obj->Pos = this->GetBlockPos( k );
			//obj->SetAngle( l );
			//ThingManager.AddThing( obj );
		}

		str = GetLine( stream );
		if( str != "END STATOBJLIST" )
		{
			Log.Error( "GLEVEL( " + file + " ): Brak ko�ca listy statycznych objekt�w!" );
			Free();
			return false;
		}
	}
	str = GetLine( stream );
	if( str != "END E3DTLEV" )
	{
		Log.Error( "GLEVEL( " + file + " ): Brak ko�ca pliku!" );
	}
	*/

	MainPlayer.SetStartPos( this->GetBlockPos( PlayerStartBlock ) );
	MainPlayer.SetStartAngle( (float)PlayerStartAngle );
	MainPlayer.Reset();

	CollisionMng.AddObject( &MainPlayer );

	loaded = true;

	return true;
}

/*	METODA RYSUJ�CA - �CIANY
W zale�no�ci od tego jak�
�ciane chcemy, tak� rysuje
razem z koordynatami tekstur
*/
void CLevel::DrawWall( unsigned int wall )
{
	float mult = GUI.GetTexPerPlane( GAME_TEX_WALL );
	switch( wall )
	{
	case LEV_WALL_TOP :
		{
			glBegin( GL_TRIANGLE_STRIP );
			glNormal3f( 0.0f, 0.0f, 1.0f );
			glTexCoord2f( mult, mult );
			glVertex3f( -5.0f, 5.0f, -5.0f );
			glTexCoord2f( mult, 0.0f );
			glVertex3f( -5.0f, -5.0f, -5.0f );
			glTexCoord2f( 0.0f, mult );
			glVertex3f( 5.0f, 5.0f, -5.0f );
			glTexCoord2f( 0.0f, 0.0f );
			glVertex3f( 5.0f, -5.0f, -5.0f );
			glEnd();
		}
		break;
	case LEV_WALL_BOTTOM :
		{
			glBegin( GL_TRIANGLE_STRIP );
			glNormal3f( 0.0f, 0.0f, -1.0f );
			glTexCoord2f( mult, mult );
			glVertex3f( 5.0f, 5.0f, 5.0f );
			glTexCoord2f( mult, 0.0f );
			glVertex3f( 5.0f, -5.0f, 5.0f );
			glTexCoord2f( 0.0f, mult );
			glVertex3f( -5.0f, 5.0f, 5.0f );
			glTexCoord2f( 0.0f, 0.0f );
			glVertex3f( -5.0f, -5.0f, 5.0f );
			glEnd();
		}
		break;
	case LEV_WALL_LEFT :
		{
			glBegin( GL_TRIANGLE_STRIP );
			glNormal3f( 1.0f, 0.0f,0.0f );
			glTexCoord2f( mult, mult );
			glVertex3f( -5.0f, 5.0f, 5.0f );
			glTexCoord2f( mult, 0.0f );
			glVertex3f( -5.0f, -5.0f, 5.0f );
			glTexCoord2f( 0.0f, mult );
			glVertex3f( -5.0f, 5.0f, -5.0f );
			glTexCoord2f( 0.0f, 0.0f );
			glVertex3f( -5.0f, -5.0f, -5.0f );
			glEnd();
		}
		break;
	case LEV_WALL_RIGHT :
		{
			glBegin( GL_TRIANGLE_STRIP );
			glNormal3f( -1.0f, 0.0f, 0.0f );
			glTexCoord2f( mult, mult );
			glVertex3f( 5.0f, 5.0f, -5.0f );
			glTexCoord2f( mult, 0.0f );
			glVertex3f( 5.0f, -5.0f, -5.0f );
			glTexCoord2f( 0.0f, mult );
			glVertex3f( 5.0f, 5.0f, 5.0f );
			glTexCoord2f( 0.0f, 0.0f );
			glVertex3f( 5.0f, -5.0f, 5.0f );
			glEnd();
		}
		break;
	}
}

/*	METODA RYSUJ�CA - POD�OGA I SUFIT
Metody podobne do poprzedniej - rysuj�
pod�oge i sufit razem z koordynatami.
*/
void CLevel::DrawFloor()
{
	float mult = GUI.GetTexPerPlane( GAME_TEX_FLOOR );
	glBegin( GL_TRIANGLE_STRIP );
	glNormal3f( 0.0f, 1.0f, 0.0f );
	glTexCoord2f( mult, mult );
	glVertex3f( -5.0f, -5.0f, -5.0f );
	glTexCoord2f( mult, 0.0f );
	glVertex3f( -5.0f, -5.0f, 5.0f );
	glTexCoord2f( 0.0f, mult );
	glVertex3f( 5.0f, -5.0f, -5.0f );
	glTexCoord2f( 0.0f, 0.0f );
	glVertex3f( 5.0f, -5.0f, 5.0f );
	glEnd();
}

void CLevel::DrawTop()
{
	float mult = GUI.GetTexPerPlane( GAME_TEX_TOP );
	glBegin( GL_TRIANGLE_STRIP );
	glNormal3f( 0.0f, -1.0f, 0.0f );
	glTexCoord2f( mult, mult );
	glVertex3f( 5.0f, 5.0f, -5.0f );
	glTexCoord2f( mult, 0.0f );
	glVertex3f( 5.0f, 5.0f, 5.0f );
	glTexCoord2f( 0.0f, mult );
	glVertex3f( -5.0f, 5.0f, -5.0f );
	glTexCoord2f( 0.0f, 0.0f );
	glVertex3f( -5.0f, 5.0f, 5.0f );
	glEnd();
}

/*	Ta metoda jednorazowo wywo�uje dwie
inne metody
*/
void CLevel::InitLevel()
{
	// Gdy poziom ju� jest za�adowany, to trzeba przerwa� inicjalizacje
	if( !loaded )
		return;

	WinFlags.VerifyFlags();
	LoseFlags.VerifyFlags();

	BuildVisual();
	BuildPhysic();
}

/*	W tej metodzie budujemy cz�� wizualn�
poziomu, oddzielnie od reszty logiki. W programowaniu gier jest ZAWSZE
zasada odzielania tych cz�ci.
*/
void CLevel::BuildVisual()
{
	if( !loaded )
		return;

	GUI.SendConMsg( "Poziom: Tworzenie czesci wizualnej...", false );
	// Zmienne pomocnicze
	int i, j, blk;

	// Generujemy list�
	Top = glGenLists( 1 );
	Wall = glGenLists( 1 );
	Floor = glGenLists( 1 );
	/*	Teraz tworzymy list�. Listy wy�wietlania
	maj� te zalete, �e raz stworzone BARDZO
	przyspieszaj� wy�wietlanie, dlatego op�aca
	si� je stosowa�. W zamian nie mo�na nic zmieni�
	w li�cie, no chyba, �e tworz�c j� od nowa.
	*/
	/*	Ta cz�� kodu tysuje pod�oge, na ca�ej
	powieszchni poziomu.
	*/
	glNewList( Floor, GL_COMPILE );
	glPushMatrix();
	glTranslatef( 5, 0, -5 );
	for( i = 0; i < Rows; i++ )
	{
		for( j = 0; j < Cols; j++ )
		{
			glPushMatrix();

			glTranslatef( j*10, 0, -(i*10) );

			DrawFloor();

			glPopMatrix();
		}
	}
	glPopMatrix();
	glEndList();
	// To samo co poprzednio, tylko, �e z sufitem
	glNewList( Top, GL_COMPILE );
	glPushMatrix();
	glTranslatef( 5, 0, -5 );
	for( i = 0; i < Rows; i++ )
	{
		for( j = 0; j < Cols; j++ )
		{
			glPushMatrix();

			glTranslatef( j*10, 0, -(i*10) );

			DrawTop();

			glPopMatrix();
		}
	}
	glPopMatrix();
	glEndList();
	/*	W tej p�tli s� rysowane �ciany
	w zale�no�ci jaka �ciana jest zawarta
	w fladze, tak� rysujemy.
	*/
	glNewList( Wall, GL_COMPILE );
	glPushMatrix();
	glTranslatef( 5, 0, -5 );
	for( i = 0; i < Rows; i++ )
	{
		for( j = 0; j < Cols; j++ )
		{
			glPushMatrix();

			glTranslatef( j*10, 0.0f, -(i*10) );

			int in = i * Cols + j;

			blk = block[ in ].walls;

			if( blk & LEV_WALL_TOP )
				DrawWall( LEV_WALL_TOP );

			if( blk & LEV_WALL_BOTTOM )
				DrawWall( LEV_WALL_BOTTOM );

			if( blk & LEV_WALL_LEFT )
				DrawWall( LEV_WALL_LEFT );

			if( blk & LEV_WALL_RIGHT )
				DrawWall( LEV_WALL_RIGHT );

			glPopMatrix();
		}
	}
	glPopMatrix();
	glEndList();
}

/*	Ta metoda jest odpowiedzilna, za cz�� fizyczn� poziomu
buduje, m.in. punkty i p�aszczyzny, w oparciu o kt�re
b�d� wykrywane kolizje.
*/
void CLevel::BuildPhysic()
{
	if( !loaded )
		return;

	GUI.SendConMsg( "Poziom: Tworzenie czesci fizycznej...", false );
	// Zmienne pomocznicze
	int i, j, k, ai, aj;
	CLvlBlock* blk;
	CLvlBlock* SideBlock[4];
	float nx, ny;

	/*	P�tla sprawdza ka�de 4 �ciany
	w ka�dym bloku, czy nale�y stworzy�
	w tym miejscu przeszkode.
	*/
	for( i = 0; i < Rows; i++ )
	{
		for( j = 0; j < Cols; j++ )
		{
			blk = &block[ i * Cols + j ];

			for( k = 0; k < 4; k++ )
			{
				/*	BTW: tu mo�na by�o by wymy�le� jaki� algorytm,
				ale nie mia�em pomys�u, wi�c po prostu pos�u�y�em
				si� switch'em.
				*/
				switch( k )
				{
					/*	Wybieramy jakie ustawienia wybra� dla danej �ciany.
					nx, ny - wektor normalny �ciany ( wskazuje gdzie jest prz�d )
					ai, aj - dodatek do punktu dolnego, prawego ( bo jako� trzeba ustawi� rogi
					bloku )
					*/
				case 0 :
					nx = 0.0f;
					ny = 1.0f;
					ai = 0;
					aj = 1;
					break;
				case 1 :
					nx = -1.0f;
					ny = 0.0f;
					ai = 1;
					aj = 1;
					break;
				case 2 :
					nx = 0.0f;
					ny = -1.0f;
					ai = 1;
					aj = 0;
					break;
				case 3 :
					nx = 1.0f;
					ny = 0.0f;
					ai = 0;
					aj = 0;
					break;
				}

				// Najpierw tworzymy odpowiednie rogi bloku.
				blk->Corner[k].Set( (float)j * 10.0f + (float)( ai * 10 ), 0.0f, -(float)i * 10.0f + (float)( aj * -10 ) );
				// Tworzymy wektor normalny p�aszczyzny �ciany
				blk->WPlane[k].Normal.Set( nx, 0.0f, ny );
				// Wyliczamy przesuni�cie p�aszczyzny, w oparciu o Punkt ( w tym przypadku r�g bloku ) i Wektor Normalny
				blk->WPlane[k].D = -blk->Corner[k].Dot( blk->WPlane[k].Normal );
			}
		}
	}

	/*	Zauwa�y�em, �e kiedy teraz pr�bowa�bym 
	wej�� na jaki� r�g na �cianach, to bym 
	po prostu... przeszed� na druga stron�!
	Co� takiego nie mo�e mie� miejsca, dlatego
	tworzymy odpowiednie rogi "kolizyjne". 
	A dlaczego nie wykorzystamy ju� wcze�niej
	stworzonych?? Bo sytuacja gdy si� blokujemy
	na �rodku pustej przestrzenie, te� jest 
	absurdalna :)
	*/
	for( i = 0; i < Rows; i++ )
	{
		for( j = 0; j < Cols; j++ )
		{
			blk = &block[ i * Cols + j ];

			/*	Znajdujemy bloki s�siednie, by mo�na
			by�o znale�� rogi �cian.
			*/
			if( i < Rows - 1 )
				SideBlock[0] = &block[ (i+1) * Cols + j ];
			else
				SideBlock[0] = NULL;

			if( j < Cols - 1 )
				SideBlock[1] = &block[ i * Cols + (j+1) ];
			else
				SideBlock[1] = NULL;

			if( i > 0 )
				SideBlock[2] = &block[ (i-1) * Cols + j ];
			else
				SideBlock[2] = NULL;

			if( j > 0 )
				SideBlock[3] = &block[ i * Cols + (j-1) ];
			else
				SideBlock[3] = NULL;

			blk->CornerCount = 0;
			/*	Teraz na ka�d� �ciany ( i jednocze�nie blok s�siedni ze �cian� )
			szukamy zetkni�cia lub rogu. Gdy taki znajdujemy, to zapisujemy
			ten r�g i zwi�kszamy liczb� rog�w do sprawdzenia.
			*/
			for( k = 0; k < 4; k++ )
			{
				if( k == 0 )
					if( ( SideBlock[0] == NULL || !( SideBlock[0]->walls & LEV_WALL_RIGHT ) ) && 
						( SideBlock[1] == NULL || !( SideBlock[1]->walls & LEV_WALL_TOP ) ) )
						continue;

				if( k == 1 )
					if( ( SideBlock[1] == NULL || !( SideBlock[1]->walls & LEV_WALL_BOTTOM ) ) && 
						( SideBlock[2] == NULL || !( SideBlock[2]->walls & LEV_WALL_RIGHT ) ) )
						continue;

				if( k == 2 )
					if( ( SideBlock[2] == NULL || !( SideBlock[2]->walls & LEV_WALL_LEFT ) ) && 
						( SideBlock[3] == NULL || !( SideBlock[3]->walls & LEV_WALL_BOTTOM ) ) )
						continue;

				if( k == 3 )
					if( ( SideBlock[3] == NULL || !( SideBlock[3]->walls & LEV_WALL_TOP ) ) && 
						( SideBlock[0] == NULL || !( SideBlock[0]->walls & LEV_WALL_LEFT ) ) )
						continue;

				if( SideBlock[k] != NULL )
					blk->TCorner[blk->CornerCount] = SideBlock[k]->Corner[(k+2)%4];
				else blk->TCorner[blk->CornerCount] = SideBlock[(k+1)%4]->Corner[k];

				blk->CornerCount++;

			}

		}
	}

}

/*	Tu odbywa si� rysowanie levelu. 
( takie proste, �e a� g�upie :) )
*/
void CLevel::DrawLevel()
{
	if( !loaded )
		return;

	if( GUI.GetReflection() )
	{
		this->DrawAllTop();
		this->DrawAllWall();

		glPushMatrix();
		glTranslatef( 0.0f, -10.0f, 0.0f );
		glScalef( 1.0f, -1.0f, 1.0f );
		glDisable( GL_CLIP_PLANE0 );
		glEnable( GL_CLIP_PLANE1 );
		glFrontFace( GL_CW );
		this->DrawAllTop();
		this->DrawAllWall();
		glFrontFace( GL_CCW );
		glDisable( GL_CLIP_PLANE1 );
		glEnable( GL_CLIP_PLANE0 );
		glPopMatrix();
	}
	else
	{
		this->DrawAllTop();
		this->DrawAllWall();
		this->DrawAllFloor();
	}
}

void CLevel::DrawAllTop()
{
	if( !loaded )
		return;

	Tex[1]->Activate( GUI.GetTexDLevel() );
	glCallList( Top );
}

void CLevel::DrawAllWall()
{
	if( !loaded )
		return;

	Tex[0]->Activate( GUI.GetTexDLevel() );
	glCallList( Wall );
}

void CLevel::DrawAllFloor()
{
	if( !loaded )
		return;

	Tex[2]->Activate( GUI.GetTexDLevel() );
	glCallList( Floor );
}

void CLevel::DrawReflect()
{
	if( !GUI.GetReflection() )
		return;

	glPushMatrix();
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	glColor4f( 1.0f, 1.0f, 1.0f, GUI.GetRefLevel() );
	glEnable( GL_BLEND );
	this->DrawAllFloor();
	glColor4f( 1.0f, 1.0f, 1.0f, 1.0f );
	glDisable( GL_BLEND );
	glPopMatrix();
}

/*	Ta metoda zwraca nazw� poziomu wyczytan� z pliku
(lub domy�ln�).
*/
std::string CLevel::GetLevelName()
{
	return LevName;
}

/*	Ta zwraca dany block z okre�lonego wiersza i kolumny
*/
CLvlBlock* CLevel::GetBlock( unsigned int i, unsigned int j ) const
{
	if( i >= 0 && j >= 0 )
	{
		if( j < Rows && i < Cols )
		{
			return (CLvlBlock*)&block[ j * Cols + i ];
		}
	}
	return NULL;
}

CLvlBlock* CLevel::GetBlock( Vector3f Pos ) const
{
	return GetBlock( (unsigned int)Pos.X, (unsigned int)Pos.Z );
}

CLvlBlock* CLevel::GetBlock( unsigned int i ) const
{
	return (CLvlBlock*)&block[i];
}

unsigned int CLevel::GetBlockCount()
{
	return Rows * Cols;
}

const Vector3f CLevel::GetBlockPos( const unsigned i ) const
{
	return GetBlockPos( i % Cols, i / Cols );
}

const Vector3f	CLevel::GetBlockPos( const int x, const int y ) const
{
	float halfW = blockWidth / 2.0f;
	float halfD = blockDepth / 2.0f;

	return Vector3f( (float)(x) * blockWidth + halfW, 0.0f, -(float)(y) * blockDepth - halfD );
}

void CLevel::CheckWLFlags()
{
	if( !this->loaded )
		return;
	if( GUI.IsShowingWLScr() )
		return;

	if( AllWin )
	{
		if( this->WinFlags.CheckAllFlags() )
		{
			GUI.ShowWinScr();
		}
	}
	else if( this->WinFlags.CheckOneFlag() )
	{
		GUI.ShowWinScr();
	}

	if( AllLose )
	{
		if( this->LoseFlags.CheckAllFlags() )
			GUI.ShowLoseScr();
	}
	else if( this->LoseFlags.CheckOneFlag() )
	{
		GUI.ShowLoseScr();
	}	
}


/*	Metoda czy�ci pami�c zarezerwowan� dla
tej klasy.
*/
void CLevel::Free()
{
	if( !loaded )
		return;

	GUI.SendConMsg( "Poziom: Zwalnianie pamieci...", false );

	block.clear();

	if( EnemyType != NULL )
	{
		delete[] EnemyType;
		EnemyType = NULL;
	}

	Rows = 0;
	Cols = 0;

	LevName = "UNKNOWN LEVEL";
	WinFlags.flags = 0;
	LoseFlags.flags = 0;

	glDeleteLists( Top, 1 );
	Top = 0;
	glDeleteLists( Wall, 1 );
	Wall = 0;
	glDeleteLists( Floor, 1 );
	Floor = 0;

	Tex[0] = NULL;
	Tex[1] = NULL;
	Tex[2] = NULL;

	loaded = false;
}

void	CLevel::OnDeleteEntity( ISceneEntity* pEntity )
{
	IRenderable* pRender = dynamic_cast<IRenderable*>( pEntity );
	IUpdateable* pUpdate = dynamic_cast<IUpdateable*>( pEntity );
	CObject* pObject = dynamic_cast<CObject*>( pEntity );

	if( pRender != nullptr )
		RenderList.Remove( pRender );

	if( pUpdate != nullptr )
		UpdateList.Remove( pUpdate );

	if( pObject != nullptr )
		CollisionMng.DeleteObject( pObject );
}

const bool	CLevel::ParseCoords(const std::string& str, int& x, int& y)
{
	if(str.empty())
		return false;

	std::vector<std::string> coordList;
	SplitString(str, ",", coordList);

	if(coordList.size() < 2)
		return false;

	x = atoi(coordList[0].c_str());
	y = atoi(coordList[1].c_str());

	return true;
}

const bool	CLevel::ParseItem(const std::string& str, int& x, int& y, ITEM_TYPE& type, std::vector<std::string>& params)
{
	auto pos = str.find( "=" );
	if( pos == std::string::npos )
		return false;

	if( !ParseCoords( str.substr( 0, pos ), x, y ) )
		return false;

	auto itemdef = str.substr(pos + 1);

	pos = itemdef.find("(");
	auto end = itemdef.find(")");

	if(pos == std::string::npos || end == std::string::npos)
		return false;

	type = ::ParseItem(itemdef.substr(0, pos));

	SplitString( itemdef.substr(pos + 1, end - pos - 1), ",", params );

	return true;
}

const bool	CLevel::LoadHeader( std::fstream& stream )
{
	Log.Log( "GLEVEL( " + file + " ): �adowanie nag��wka.");

	std::string str, name, value;
	while( stream )
	{
		str = GetLine( stream );

		if( str == "END HEADER" )
			return true;

		if( !ParseNameValue( str, name, value ) )
		{
			Log.Error( "GLEVEL( " + file + " ): Nieprawid�owy ci�g w nag��wku: " + str + "." );
			continue;
		}

		if( name == "NAME" )
			LevName = value;

		else if( name == "ROWS" )
			Rows = StrToUInt( value );

		else if( name == "COLS" )
			Cols = StrToUInt( value );

		/*
		if( ContainsString( str, "WEAPCOUNT" ) )
		{
		if( !sscanf_s( str.c_str(), "WEAPCOUNT=%u", &WeapCount ) )
		{
		Log.Error( "GLEVEL( " + file + " ): Nie mo�na odczyta� liczby broni!" );
		Free();
		return false;
		}
		}
		if( ContainsString( str, "BONUSCOUNT" ) )
		{
		if( !sscanf_s( str.c_str(), "BONUSCOUNT=%u", &BonusCount ) )
		{
		Log.Error( "GLEVEL( " + file + " ): Nie mo�na odczyta� liczby broni!" );
		Free();
		return false;
		}
		}
		if( ContainsString( str, "ENEMYTYPECOUNT" ) )
		{
		if( !sscanf_s( str.c_str(), "ENEMYTYPECOUNT=%u", &EnemyTypeCount ) )
		{
		Log.Error( "GLEVEL( " + file + " ): Nie mo�na odczyta� liczby typ�w wroga!" );
		Free();
		return false;
		}
		}
		if( ContainsString( str, "ENEMYCOUNT" ) )
		{
		if( !sscanf_s( str.c_str(), "ENEMYCOUNT=%u", &EnemyCount ) )
		{
		Log.Error( "GLEVEL( " + file + " ): Nie mo�na odczyta� liczby wrog�w!" );
		Free();
		return false;
		}
		}
		if( ContainsString( str, "PLAYERSTARTPOS" ) )
		{
		if( !sscanf_s( str.c_str(), "PLAYERSTARTPOS=%u", &PlayerStartBlock ) )
		{
		Log.Error( "GLEVEL( " + file + " ): Nie mo�na odczyta� pozycji startowej gracza!" );
		Free();
		return false;
		}
		}
		if( ContainsString( str, "PLAYERSTARTANGLE" ) )
		{
		if( !sscanf_s( str.c_str(), "PLAYERSTARTANGLE=%u", &PlayerStartAngle ) )
		{
		Log.Error( "GLEVEL( " + file + " ): Nie mo�na odczyta� obrotu startowego gracza!" );
		Free();
		return false;
		}
		}
		if( ContainsString( str, "STATOBJ" ) )
		{
		if( !sscanf_s( str.c_str(), "STATOBJ=%u", &StatObjCount ) )
		{
		Log.Error( "GLEVEL( " + file + " ): Nie mo�na odczyta� liczby objekt�w statycznych!" );
		Free();
		return false;
		}
		}
		if( ContainsString( str, "WINALL" ) )
		{
		if( !sscanf_s( str.c_str(), "WINALL=%d", &AllWin ) )
		{
		Log.Error( "GLEVEL( " + file + " ): Nie mo�na odczyta� flagi!" );
		Free();
		return false;
		}
		}
		if( ContainsString( str, "LOSEALL" ) )
		{
		if( !sscanf_s( str.c_str(), "LOSEALL=%d", &AllLose ) )
		{
		Log.Error( "GLEVEL( " + file + " ): Nie mo�na odczyta� flagi!" );
		Free();
		return false;
		}
		}
		if( ContainsString( str, "WINFLAGS" ) )
		{
		if( !sscanf_s( str.c_str(), "WINFLAGS=%u", &WinFlags.flags ) )
		{
		Log.Error( "GLEVEL( " + file + " ): Nie mo�na odczyta� flagi!" );
		Free();
		return false;
		}
		}
		if( ContainsString( str, "WINBLOCK" ) )
		{
		if( !sscanf_s( str.c_str(), "WINBLOCK=%u", &WinFlags.BlockID ) )
		{
		Log.Error( "GLEVEL( " + file + " ): Nie mo�na odczyta� flagi!" );
		Free();
		return false;
		}
		}
		if( ContainsString( str, "LOSEFLAGS" ) )
		{
		if( !sscanf_s( str.c_str(), "LOSEFLAGS=%u", &LoseFlags.flags ) )
		{
		Log.Error( "GLEVEL( " + file + " ): Nie mo�na odczyta� flagi!" );
		Free();
		return false;
		}
		}
		if( ContainsString( str, "LOSEBLOCK" ) )
		{
		if( !sscanf_s( str.c_str(), "LOSEBLOCK=%u", &LoseFlags.BlockID ) )
		{
		Log.Error( "GLEVEL( " + file + " ): Nie mo�na odczyta� flagi!" );
		Free();
		return false;
		}
		}*/

		else 
			Log.Error( "GLEVEL( " + file + " ): Nierozpoznana warto�� " + name + " w ci�gu: " + str + "." );
	}

	Log.Error( "GLEVEL( " + file + " ): Brak ko�ca nag��wka." );
	return false;
}

const bool	CLevel::LoadTextures( std::fstream& stream )
{
	Log.Log( "GLEVEL( " + file + " ): �adowanie tekstur.");

	std::string str, name, value;
	while( stream )
	{
		str = GetLine( stream );

		if( str == "END TEXTURES" )
			return true;

		if( !ParseNameValue( str, name, value ) )
		{
			Log.Error( "GLEVEL( " + file + " ): Nieprawid�owy ci�g w li�cie tekstur: " + str + "." );
			continue;
		}
		else if( name == "WALL" )
		{
			if( !( Tex[0] = TexManager.Get( value ) ) )
			{
				Log.Error( "GLEVEL( " + file + " ): Nie uda�o si� za�adowa� tekstury �cian!" );
				continue;
			}
		}
		else if( name == "CEILING" )
		{
			if( !( Tex[1] = TexManager.Get( value ) ) )
			{
				Log.Error( "GLEVEL( " + file + " ): Nie uda�o si� za�adowa� tekstury sufitu!" );
				continue;
			}
		}
		else if( name == "FLOOR" )
		{
			if( !( Tex[2] = TexManager.Get( value ) ) )
			{
				Log.Error( "GLEVEL( " + file + " ): Nie uda�o si� za�adowa� tekstury pod�ogi!" );
				continue;
			}
		}
	}

	Log.Error( "GLEVEL( " + file + " ): Brak zako�czenia liczby tesktur!" );
	return false;
}

const bool	CLevel::LoadWalls( std::fstream& stream )
{
	Log.Log( "GLEVEL( " + file + " ): �adowanie �cian.");

	/*	Teraz tworzymy tablic� odpowiednich
	rozmiar�w zdoln� pomie�ci� nast�pne
	dane. S� to liczby ca�kowite jednoznacznie
	okre�laj�ce zawarte w nich �ciany ( patrz
	definicje makrowe )
	*/
	std::string str;
	block.resize( Rows * Cols );
	for( unsigned i = 0; i < block.size(); i++ )
	{
		str = GetClearLine( stream );

		if(ContainsString( str, "," ))
		{
			std::vector<std::string> list;
			SplitString( str, ",", list );

			for(unsigned j = 0; j < list.size() && i + j < block.size(); j++ )
			{
				// Wpisujemy t� liczb� do sk�adowej walls
				//sscanf_s( list[j].c_str(), "%d", &block[i + j].walls );
				block[i + j].LoadWalls( list[j] );
				block[i + j].CornerCount = 0;
			}
			if( list.size() > 0 )
				i += list.size() - 1;
		}
		else
		{
			// Wpisujemy t� liczb� do sk�adowej walls
			//sscanf_s( str.c_str(), "%d", &block[i].walls );
			block[i].LoadWalls( str );
			block[i].CornerCount = 0;
		}
	}

	str = GetLine( stream );
	if( str != "END WALLS" )
	{
		Log.Error( "GLEVEL( " + file + " ): Brak ko�ca listy �cian!" );
		Free();
		return false;
	}

	return true;
}

const bool	CLevel::LoadItemList( std::fstream& stream )
{
	Log.Log( "GLEVEL( " + file + " ): �adowanie listy przedmiot�w.");

	std::string str;
	std::vector<std::string> params;
	ITEM_TYPE type;
	int x, y;

	while( stream )
	{
		str = GetLine( stream );

		if( str == "END ITEMLIST" )
			return true;

		str = ClearWhiteSpace( str );	

		params.clear();
		if( ParseItem( str, x, y, type, params ) )
		{
			CItem* item = nullptr;
			switch (type)
			{
				break;
			case ITEM_TYPE::AMMO:
				item = new CItemAmmo( ParseWeapon( params[0] ) , StrToUInt( params[1] ) );
				break;

			case ITEM_TYPE::HEALTH:
				item = new CItemHealth( StrToFloat( params[0] ) );
				break;

			case ITEM_TYPE::ARMOR:
				item = new CItemArmor( StrToFloat( params[0] ) );
				break;

			case ITEM_TYPE::WEAPON:
				item = new CItemWeapon( ParseWeapon( params[0] ), StrToUInt( params[1] ) );
				break;

			case ITEM_TYPE::UNKNOWN:
			default:
				Log.Error("GLEVEL( " + file + " ): B��d parsowania przedmiotu, nieznany typ z parsowania ci�gu: " + str + ".");
				break;
			}

			if(item != nullptr)
			{
				item->Pos = GetBlockPos( x, y );
				AddEntity( item );
			}
		}
		else
		{
			Log.Error("GLEVEL( " + file + " ): B��d parsowania przedmiotu dla ci�gu: " + str + ".");
		}
	}

	Log.Error( "GLEVEL( " + file + " ): Brak ko�ca listy przedmiot�w!" );
	return false;
}


/*	To jest odzielna funkcja sprawdzaj�ca kolizje
obiektu CDynamic, ze �cianami podanego bloku.
Jest to samodzielna funkcja, by mo�na by�o j� wygonie
u�y�, nie korzystaj�c z klasy CLevel.
*/
bool TestCollBlock( CDynamic* Dum, CLvlBlock* Block, bool testthing )
{
	if( Block == NULL )
		return false;


	// zmienne pomocnicze
	int i;
	Vector3f A, B, Normal, V, P;
	float wynik, D, W;
	bool is = false;

	/*	A teraz troche trygonometrii :)
	Aby wykry� kolizje, w grach 3d u�ywa
	sie sprawdzenia przeci�cia p�aszczyzny przez
	sfer� ( kul� ). Polega ona, �e do po�orzenia kuli
	dodajemy wektor o d�ugo�ci r�wnej promieniowy kuli.
	Ca�e sprawdzenie opiera si� na dw�ch takich punktach
	aktualnej pozycji i nast�pnej pozycji. Nastepnie spraw-
	dzamy po�orzenie tych dw�ch punkt�w wzgl�dem p�aszczyzny
	�ciany. Je�eli le�� po tej samej stronie, to nie ma kolizji,
	a je�eli po r�nych, to kolizja nast�pi(�a). W grach 3D dodatkowo
	sprawdza si� p�aszczyzny na kraw�dziach �ciany, by stwierdzi� czy 
	kolizja nast�pi�a na �cianie ( bo sama p�aszczyzna nie jest
	ograniczona i mo�e si� roci�ga� poza �cian� ), ale tu
	operujemy na 4 �cianach, w dodatku pewnie ograniczonych, wi�c
	nie zachodzi taka potrzeba :)
	*/
	for( i = 0; i < 4; i++ )
	{
		if( i == 0 && !( Block->walls & LEV_WALL_TOP ) )
			continue;
		if( i == 1 && !( Block->walls & LEV_WALL_RIGHT ) )
			continue;
		if( i == 2 && !( Block->walls & LEV_WALL_BOTTOM ) )
			continue;
		if( i == 3 && !( Block->walls & LEV_WALL_LEFT ) )
			continue;

		Normal = Block->WPlane[i].Normal;
		D = Block->WPlane[i].D;

		/* FIXME: nie wiem dlaczego, ale je�eli aktualna
		pozycja ma dodany promie� to cz�sto kolizja
		nie jest wykryta (?) kiedy� to musze naprawi�...
		*/
		// Aktualna pozycja
		A = Dum->Pos;// + Normal.Reverse() * R;
		// Nast�pna pozycja
		B = Dum->NextPos + Normal.Reverse() * Dum->Radius;

		// Wyra�enie liczy, gdzie s� punkty wzgl�dem p�aszczyzny
		wynik = ( Normal.Dot( A ) + D ) * ( Normal.Dot( B ) + D );

		// wynik > 0 - po tej samej stronie
		// wynik < 0 - po r�nych stronach
		if( wynik >= 0 ) 
			continue;

		/*	No dobra, wiemy, �e nast�pi(�a) kolizja. Co dalej??
		Najlepiej wyliczy� punkt przeci�cia p�aszczyzny i 
		wyliczy� now� pozycj� nast�pnego ruchu w oparciu
		o wektor normalny �ciany o d�ugo�ci promienia.
		To dzia�a i nie wygl�da sztucznie :)
		*/
		// Wyliczamy punkt przeci�cia
		V = A - B;
		W = ( Normal.Dot( B ) + D ) / Normal.Dot( V );
		P = B - V * W;

		is = true;
		// i tworzymy now� pozycj�.
		Dum->NextPos = P + Normal * Dum->Radius;
	}

	/*	No dobra, wykrywamy kolizje na �cianach, ale
	co zrobi� gdy block nie ma �cian, a w s�siednich
	jest r�g? Najlepiej sprawdzi� czy odleg�o��
	rogu od nast�pnej pozycji nie jest mniejsza od
	promienia :). Wtedy wystarczy stworzy� now� p�aszczyzne
	na podstawie rogu, a pod wektor normalny podstawi� wektor
	od pozycji gracza, do rogu. Potem powtarzamy regu�ke i 
	wszystko pi�knie dzia�a :)
	*/

	for( i = 0; i < Block->CornerCount; i++ )
	{
		if( mathDistSq( Dum->NextPos, Block->TCorner[i] ) > POW(Dum->Radius) )
			continue;

		// liczymy wektor od rogu, do pozycji
		V = ( Dum->NextPos - Block->TCorner[i] ).Normalize();
		// i go normalizujemy, by si� nadawa� do wyliczenia odleg�o�ci
		Normal = V;
		D = -Block->TCorner[i].Dot( V );

		// Aktualna pozycja
		A = Dum->Pos;// + Normal.Reverse() * R;
		// Nast�pna pozycja
		B = Dum->NextPos + Normal.Reverse() * Dum->Radius;

		// Powtarzamy poprzedni� regu�ke...
		wynik = ( Normal.Dot( A ) + D ) * ( Normal.Dot( B ) + D );
		if( wynik >= 0 ) 
			continue;
		V = A - B;
		W = ( Normal.Dot( B ) + D ) / Normal.Dot( V );
		P = B - V * W;

		is = true;
		// i tworzymy now� pozycj�.
		Dum->NextPos = P + Normal * Dum->Radius;
	}

	if( !testthing )
		return is;

	/*	No dobra, wykrywamy kolizje na �cianach i na rogach, ale co z innymi graczami/przeciwnikami?
	Tu jest troch� odmienna sprawa, ale te� da si� wyj�� z sytuacji. Najpierw sprawdzamy, kt�ry z
	przeciwnik�w jest na tyle blisko, �e mo�e pojawi� si� kolizja. Potem usyskujemy wektor normalny
	od przeciwnika do badanej kuk�y. Dobra mamy ju� wektor, ale �eby zbudowa� p�aszczyzne, jest jeszcze
	potrzebny punkt przeci�cia sfer. Mo�na to uzyska� mno��c wektor normalny przez promie� przeciwnika
	i dodaj�c to do jego po�orzenia. No �wietnie, mamy do�� informacji, by skonstruowa� p�aszczyzne.
	Kiedy ju� stwierdzimy wyst�pienie kolizji, to oddalamy kuk�e na odleg�o�� sumy promieni pomno�onej
	przez wektor normalny. I tyle :)
	*/
	//CActor* Thing;
	//for( i = 0; i < ThingManager.Count(); i++ )
	//{
	//	Thing = ThingManager.GetThing( i );
	//	// Po kiego maja sprawdza� kolizje na martwym przeciwniku??
	//	if( Thing->IsDead() )
	//		continue;
	//	// No chyba sam ze sob� nie mo�na si� zderzy�... :D
	//	if( Dum == Thing )
	//		continue;
	//	if( Dum != &MainPlayer && Thing->GetType() == ACTOR_TYPE::ACTOR_ENEMY )
	//		continue;
	//	// Sprawdzamy odleg�o�� ( ja jeszcze doda�em dwie jednostki, by sprawdzanie rozpocze�o si� wcze�niej )
	//	if( mathDistSq( Thing->Pos, Dum->NextPos ) > POW( Thing->Radius + Dum->Radius + 2.0f ) )
	//		continue;

	//	// Tworzymy p�aszczyzne i punkt przeci�cia.
	//	Normal = ( Dum->NextPos - Thing->NextPos ).Normalize();
	//	V = Thing->Pos + Normal * Thing->Radius;
	//	D = -V.Dot( Normal );

	//	// Aktualna pozycja
	//	A = Dum->Pos;// + Normal.Reverse() * R;
	//	// Nast�pna pozycja
	//	B = Dum->NextPos + Normal.Reverse() * Dum->Radius;

	//	// Powtarzamy poprzedni� regu�ke...
	//	wynik = ( Normal.Dot( A ) + D ) * ( Normal.Dot( B ) + D );
	//	if( wynik >= 0 ) 
	//		continue;
	//	V = A - B;
	//	W = ( Normal.Dot( B ) + D ) / Normal.Dot( V );
	//	P = B - V * W;

	//	is = true;
	//	// i tworzymy now� pozycj�.
	//	Dum->NextPos = P + Normal * Dum->Radius;
	//}

	return is;
}

/*	Ta funkcja wykorzystuje technik� zwan� Ray Casting
czyli wypuszczanie/puszczanie promieni. Polega ona
na tym, by z punktu pocz�tkowego w danym kierunku,
co okre�lony krok bada� kolizje, a� do znalezienia
punktu kolizji. Kolizje s� sprawdzane za pomoc�
poprzedniej funkcji.
*/
const Vector3f RayCast( const Vector3f& pos, const Vector3f& vector, const float step, const CLevel& level )
{
	if(vector.LeangthSq() == 0.0f)
		return pos;

	CDynamic Dum;
	Dum.Radius = 0.1f;
	Dum.NextPos = pos;
	CLvlBlock* Block = NULL;

	do
	{
		Dum.Pos = Dum.NextPos;
		Dum.NextPos = Dum.Pos + vector * step;

		Block = level.GetBlock( Dum.GetBlockPos() );
		/*	Takie ma�e zabezpieczenie.
		Kiedy promie� znajdzie si� poza poziomem
		( gdzie nie ma �adnych �cian ) to zamiast
		si� zawiesi�, to niech zwr�ci bezpieczny
		punkt �rodka (0, 0, 0).
		*/
		if( Block == NULL )
		{
			return Vector3f( 0.0f, 0.0f, 0.0f );
		}
	}
	while( !TestCollBlock( &Dum, Block ) );

	return Dum.NextPos;
}

/*	Ta funckja jest tak �atwa, �e a� �mieszna.
sprawdza ona odleg�o�� mi�dzy dwoma kuk�ami i
zwraca czy si� zde�y�y, czy nie.
*/
bool TestCollDum( CDynamic* Dum, CDynamic* Dum2 )
{
	Vector3f V1 = ClosestPoint( Dum2->Pos, Dum2->NextPos, Dum->NextPos );

	if( mathDistSq( V1, Dum->NextPos ) < POW(Dum->Radius) )
		return true;
	else return false;
}

/*	Tutaj co� lepszego - ta funckja sprawdza czy mi�dzy
dowama punktami nie ma przeszk�d. Wykorzystuje to m.in.
do sprawdzania widoczno�ci.
*/
bool IsCollOnRay( Vector3f V1, Vector3f V2, int Steps )
{
	Vector3f Veloc = V2 - V1;
	Vector3f Step = Veloc / (float)Steps;
	CDynamic Dum;
	Dum.Radius = 0.1f;
	Dum.NextPos = V1;
	CLvlBlock* Block = NULL;

	for( int i = 0; i < Steps; i++ )
	{
		Dum.Pos = Dum.NextPos;
		Dum.NextPos = Dum.Pos + Step;
		Block = pGLevel->GetBlock( Dum.GetBlockPos() );

		if( Block == NULL )
			return true;

		if( TestCollBlock( &Dum, Block ) )
			return true;
	}

	return false;
}