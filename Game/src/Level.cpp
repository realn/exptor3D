/*///////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
Plik:	Level.cpp
Autor:	Real_Noname (real_noname@wp.pl)
(C):	CODE RULERS (Real_Noname)
WWW:	www.coderulers.prv.pl
Opis:	Patrz -> Level.h

/////////////////////////////////////////////////////////
///////////////////////////////////////////////////////*/

#include <CBGL/COpenGL.h>

#include "Level.h"

#include "GamePlayer.h"

#include "ItemAmmo.h"
#include "ItemArmor.h"
#include "ItemHealth.h"
#include "ItemWeapon.h"

CLevel* pGLevel = nullptr;
const unsigned GAME_VERSION = 100;

/*	KLASA CLevel
Patrz -> definicja klasy
*/
/*	KONSTRUKTOR	*/
CLevel::CLevel( gfx::TextureRepository& texManager, gfx::ModelManager& modelManager ) 
	: core::Object(L"Level"),
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

	PlayerStartBlock = 0;
	PlayerStartAngle = 0;

	Tex[0] = nullptr;
	Tex[1] = nullptr;
	Tex[2] = nullptr;
}

/*	DESTRUKTOR	*/
CLevel::~CLevel()
{
	/*	Przy destrukcji wypada³o
	by zwolniæ pamiêæ
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
	RenderLevel();

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

// Zwraca, czy poziom jest za³adowany i gotowy
const bool CLevel::IsLoaded() const
{
	return loaded;
}

/*	METODA WCZYTUJ¥CA DANE
Za³o¿enia pliku s¹ proste.
Na pocz¹tku podstawowe dane, 
a dalej jedziemy ze œcianami
i ca³¹ reszt¹ danych.
*/
const bool CLevel::LoadLevel( const std::string &filename )
{
	if( filename == "" )
	{
		error( L"£añcuch znaków jest pusty!" );
		return false;
	}

	// Najpierw definiujemy wskaŸnik na plik i go otwieramy
	std::fstream stream( filename, std::ios::in );

	if( !stream )
	{
		error( L"Plik nie istnieje, lub podana œcie¿ka jest b³êdna: " + cb::fromUtf8(filename) );
		return false;
	}

	// definiujemy zmienne pomocnicze
	std::string str;

	// Pobieramy wersje poziomu oraz j¹ sprawdzamy
	str = GetClearLine( stream );
	if( !sscanf_s( str.c_str(), "E3DTLEV=%u", &Version ) )
	{
		error( L"Nieprawid³owy plik poziomu!" );
		return false;
	}

	if( Version > GAME_VERSION )
	{
		error( L"Zbyt wysoka wersja pliku!" );
		return false;
	}

	/*	Wczeœnijsze if'y by³y, by uzyskaæ pewnoœæ,
	¿e wszystko z nowym plikiem jest w porz¹dku.
	Dlaczego tyle zachodu? By unikn¹æ sytuacji,
	kiedy ³adujemy plik, na ju¿ istniej¹cy level.
	Level istnieje, wiêc go kasujemy i czytamy plik.
	A teraz Zonk! Nie ma pliku! I co? zostajemy na
	lodzie bez levelu :). Teraz wiecie o co chodzi :)
	*/
	if( loaded )
	{
		report( L"Prze³adowanie poziomu na : " + cb::fromUtf8(filename) );
		Free();
		Player.Reset();
	}

	log( L"Wczytywanie poziomu: " + cb::fromUtf8(filename) );
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
				error( L"B³¹d odczytu nag³ówka!" );
				continue;
			}
		}
		else if( str == "TEXTURES" )
		{
			if( !LoadTextures( stream ) )
			{
				error( L"B³¹d odczytu tekstur!" );
				continue;
			}
		}
		else if( str == "WALLS" )
		{
			if( !LoadWalls( stream ) )
			{
				error( L"B³¹d odczytu œcian!" );
				continue;
			}
		}
		else if( str == "ITEMLIST" )
		{
			if( !LoadItemList( stream ) )
			{
				error( L"B³¹d odczytu listy przedmiotów!" );
				continue;
			}
		}
		else
			error( L"Nierozpoznany ci¹g " + cb::fromUtf8(str) + L"!" );
	}

	for( unsigned row = 0; row < Rows; row++ )
	{
		for( unsigned col = 0; col < Cols; col++ )
			GetBlock( col, row )->Set( row, col, blockWidth, blockHeight, blockDepth );
	}

	Player.SetStartPos( this->GetBlockPos( PlayerStartBlock ) );
	Player.SetStartAngle( (float)PlayerStartAngle );
	Player.Reset();

	this->AddEntity( &Player );

	loaded = true;

	BuildVisual();
	BuildPhysic();

	return true;
}

/*	W tej metodzie budujemy czêœæ wizualn¹
poziomu, oddzielnie od reszty logiki. W programowaniu gier jest ZAWSZE
zasada odzielania tych czêœci.
*/
void CLevel::BuildVisual()
{
	if( !loaded )
		return;

	//GUI.SendConMsg( "Poziom: Tworzenie czesci wizualnej...", false );
	// Zmienne pomocnicze

	/*	Teraz tworzymy listê. Listy wyœwietlania
	maj¹ te zalete, ¿e raz stworzone BARDZO
	przyspieszaj¹ wyœwietlanie, dlatego op³aca
	siê je stosowaæ. W zamian nie mo¿na nic zmieniæ
	w liœcie, no chyba, ¿e tworz¹c j¹ od nowa.
	*/
	/*	Ta czêœæ kodu tysuje pod³oge, na ca³ej
	powieszchni poziomu.
	*/

	std::vector<glm::vec2> texCoord;
	texCoord.push_back( glm::vec2( 0.0f, 0.0f ) );
	texCoord.push_back( glm::vec2( 1.0f, 0.0f ) );
	texCoord.push_back( glm::vec2( 1.0f, 1.0f ) );
	texCoord.push_back( glm::vec2( 0.0f, 1.0f ) );

	for( unsigned row = 0; row < Rows; row++ )
	{
		for( unsigned col = 0; col < Cols; col++ )
		{
			auto pBlock = GetBlock( col, row );
			if( pBlock == nullptr )
				continue;

			std::vector<glm::vec3> vertFloor;
			std::vector<glm::vec3> vertCeiling;
			std::vector<glm::vec3> vertWall;

			GenSurfaceVerts( (LEV_SURFACE)(unsigned)( pBlock->walls & ( (unsigned)LEV_SURFACE::FLOOR) ), vertFloor );
			GenSurfaceVerts( (LEV_SURFACE)(unsigned)( pBlock->walls & ( (unsigned)LEV_SURFACE::CEILING) ), vertCeiling );
			GenSurfaceVerts( (LEV_SURFACE)(unsigned)( pBlock->walls & ( (unsigned)LEV_SURFACE::WALL_FAR) ), vertWall );
			GenSurfaceVerts( (LEV_SURFACE)(unsigned)( pBlock->walls & ( (unsigned)LEV_SURFACE::WALL_LEFT) ), vertWall );
			GenSurfaceVerts( (LEV_SURFACE)(unsigned)( pBlock->walls & ( (unsigned)LEV_SURFACE::WALL_NEAR) ), vertWall );
			GenSurfaceVerts( (LEV_SURFACE)(unsigned)( pBlock->walls & ( (unsigned)LEV_SURFACE::WALL_RIGHT) ), vertWall );

			for( unsigned i = 0; i < vertFloor.size(); i++ )
				Floor.AddVertex( (pBlock->Origin + vertFloor[i]) * glm::vec3( 1.0f, 1.0f, -1.0f ), texCoord[i] );

			for( unsigned i = 0; i < vertCeiling.size(); i++ )
				Ceiling.AddVertex( (pBlock->Origin + vertCeiling[i]) * glm::vec3( 1.0f, 1.0f, -1.0f ), texCoord[i] );

			for( unsigned i = 0; i < vertWall.size(); i++ )
				Wall.AddVertex( (pBlock->Origin + vertWall[i]) * glm::vec3( 1.0f, 1.0f, -1.0f ), texCoord[ i % 4 ] );
		}
	}
}

/*	Ta metoda jest odpowiedzilna, za czêœæ fizyczn¹ poziomu
buduje, m.in. punkty i p³aszczyzny, w oparciu o które
bêd¹ wykrywane kolizje.
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

			std::vector<glm::vec3> verts;

			GenSurfaceVerts( (LEV_SURFACE)(unsigned)( pBlock->walls & ( (unsigned)LEV_SURFACE::FLOOR) ), verts );
			GenSurfaceVerts( (LEV_SURFACE)(unsigned)( pBlock->walls & ( (unsigned)LEV_SURFACE::CEILING) ), verts );
			GenSurfaceVerts( (LEV_SURFACE)(unsigned)( pBlock->walls & ( (unsigned)LEV_SURFACE::WALL_FAR) ), verts );
			GenSurfaceVerts( (LEV_SURFACE)(unsigned)( pBlock->walls & ( (unsigned)LEV_SURFACE::WALL_LEFT) ), verts );
			GenSurfaceVerts( (LEV_SURFACE)(unsigned)( pBlock->walls & ( (unsigned)LEV_SURFACE::WALL_NEAR) ), verts );
			GenSurfaceVerts( (LEV_SURFACE)(unsigned)( pBlock->walls & ( (unsigned)LEV_SURFACE::WALL_RIGHT) ), verts );

			for( size_t faceIndex = 0; faceIndex < verts.size() / 4; faceIndex++ )
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

/*	Tu odbywa siê rysowanie levelu. 
( takie proste, ¿e a¿ g³upie :) )
*/
void	CLevel::RenderLevel()
{
	if( !loaded )
		return;

	this->RenderCeilings();
	this->RenderWalls();
	this->RenderFloors();
}

void	CLevel::RenderCeilings()
{
	if( !loaded )
		return;

	Tex[1]->Activate();
	Ceiling.Render();
}

void	CLevel::RenderWalls()
{
	if( !loaded )
		return;

	Tex[0]->Activate();
	Wall.Render();
}

void	CLevel::RenderFloors()
{
	if( !loaded )
		return;

	Tex[2]->Activate();
	Floor.Render();
}

/*	Ta metoda zwraca nazwê poziomu wyczytan¹ z pliku
(lub domyœln¹).
*/
const std::string CLevel::GetName() const
{
	return LevName;
}

/*	Ta zwraca dany block z okreœlonego wiersza i kolumny
*/
CLvlBlock* CLevel::GetBlock( const unsigned col, const unsigned row ) const
{
	if( col >= Cols || row >= Rows )
		return nullptr;

	return const_cast<CLvlBlock*>(&block[ static_cast<size_t>(row) * Cols + col ]);
}

CLvlBlock* CLevel::GetBlock( glm::vec3 Pos ) const
{
	return GetBlock( (unsigned int)Pos.x, (unsigned int)Pos.z );
}

CLvlBlock* CLevel::GetBlock( unsigned int i ) const
{
	return (CLvlBlock*)&block[i];
}

const unsigned	CLevel::GetBlockCount() const
{
	return Rows * Cols;
}

const glm::vec3 CLevel::GetBlockPos( const unsigned i ) const
{
	return GetBlockPos( i % Cols, i / Cols );
}

const glm::vec3	CLevel::GetBlockPos( const int x, const int y ) const
{
	float halfW = blockWidth / 2.0f;
	float halfD = blockDepth / 2.0f;

	return glm::vec3( (float)(x) * blockWidth + halfW, 0.0f, (float)(y) * blockDepth + halfD );
}

const unsigned	CLevel::GetBlockIndex( const unsigned col, const unsigned row ) const
{
	if( col >= Cols || row >= Rows )
		return 0;

	return row * Cols + col;
}

/*	Metoda czyœci pamiêc zarezerwowan¹ dla
tej klasy.
*/
void CLevel::Free()
{
	if( !loaded )
		return;

	block.clear();

	Rows = 0;
	Cols = 0;

	LevName = "UNKNOWN LEVEL";

	Floor.Clear();
	Ceiling.Clear();
	Wall.Clear();

	Tex[0] = nullptr;
	Tex[1] = nullptr;
	Tex[2] = nullptr;

	ClearScene();

	for( unsigned i = 0; i < Items.size(); i++ )
		delete (Items[i]);
	Items.clear();

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
	log( L"£adowanie nag³ówka.");

	std::string str, name, value;
	while( stream )
	{
		str = GetLine( stream );

		if( str == "END HEADER" )
			return true;

		if( !ParseNameValue( str, name, value ) )
		{
			error( L"Nieprawid³owy ci¹g w nag³ówku: " + cb::fromUtf8(str) + L"." );
			continue;
		}

		if( name == "NAME" )
			LevName = value;
		else if( name == "ROWS" )
			Rows = StrToUInt( value );
		else if( name == "COLS" )
			Cols = StrToUInt( value );
		else 
			error( L"Nierozpoznana wartoœæ " + cb::fromUtf8(name) + L" w ci¹gu: " + cb::fromUtf8(str) + L"." );
	}

	error( L"Brak koñca nag³ówka." );
	return false;
}

const bool	CLevel::LoadTextures( std::fstream& stream )
{
	log( L"£adowanie tekstur.");

	std::string str, name, value;
	while( stream )
	{
		str = GetLine( stream );

		if( str == "END TEXTURES" )
			return true;

		if( !ParseNameValue( str, name, value ) )
		{
			error( L"Nieprawid³owy ci¹g w liœcie tekstur: " + cb::fromUtf8(str) + L"." );
			continue;
		}
		else if( name == "WALL" )
		{
			if( !( Tex[0] = TexManager.Get( value ) ) )
			{
				error( L"Nie uda³o siê za³adowaæ tekstury œcian!" );
				continue;
			}
		}
		else if( name == "CEILING" )
		{
			if( !( Tex[1] = TexManager.Get( value ) ) )
			{
				error( L"Nie uda³o siê za³adowaæ tekstury sufitu!" );
				continue;
			}
		}
		else if( name == "FLOOR" )
		{
			if( !( Tex[2] = TexManager.Get( value ) ) )
			{
				error( L"Nie uda³o siê za³adowaæ tekstury pod³ogi!" );
				continue;
			}
		}
	}

	error( L"Brak zakoñczenia liczby tesktur!" );
	return false;
}

const bool	CLevel::LoadWalls( std::fstream& stream )
{
	log( L"£adowanie œcian.");

	std::string str;
	block.resize( static_cast<size_t>(Rows) * Cols );
	for( size_t i = 0; i < block.size(); i++ )
	{
		str = GetClearLine( stream );

		if(ContainsString( str, "," ))
		{
			std::vector<std::string> list;
			SplitString( str, ",", list );

			for(size_t j = 0; j < list.size() && i + j < block.size(); j++ )
			{
				block[i + j].LoadWalls( list[j] );
				block[i + j].CornerCount = 0;
			}
			if( list.size() > 0 )
				i += list.size() - 1;
		}
		else
		{
			block[i].LoadWalls( str );
			block[i].CornerCount = 0;
		}
	}

	str = GetLine( stream );
	if( str != "END WALLS" )
	{
		error( L"Brak koñca listy œcian!" );
		Free();
		return false;
	}

	return true;
}

const bool	CLevel::LoadItemList( std::fstream& stream )
{
	log( L"£adowanie listy przedmiotów.");

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
				error(L"B³¹d parsowania przedmiotu, nieznany typ z parsowania ci¹gu: " + cb::fromUtf8(str) + L".");
				break;
			}

			if(item != nullptr)
			{
				item->Pos = GetBlockPos( x, y );
				AddEntity( item );
				Items.push_back( item );
			}
		}
		else
		{
			error(L"B³¹d parsowania przedmiotu dla ci¹gu: " + cb::fromUtf8(str) + L".");
		}
	}

	error( L"Brak koñca listy przedmiotów!" );
	return false;
}

void	CLevel::GenSurfaceVerts( const LEV_SURFACE surf, std::vector<glm::vec3>& verts )
{
	float halfX = blockWidth / 2.0f;
	float halfY = blockHeight / 2.0f;
	float halfZ = blockDepth / 2.0f;

	switch (surf)
	{
	case LEV_SURFACE::CEILING:
		verts.push_back( glm::vec3( -halfX, halfY, halfZ ) );
		verts.push_back( glm::vec3( halfX, halfY, halfZ ) );
		verts.push_back( glm::vec3( halfX, halfY, -halfZ ) );
		verts.push_back( glm::vec3( -halfX, halfY, -halfZ ) );
		break;

	case LEV_SURFACE::FLOOR:
		verts.push_back( glm::vec3( -halfX, -halfY, -halfZ ) );
		verts.push_back( glm::vec3( halfX, -halfY, -halfZ ) );
		verts.push_back( glm::vec3( halfX, -halfY, halfZ ) );
		verts.push_back( glm::vec3( -halfX, -halfY, halfZ ) );
		break;

	case LEV_SURFACE::WALL_FAR:
		verts.push_back( glm::vec3( -halfX, -halfY, halfZ ) );
		verts.push_back( glm::vec3( halfX, -halfY, halfZ ) );
		verts.push_back( glm::vec3( halfX, halfY, halfZ ) );
		verts.push_back( glm::vec3( -halfX, halfY, halfZ ) );
		break;

	case LEV_SURFACE::WALL_NEAR:
		verts.push_back( glm::vec3( halfX, -halfY, -halfZ ) );
		verts.push_back( glm::vec3( -halfX, -halfY, -halfZ ) );
		verts.push_back( glm::vec3( -halfX, halfY, -halfZ ) );
		verts.push_back( glm::vec3( halfX, halfY, -halfZ ) );
		break;

	case LEV_SURFACE::WALL_LEFT:
		verts.push_back( glm::vec3( -halfX, -halfY, -halfZ ) );
		verts.push_back( glm::vec3( -halfX, -halfY, halfZ ) );
		verts.push_back( glm::vec3( -halfX, halfY, halfZ ) );
		verts.push_back( glm::vec3( -halfX, halfY, -halfZ ) );
		break;

	case LEV_SURFACE::WALL_RIGHT:
		verts.push_back( glm::vec3( halfX, -halfY, halfZ ) );
		verts.push_back( glm::vec3( halfX, -halfY, -halfZ ) );
		verts.push_back( glm::vec3( halfX, halfY, -halfZ ) );
		verts.push_back( glm::vec3( halfX, halfY, halfZ ) );
		break;

	default:
		break;
	}
}