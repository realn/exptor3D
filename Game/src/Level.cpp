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
	Player( modelManager ),
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

const CCollisionManager&	CLevel::GetCollisionManager() const
{
	return CollisionMng;
}

CPlayer&	CLevel::GetPlayer()
{
	return Player;
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
		Player.Reset();
	}

	Log.Log( "GLEVEL( " + file + " ): Wczytywanie poziomu: " + filename );
	//GUI.SendConMsg( "Wczytywanie poziomu: " + filename, false );
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

	for( unsigned row = 0; row < Rows; row++ )
	{
		for( unsigned col = 0; col < Cols; col++ )
		{
			GetBlock( col, row )->Set( row, col, blockWidth, blockHeight, blockDepth );
		}
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

	Player.SetStartPos( this->GetBlockPos( PlayerStartBlock ) );
	Player.SetStartAngle( (float)PlayerStartAngle );
	Player.Reset();

	this->AddEntity( &Player );

	loaded = true;

	return true;
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

	//GUI.SendConMsg( "Poziom: Tworzenie czesci wizualnej...", false );
	// Zmienne pomocnicze

	/*	Teraz tworzymy list�. Listy wy�wietlania
	maj� te zalete, �e raz stworzone BARDZO
	przyspieszaj� wy�wietlanie, dlatego op�aca
	si� je stosowa�. W zamian nie mo�na nic zmieni�
	w li�cie, no chyba, �e tworz�c j� od nowa.
	*/
	/*	Ta cz�� kodu tysuje pod�oge, na ca�ej
	powieszchni poziomu.
	*/

	std::vector<Vector2f> texCoord;
	texCoord.push_back( Vector2f( 0.0f, 0.0f ) );
	texCoord.push_back( Vector2f( 1.0f, 0.0f ) );
	texCoord.push_back( Vector2f( 1.0f, 1.0f ) );
	texCoord.push_back( Vector2f( 0.0f, 1.0f ) );

	for( unsigned row = 0; row < Rows; row++ )
	{
		for( unsigned col = 0; col < Cols; col++ )
		{
			auto pBlock = GetBlock( col, row );
			if( pBlock == nullptr )
				continue;

			std::vector<Vector3f> vertFloor;
			std::vector<Vector3f> vertCeiling;
			std::vector<Vector3f> vertWall;

			GenSurfaceVerts( (LEV_SURFACE)(unsigned)( pBlock->walls & ( (unsigned)LEV_SURFACE::FLOOR) ), vertFloor );
			GenSurfaceVerts( (LEV_SURFACE)(unsigned)( pBlock->walls & ( (unsigned)LEV_SURFACE::CEILING) ), vertCeiling );
			GenSurfaceVerts( (LEV_SURFACE)(unsigned)( pBlock->walls & ( (unsigned)LEV_SURFACE::WALL_FAR) ), vertWall );
			GenSurfaceVerts( (LEV_SURFACE)(unsigned)( pBlock->walls & ( (unsigned)LEV_SURFACE::WALL_LEFT) ), vertWall );
			GenSurfaceVerts( (LEV_SURFACE)(unsigned)( pBlock->walls & ( (unsigned)LEV_SURFACE::WALL_NEAR) ), vertWall );
			GenSurfaceVerts( (LEV_SURFACE)(unsigned)( pBlock->walls & ( (unsigned)LEV_SURFACE::WALL_RIGHT) ), vertWall );

			for( unsigned i = 0; i < vertFloor.size(); i++ )
				Floor.AddVertex( (pBlock->Origin + vertFloor[i]) * Vector3f( 1.0f, 1.0f, -1.0f ), texCoord[i] );

			for( unsigned i = 0; i < vertCeiling.size(); i++ )
				Ceiling.AddVertex( (pBlock->Origin + vertCeiling[i]) * Vector3f( 1.0f, 1.0f, -1.0f ), texCoord[i] );

			for( unsigned i = 0; i < vertWall.size(); i++ )
				Wall.AddVertex( (pBlock->Origin + vertWall[i]) * Vector3f( 1.0f, 1.0f, -1.0f ), texCoord[ i % 4 ] );
		}
	}
}

/*	Ta metoda jest odpowiedzilna, za cz�� fizyczn� poziomu
buduje, m.in. punkty i p�aszczyzny, w oparciu o kt�re
b�d� wykrywane kolizje.
*/
void CLevel::BuildPhysic()
{
	if( !loaded )
		return;

	//GUI.SendConMsg( "Poziom: Tworzenie czesci fizycznej...", false );

	CollisionMng.SetBlockSize( blockWidth, blockDepth );
	CollisionMng.SetLevelSize( Rows, Cols );
	for( unsigned row = 0; row < Rows; row++ )
	{
		for( unsigned col = 0; col < Cols; col++ )
		{
			auto pBlock = this->GetBlock( col, row );

			CCollisionBlock block( GetBlockIndex( col, row ) );

			std::vector<Vector3f> verts;

			GenSurfaceVerts( (LEV_SURFACE)(unsigned)( pBlock->walls & ( (unsigned)LEV_SURFACE::FLOOR) ), verts );
			GenSurfaceVerts( (LEV_SURFACE)(unsigned)( pBlock->walls & ( (unsigned)LEV_SURFACE::CEILING) ), verts );
			GenSurfaceVerts( (LEV_SURFACE)(unsigned)( pBlock->walls & ( (unsigned)LEV_SURFACE::WALL_FAR) ), verts );
			GenSurfaceVerts( (LEV_SURFACE)(unsigned)( pBlock->walls & ( (unsigned)LEV_SURFACE::WALL_LEFT) ), verts );
			GenSurfaceVerts( (LEV_SURFACE)(unsigned)( pBlock->walls & ( (unsigned)LEV_SURFACE::WALL_NEAR) ), verts );
			GenSurfaceVerts( (LEV_SURFACE)(unsigned)( pBlock->walls & ( (unsigned)LEV_SURFACE::WALL_RIGHT) ), verts );

			for( unsigned faceIndex = 0; faceIndex < verts.size() / 4; faceIndex++ )
			{
				auto v1 = verts[ faceIndex * 4 + 0 ] + pBlock->Origin;
				auto v2 = verts[ faceIndex * 4 + 1 ] + pBlock->Origin;
				auto v3 = verts[ faceIndex * 4 + 2 ] + pBlock->Origin;
				auto v4 = verts[ faceIndex * 4 + 3 ] + pBlock->Origin;

				block.AddFace( v1, v2, v3, v4 );
			}

			CollisionMng.AddBlock( block );
		}
	}
	CollisionMng.FindSideBlocks();
}

/*	Tu odbywa si� rysowanie levelu. 
( takie proste, �e a� g�upie :) )
*/
void CLevel::DrawLevel()
{
	if( !loaded )
		return;

	//if( GUI.GetReflection() )
	//{
	//	this->DrawAllTop();
	//	this->DrawAllWall();

	//	glPushMatrix();
	//	glTranslatef( 0.0f, -10.0f, 0.0f );
	//	glScalef( 1.0f, -1.0f, 1.0f );
	//	//glDisable( GL_CLIP_PLANE0 );
	//	//glEnable( GL_CLIP_PLANE1 );
	//	glFrontFace( GL_CW );
	//	this->DrawAllTop();
	//	this->DrawAllWall();
	//	glFrontFace( GL_CCW );
	//	//glDisable( GL_CLIP_PLANE1 );
	//	//glEnable( GL_CLIP_PLANE0 );
	//	glPopMatrix();
	//}
	//else
	//{
		this->DrawAllTop();
		this->DrawAllWall();
		this->DrawAllFloor();
	//}
}

void CLevel::DrawAllTop()
{
	if( !loaded )
		return;

	Tex[1]->Activate();
	Ceiling.Render();
}

void CLevel::DrawAllWall()
{
	if( !loaded )
		return;

	Tex[0]->Activate();
	Wall.Render();
}

void CLevel::DrawAllFloor()
{
	if( !loaded )
		return;

	Tex[2]->Activate();
	Floor.Render();
}

void CLevel::DrawReflect()
{
	//if( !GUI.GetReflection() )
	//	return;

	//glPushMatrix();
	//glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	//glColor4f( 1.0f, 1.0f, 1.0f, GUI.GetRefLevel() );
	//glEnable( GL_BLEND );
	//this->DrawAllFloor();
	//glColor4f( 1.0f, 1.0f, 1.0f, 1.0f );
	//glDisable( GL_BLEND );
	//glPopMatrix();
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
CLvlBlock* CLevel::GetBlock( const unsigned col, const unsigned row ) const
{
	if( col >= Cols || row >= Rows )
		return nullptr;

	return const_cast<CLvlBlock*>(&block[ row * Cols + col ]);
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

	return Vector3f( (float)(x) * blockWidth + halfW, 0.0f, (float)(y) * blockDepth + halfD );
}

const unsigned	CLevel::GetBlockIndex( const unsigned col, const unsigned row ) const
{
	if( col >= Cols || row >= Rows )
		return 0;

	return row * Cols + col;
}

void CLevel::CheckWLFlags()
{
	//if( !this->loaded )
	//	return;
	//if( GUI.IsShowingWLScr() )
	//	return;

	//if( AllWin )
	//{
	//	if( this->WinFlags.CheckAllFlags() )
	//	{
	//		GUI.ShowWinScr();
	//	}
	//}
	//else if( this->WinFlags.CheckOneFlag() )
	//{
	//	GUI.ShowWinScr();
	//}

	//if( AllLose )
	//{
	//	if( this->LoseFlags.CheckAllFlags() )
	//		GUI.ShowLoseScr();
	//}
	//else if( this->LoseFlags.CheckOneFlag() )
	//{
	//	GUI.ShowLoseScr();
	//}	
}


/*	Metoda czy�ci pami�c zarezerwowan� dla
tej klasy.
*/
void CLevel::Free()
{
	if( !loaded )
		return;

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

	//glDeleteLists( Top, 1 );
	Floor.Clear();
	Ceiling.Clear();
	Wall.Clear();

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

void	CLevel::GenSurfaceVerts( const LEV_SURFACE surf, std::vector<Vector3f>& verts )
{
	float halfX = blockWidth / 2.0f;
	float halfY = blockHeight / 2.0f;
	float halfZ = blockDepth / 2.0f;

	switch (surf)
	{
	case LEV_SURFACE::CEILING:
		verts.push_back( Vector3f( -halfX, halfY, halfZ ) );
		verts.push_back( Vector3f( halfX, halfY, halfZ ) );
		verts.push_back( Vector3f( halfX, halfY, -halfZ ) );
		verts.push_back( Vector3f( -halfX, halfY, -halfZ ) );
		break;

	case LEV_SURFACE::FLOOR:
		verts.push_back( Vector3f( -halfX, -halfY, -halfZ ) );
		verts.push_back( Vector3f( halfX, -halfY, -halfZ ) );
		verts.push_back( Vector3f( halfX, -halfY, halfZ ) );
		verts.push_back( Vector3f( -halfX, -halfY, halfZ ) );
		break;

	case LEV_SURFACE::WALL_FAR:
		verts.push_back( Vector3f( -halfX, -halfY, halfZ ) );
		verts.push_back( Vector3f( halfX, -halfY, halfZ ) );
		verts.push_back( Vector3f( halfX, halfY, halfZ ) );
		verts.push_back( Vector3f( -halfX, halfY, halfZ ) );
		break;

	case LEV_SURFACE::WALL_NEAR:
		verts.push_back( Vector3f( halfX, -halfY, -halfZ ) );
		verts.push_back( Vector3f( -halfX, -halfY, -halfZ ) );
		verts.push_back( Vector3f( -halfX, halfY, -halfZ ) );
		verts.push_back( Vector3f( halfX, halfY, -halfZ ) );
		break;

	case LEV_SURFACE::WALL_LEFT:
		verts.push_back( Vector3f( -halfX, -halfY, -halfZ ) );
		verts.push_back( Vector3f( -halfX, -halfY, halfZ ) );
		verts.push_back( Vector3f( -halfX, halfY, halfZ ) );
		verts.push_back( Vector3f( -halfX, halfY, -halfZ ) );
		break;

	case LEV_SURFACE::WALL_RIGHT:
		verts.push_back( Vector3f( halfX, -halfY, halfZ ) );
		verts.push_back( Vector3f( halfX, -halfY, -halfZ ) );
		verts.push_back( Vector3f( halfX, halfY, -halfZ ) );
		verts.push_back( Vector3f( halfX, halfY, halfZ ) );
		break;

	default:
		break;
	}
}