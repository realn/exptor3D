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
#include "WeaponBulletManager.h"

#include "ItemManager.h"
#include "ItemAmmo.h"
#include "ItemArmor.h"
#include "ItemHealth.h"

CLevel GLevel;
CLevel* pGLevel = &GLevel;

/*	KLASA gameWLFlags
S�u�y do sprawdzania warunk�w wygranej jak i przegranej.
w klasie level'u wyst�puj� dwa takei obiekty.
*/
// Sprawdzamy, czy wszyscy nie �yj� ( poza graczem (
bool gameWLFlags::IsAllDead()
{
	if( ThingManager.GetAllCount() == 0 )
		return false;

	if( ThingManager.GetLifeCount() == 0 )
		return true;
	else return false;

	return false;
}

// Sprawdzamy, czy gracz jest w odpowiednim bloku
bool gameWLFlags::IsInBlock()
{
	if( GLevel.GetBlock( MainPlayer.GetBlockPos() ) == Block )
		return true;
	else return false;

	return false;
}

// Sprawdzamy, czy gracz znalaz� odpowiedni� bro�
bool gameWLFlags::GotWeap()
{
	return MainPlayer.Weapon[WeapID]->GetHave();	
}

// Sprawdzamy, czy gracz nie �yje
bool gameWLFlags::IsSelfDead()
{
	return MainPlayer.IsDead();
}

// Sprawdzamy, czy okre�lony stw�r nie �yje
bool gameWLFlags::IsThingDead()
{
	if( Enemy == NULL )
		return false;

	return Enemy->IsDead();
}

// Inicjujemy flagi ( znajdujemy potwory, itp ).
void gameWLFlags::VerifyFlags()
{
	if( flags & WLFLAG_GO_TO_BLOCK )
	{
		Block = GLevel.GetBlock( BlockID );
	}
	if( flags & WLFLAG_THING_DEAD )
	{
		Enemy = ThingManager.GetEnemyByID( EnemyID );
	}
}

// Metoda sprawdza, czy WSZYSTKIE warunki s� prawdziwe
bool gameWLFlags::CheckAllFlags()
{
	bool status = true;
	if( flags == 0 )
		status = false;

	if( ( flags & WLFLAG_ALL_ENEM_DEAD ) )
		if( !IsAllDead() )
			status = false;
	if( ( flags & WLFLAG_GO_TO_BLOCK ) )
		if( !IsInBlock() )
			status = false;
	if( ( flags & WLFLAG_GET_WEAP ) )
		if( !GotWeap() )
			status = false;
	if( ( flags & WLFLAG_SELF_DEAD ) )
		if( !IsSelfDead() )
			status = false;
	if( ( flags & WLFLAG_THING_DEAD ) )
		if( !IsThingDead() )
			status = false;
	return status;
}

// Metoda sprawdza, czy chocia� jeden warunek jest prawdziwy
bool gameWLFlags::CheckOneFlag()
{
	bool status = false;
	if( ( flags & WLFLAG_ALL_ENEM_DEAD ) )
		if( IsAllDead() )
			status = true;
	if( ( flags & WLFLAG_GO_TO_BLOCK ) )
		if( IsInBlock() )
			status = true;
	if( ( flags & WLFLAG_GET_WEAP ) )
		if( GotWeap() )
			status = true;
	if( ( flags & WLFLAG_SELF_DEAD ) )
		if( IsSelfDead() )
			status = true;
	if( ( flags & WLFLAG_THING_DEAD ) )
		if( IsThingDead() )
			status = true;
	return status;
}


/*	KLASA CLevel
Patrz -> definicja klasy
*/
/*	KONSTRUKTOR	*/
CLevel::CLevel()
{
	// Ustawiamy, �e poziom nie by� jeszcze �adowany
	loaded = false;
	// Ustawiamy domy�ln� nazw� poziomu
	LevName = "UNKNOWN LEVEL";
	// Oraz domy�ln� nazw� pliku
	file = "-";
	// Ustawiamy, �e nie ma �adnych kolumn, czy wierszy
	rows = 0;
	cols = 0;
	// I zerujemy list� wy�wietlania
	Top = 0;
	Floor = 0;
	Wall = 0;
	// Zerujemy inne warto�ci
	WeapCount = 0;
	StatObjCount = 0;
	BonusCount = 0;
	EnemyCount = 0;
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

void	CLevel::Init( CTexManager& texManager, GLModelManager& modelManager )
{
	TexManager = &texManager;
	ModelManager = &modelManager;
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

// Wyci�ga z warto�� po znaku "="
std::string CLevel::GetParamStr( const std::string &str )
{
	int i;

	for( i = 0; i < str.length(); i++ )
	{
		if( str[i] == '=' )
		{
			i++;
			break;
		}
	}

	return str.substr( i, str.length() - i );
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
		ThingManager.Clear();
		WManager.Clear();
		BonusMan.Clear();
		MainPlayer.Reset();
		BManager.Clear();
	}

	Log.Log( "GLEVEL( " + file + " ): Wczytywanie poziomu: " + filename );
	GUI.SendConMsg( "Wczytywanie poziomu: " + filename, false );
	file = filename;

	// Teraz nast�pna linia MUSI byc rozpocz�ciem nag��wka, inaczej klops
	str = GetClearLine( stream );
	if( str != "HEADER" )
	{
		Log.Error( "GLEVEL( " + file + " ): Brak nag��wka, lub w nieodpowiednim miejscu!" );
		Free();
		return false;
	}
	else
	{
		do
		{
			str = GetLine( stream );
			if( ContainsString( str, "NAME" ) )
			{
				LevName = GetParamStr( str );
			}
			if( ContainsString( str, "ROWS" ) )
			{
				str = ClearWhiteSpace( str );
				if( !sscanf_s( str.c_str(), "ROWS=%u", &rows ) )
				{
					Log.Error( "GLEVEL( " + file + " ): Nie mo�na odczyta� liczby wierszy!" );
					Free();
					return false;
				}
			}
			if( ContainsString( str, "COLS" ) )
			{
				str = ClearWhiteSpace( str );
				if( !sscanf_s( str.c_str(), "COLS=%u", &cols ) )
				{
					Log.Error( "GLEVEL( " + file + " ): Nie mo�na odczyta� liczby kolumn!" );
					Free();
					return false;
				}
			}
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
			}
		}
		while( str != "END HEADER" );	
	}
	Log.Log( "GLEVEL( " + file + " ): Nazwa poziomu: " + LevName );

	/*	Dalej pobieramy dane o plikach
	tekstur do poziom�w.
	Pierwszy to �ciany, drugi sufit,
	trzeci pod�oga.
	*/
	str = GetLine( stream );
	if( str != "TEXTURES" )
	{
		Log.Error( "GLEVEL( " + file + " ): Brak listy tekstur!" );
		Free();
		return false;
	}
	else
	{
		str = GetLine( stream );
		if( !( Tex[0] = TexManager->Get( GetParamStr( str ) ) ) )
		{
			Log.Error( "GLEVEL( " + file + " ): B��dny plik graficzny!" );
			Free();
			return false;
		}
		str = GetLine( stream );
		if( !( Tex[1] = TexManager->Get( GetParamStr( str ) ) ) )
		{
			Log.Error( "GLEVEL( " + file + " ): B��dny plik graficzny!" );
			Free();
			return false;
		}
		str = GetLine( stream );
		if( !( Tex[2] = TexManager->Get( GetParamStr( str ) ) ) )
		{
			Log.Error( "GLEVEL( " + file + " ): B��dny plik graficzny!" );
			Free();
			return false;
		}

		str = GetLine( stream );
		if( str != "END TEXTURES" )
		{
			Log.Error( "GLEVEL( " + file + " ): Brak zako�czenia liczby tesktur!" );
			Free();
			return false;
		}
	}

	if(! LoadWalls( stream ) )
		return false;

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

			weWeapon* Weap;
			switch( k )
			{
			case GAME_WEAP_SAW :
				Weap = new weSaw;
				break;

			case GAME_WEAP_PISTOL :
				Weap = new wePistol;
				break;

			case GAME_WEAP_MINIPZR :
				Weap = new weMiniPhazer;
				break;

			case GAME_WEAP_MINIGUN :
				Weap = new weMiniGun;
				break;

			case GAME_WEAP_ROCKETLUN :
				Weap = new weRocketLuncher;
				break;

				//case GAME_WEAP_PICKABOO	:
			case GAME_WEAP_PHAZER :
				Weap = new wePhazer;
				break;

				//case GAME_WEAP_MINE :
			case GAME_WEAP_ATOM_BOMB :
				Weap = new weAtomBomb;
				break;
			default:
				continue;
			}
			Weap->Init( *ModelManager );
			Weap->Pos = GetBlockPos( j );
			Weap->NextPos = Weap->Pos;
			WManager.AddWeapon( *ModelManager, Weap );
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

			CEnemy* Enemy = new CEnemy( *ModelManager );
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

	if( BonusCount > 0 )
	{
		str = GetLine( stream );

		if( str != "BONUSLIST" )
		{
			Log.Error( "GLEVEL( " + file + " ): Brak listy bonus�w!" );
			Free();
			return false;
		}

		for( i = 0; i < BonusCount; i++ )
		{
			int a, b;
			str = GetLine( stream );
			std::string param = GetParamStr( str );

			sscanf_s( str.c_str(), "%d,%d/", &k, &l );
			CItem* Bonus;
			switch( l )
			{
			case BONUS_TYPE_AMMO :
				sscanf_s( str.c_str(), "%d,%d/%d,%d=", &k, &l, &a, &b );
				Bonus = new CItemAmmo( a, b, ModelManager->Get( param ) );
				break;

			case BONUS_TYPE_HEALTH :
				sscanf_s( str.c_str(), "%d,%d/%d=", &k, &l, &a );
				Bonus = new CItemHealth( a, ModelManager->Get( param ) );
				break;

			case BONUS_TYPE_ARMOR :
				sscanf_s( str.c_str(), "%d,%d/%d=", &k, &l, &a );
				Bonus = new CItemArmor( a, ModelManager->Get( param ) );
				break;

			case BONUS_TYPE_UNKNOWN :
			default:
				continue;
			}

			Bonus->Pos = GetBlockPos( k );
			BonusMan.AddBonus( Bonus );
		}

		str = GetLine( stream );
		if( str != "END BONUSLIST" )
		{
			Log.Error( "GLEVEL( " + file + " ): Brak ko�ca listy bonus�w!" );
			Free();
			return false;
		}
	}

	// Zamykamy plik
	str = GetLine( stream );
	if( str != "END E3DTLEV" )
	{
		Log.Error( "GLEVEL( " + file + " ): Brak ko�ca pliku!" );
	}

	MainPlayer.SetStartPos( this->GetBlockPos( PlayerStartBlock ) );
	MainPlayer.SetStartAngle( (float)PlayerStartAngle );
	MainPlayer.Reset();

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
	for( i = 0; i < rows; i++ )
	{
		for( j = 0; j < cols; j++ )
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
	for( i = 0; i < rows; i++ )
	{
		for( j = 0; j < cols; j++ )
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
	for( i = 0; i < rows; i++ )
	{
		for( j = 0; j < cols; j++ )
		{
			glPushMatrix();

			glTranslatef( j*10, 0.0f, -(i*10) );

			int in = i * cols + j;

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
	for( i = 0; i < rows; i++ )
	{
		for( j = 0; j < cols; j++ )
		{
			blk = &block[ i * cols + j ];

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
	for( i = 0; i < rows; i++ )
	{
		for( j = 0; j < cols; j++ )
		{
			blk = &block[ i * cols + j ];

			/*	Znajdujemy bloki s�siednie, by mo�na
			by�o znale�� rogi �cian.
			*/
			if( i < rows - 1 )
				SideBlock[0] = &block[ (i+1) * cols + j ];
			else
				SideBlock[0] = NULL;

			if( j < cols - 1 )
				SideBlock[1] = &block[ i * cols + (j+1) ];
			else
				SideBlock[1] = NULL;

			if( i > 0 )
				SideBlock[2] = &block[ (i-1) * cols + j ];
			else
				SideBlock[2] = NULL;

			if( j > 0 )
				SideBlock[3] = &block[ i * cols + (j-1) ];
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
		if( j < rows && i < cols )
		{
			return (CLvlBlock*)&block[ j * cols + i ];
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
	return rows*cols;
}

Vector3f CLevel::GetBlockPos( unsigned int i )
{
	return Vector3f( (float)(i % cols)*10.0f+5.0f, 0.0f, -(float)(i / cols)*10.0f-5.0f );
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

	rows = 0;
	cols = 0;
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

const bool	CLevel::LoadWalls( std::fstream& stream )
{
	/*	Teraz tworzymy tablic� odpowiednich
	rozmiar�w zdoln� pomie�ci� nast�pne
	dane. S� to liczby ca�kowite jednoznacznie
	okre�laj�ce zawarte w nich �ciany ( patrz
	definicje makrowe )
	*/
	std::string str = GetLine( stream );
	if( str != "WALLS" )
	{
		Log.Error( "GLEVEL( " + file + " ): Brak listy �cian!" );
		Free();
		return false;
	}
	block.resize( rows * cols );

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
		V = Dum->NextPos - Block->TCorner[i];
		// i go normalizujemy, by si� nadawa� do wyliczenia odleg�o�ci
		V.Normalize();
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
	CActor* Thing;
	for( i = 0; i < ThingManager.Count(); i++ )
	{
		Thing = ThingManager.GetThing( i );
		// Po kiego maja sprawdza� kolizje na martwym przeciwniku??
		if( Thing->IsDead() )
			continue;
		// No chyba sam ze sob� nie mo�na si� zderzy�... :D
		if( Dum == Thing )
			continue;
		if( Dum != &MainPlayer && Thing->GetType() == GAME_THING_ENEMY )
			continue;
		// Sprawdzamy odleg�o�� ( ja jeszcze doda�em dwie jednostki, by sprawdzanie rozpocze�o si� wcze�niej )
		if( mathDistSq( Thing->Pos, Dum->NextPos ) > POW( Thing->Radius + Dum->Radius + 2.0f ) )
			continue;

		// Tworzymy p�aszczyzne i punkt przeci�cia.
		Normal = Dum->NextPos - Thing->NextPos;
		Normal.Normalize();
		V = Thing->Pos + Normal * Thing->Radius;
		D = -V.Dot( Normal );

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
		Block = GLevel.GetBlock( Dum.GetBlockPos() );

		if( Block == NULL )
			return true;

		if( TestCollBlock( &Dum, Block ) )
			return true;
	}

	return false;
}