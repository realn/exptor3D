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

gameLevel GLevel;
gameLevel* pGLevel = &GLevel;

/*	KLASA gameWLFlags
	S³u¿y do sprawdzania warunków wygranej jak i przegranej.
	w klasie level'u wystêpuj¹ dwa takei obiekty.
*/
// Sprawdzamy, czy wszyscy nie ¿yj¹ ( poza graczem (
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

// Sprawdzamy, czy gracz znalaz³ odpowiedni¹ broñ
bool gameWLFlags::GotWeap()
{
	return MainPlayer.Weapon[WeapID]->GetHave();	
}

// Sprawdzamy, czy gracz nie ¿yje
bool gameWLFlags::IsSelfDead()
{
	return MainPlayer.IsDead();
}

// Sprawdzamy, czy okreœlony stwór nie ¿yje
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

// Metoda sprawdza, czy WSZYSTKIE warunki s¹ prawdziwe
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

// Metoda sprawdza, czy chocia¿ jeden warunek jest prawdziwy
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


/*	KLASA gameLevel
	Patrz -> definicja klasy
*/
/*	KONSTRUKTOR	*/
gameLevel::gameLevel()
{
	// Zerujemy wskaŸnik (który bêdzie do tablicy)
	block = NULL;
	// Ustawiamy, ¿e poziom nie by³ jeszcze ³adowany
	loaded = false;
	// Ustawiamy domyœln¹ nazwê poziomu
	LevName = "UNKNOWN LEVEL";
	// Oraz domyœln¹ nazwê pliku
	file = "-";
	// Ustawiamy, ¿e nie ma ¿adnych kolumn, czy wierszy
	rows = 0;
	cols = 0;
	// I zerujemy listê wyœwietlania
	Top = 0;
	Floor = 0;
	Wall = 0;
	// Zerujemy inne wartoœci
	WeapCount = 0;
	StatObjCount = 0;
	BonusCount = 0;
	EnemyCount = 0;
	// Zerujemy liste typów wroga
	EnemyType = NULL;

	Tex[0] = NULL;
	Tex[1] = NULL;
	Tex[2] = NULL;
}

/*	DESTRUKTOR	*/
gameLevel::~gameLevel()
{
	/*	Przy destrukcji wypada³o
		by zwolniæ pamiêæ
	*/
	Free();
}

// Zwraca, czy poziom jest za³adowany i gotowy
bool gameLevel::GetLoaded()
{
	return loaded;
}

// Zwraca iloœæ przeciwników odczytanych z pliku
unsigned int gameLevel::GetEnemyCount()
{
	return EnemyCount;
}

// Zwraca iloœæ obiektów statycznych odczytanych z pliku
unsigned int gameLevel::GetSObjCount()
{
	return StatObjCount;
}

// Zwraca Liniê z pliku
std::string gameLevel::GetString( FILE* fp )
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

// Wyci¹ga z wartoœæ po znaku "="
std::string gameLevel::GetParamStr( const std::string &str )
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

/*	METODA WCZYTUJ¥CA DANE
	Za³o¿enia pliku s¹ proste.
	Na pocz¹tku podstawowe dane, 
	a dalej jedziemy ze œcianami
	i ca³¹ reszt¹ danych.
*/
bool gameLevel::LoadLevel( const std::string &filename )
{
	if( filename == "" )
	{
		Log.Error( "GLEVEL( " + file + " ): £añcuch znaków jest pusty!" );
		return false;
	}

	// Najpierw definiujemy wskaŸnik na plik i go otwieramy
	FILE* fp = 0;
	fopen_s(&fp, filename.c_str(), "rt" );

	if( !fp )
	{
		Log.Error( "GLEVEL( " + file + " ): Plik nie istnieje, lub podana œcie¿ka jest b³êdna: " + filename );
		return false;
	}

	// definiujemy zmienne pomocnicze
	std::string str;
	int i, j, k, l;

	// Pobieramy wersje poziomu oraz j¹ sprawdzamy
	str = GetString( fp );
	if( !sscanf_s( str.c_str(), "E3DTLEV=%u", &Version ) )
	{
		Log.Error( "GLEVEL( " + file + " ): Nieprawid³owy plik poziomu!" );
		fclose( fp );
		return false;
	}

	if( Version > GAME_VERSION )
	{
		Log.Error( "GLEVEL( " + file + " ): Zbyt wysoka wersja pliku!" );
		fclose( fp );
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
		Log.Report( "GLEVEL( " + file + " ): Prze³adowanie poziomu na : " + filename );
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
	
	// Teraz nastêpna linia MUSI byc rozpoczêciem nag³ówka, inaczej klops
	str = GetString( fp );
	if( str != "HEADER" )
	{
		Log.Error( "GLEVEL( " + file + " ): Brak nag³ówka, lub w nieodpowiednim miejscu!" );
		Free();
		fclose( fp );
		return false;
	}
	else
	{
		do
		{
			str = GetString( fp );
			if( guiIsInStr( str, "NAME" ) )
			{
				LevName = GetParamStr( str );
			}
			if( guiIsInStr( str, "ROWS" ) )
			{
				if( !sscanf_s( str.c_str(), "ROWS=%u", &rows ) )
				{
					Log.Error( "GLEVEL( " + file + " ): Nie mo¿na odczytaæ liczby wierszy!" );
					Free();
					fclose( fp );
					return false;
				}
			}
			if( guiIsInStr( str, "COLS" ) )
			{
				if( !sscanf_s( str.c_str(), "COLS=%u", &cols ) )
				{
					Log.Error( "GLEVEL( " + file + " ): Nie mo¿na odczytaæ liczby kolumn!" );
					Free();
					fclose( fp );
					return false;
				}
			}
			if( guiIsInStr( str, "WEAPCOUNT" ) )
			{
				if( !sscanf_s( str.c_str(), "WEAPCOUNT=%u", &WeapCount ) )
				{
					Log.Error( "GLEVEL( " + file + " ): Nie mo¿na odczytaæ liczby broni!" );
					Free();
					fclose( fp );
					return false;
				}
			}
			if( guiIsInStr( str, "BONUSCOUNT" ) )
			{
				if( !sscanf_s( str.c_str(), "BONUSCOUNT=%u", &BonusCount ) )
				{
					Log.Error( "GLEVEL( " + file + " ): Nie mo¿na odczytaæ liczby broni!" );
					Free();
					fclose( fp );
					return false;
				}
			}
			if( guiIsInStr( str, "ENEMYTYPECOUNT" ) )
			{
				if( !sscanf_s( str.c_str(), "ENEMYTYPECOUNT=%u", &EnemyTypeCount ) )
				{
					Log.Error( "GLEVEL( " + file + " ): Nie mo¿na odczytaæ liczby typów wroga!" );
					Free();
					fclose( fp );
					return false;
				}
			}
			if( guiIsInStr( str, "ENEMYCOUNT" ) )
			{
				if( !sscanf_s( str.c_str(), "ENEMYCOUNT=%u", &EnemyCount ) )
				{
					Log.Error( "GLEVEL( " + file + " ): Nie mo¿na odczytaæ liczby wrogów!" );
					Free();
					fclose( fp );
					return false;
				}
			}
			if( guiIsInStr( str, "PLAYERSTARTPOS" ) )
			{
				if( !sscanf_s( str.c_str(), "PLAYERSTARTPOS=%u", &PlayerStartBlock ) )
				{
					Log.Error( "GLEVEL( " + file + " ): Nie mo¿na odczytaæ pozycji startowej gracza!" );
					Free();
					fclose( fp );
					return false;
				}
			}
			if( guiIsInStr( str, "PLAYERSTARTANGLE" ) )
			{
				if( !sscanf_s( str.c_str(), "PLAYERSTARTANGLE=%u", &PlayerStartAngle ) )
				{
					Log.Error( "GLEVEL( " + file + " ): Nie mo¿na odczytaæ obrotu startowego gracza!" );
					Free();
					fclose( fp );
					return false;
				}
			}
			if( guiIsInStr( str, "STATOBJ" ) )
			{
				if( !sscanf_s( str.c_str(), "STATOBJ=%u", &StatObjCount ) )
				{
					Log.Error( "GLEVEL( " + file + " ): Nie mo¿na odczytaæ liczby objektów statycznych!" );
					Free();
					fclose( fp );
					return false;
				}
			}
			if( guiIsInStr( str, "WINALL" ) )
			{
				if( !sscanf_s( str.c_str(), "WINALL=%d", &AllWin ) )
				{
					Log.Error( "GLEVEL( " + file + " ): Nie mo¿na odczytaæ flagi!" );
					Free();
					fclose( fp );
					return false;
				}
			}
			if( guiIsInStr( str, "LOSEALL" ) )
			{
				if( !sscanf_s( str.c_str(), "LOSEALL=%d", &AllLose ) )
				{
					Log.Error( "GLEVEL( " + file + " ): Nie mo¿na odczytaæ flagi!" );
					Free();
					fclose( fp );
					return false;
				}
			}
			if( guiIsInStr( str, "WINFLAGS" ) )
			{
				if( !sscanf_s( str.c_str(), "WINFLAGS=%u", &WinFlags.flags ) )
				{
					Log.Error( "GLEVEL( " + file + " ): Nie mo¿na odczytaæ flagi!" );
					Free();
					fclose( fp );
					return false;
				}
			}
			if( guiIsInStr( str, "WINBLOCK" ) )
			{
				if( !sscanf_s( str.c_str(), "WINBLOCK=%u", &WinFlags.BlockID ) )
				{
					Log.Error( "GLEVEL( " + file + " ): Nie mo¿na odczytaæ flagi!" );
					Free();
					fclose( fp );
					return false;
				}
			}
			if( guiIsInStr( str, "LOSEFLAGS" ) )
			{
				if( !sscanf_s( str.c_str(), "LOSEFLAGS=%u", &LoseFlags.flags ) )
				{
					Log.Error( "GLEVEL( " + file + " ): Nie mo¿na odczytaæ flagi!" );
					Free();
					fclose( fp );
					return false;
				}
			}
			if( guiIsInStr( str, "LOSEBLOCK" ) )
			{
				if( !sscanf_s( str.c_str(), "LOSEBLOCK=%u", &LoseFlags.BlockID ) )
				{
					Log.Error( "GLEVEL( " + file + " ): Nie mo¿na odczytaæ flagi!" );
					Free();
					fclose( fp );
					return false;
				}
			}
		}
		while( str != "END HEADER" );	
	}
	Log.Log( "GLEVEL( " + file + " ): Nazwa poziomu: " + LevName );

	/*	Dalej pobieramy dane o plikach
		tekstur do poziomów.
		Pierwszy to œciany, drugi sufit,
		trzeci pod³oga.
	*/
	str = GetString( fp );
	if( str != "TEXTURES" )
	{
		Log.Error( "GLEVEL( " + file + " ): Brak listy tekstur!" );
		Free();
		fclose( fp );
		return false;
	}
	else
	{
		str = GetString( fp );
		if( !( Tex[0] = TManager.Get( GetParamStr( str ) ) ) )
		{
			Log.Error( "GLEVEL( " + file + " ): B³êdny plik graficzny!" );
			Free();
			fclose( fp );
			return false;
		}
		str = GetString( fp );
		if( !( Tex[1] = TManager.Get( GetParamStr( str ) ) ) )
		{
			Log.Error( "GLEVEL( " + file + " ): B³êdny plik graficzny!" );
			Free();
			fclose( fp );
			return false;
		}
		str = GetString( fp );
		if( !( Tex[2] = TManager.Get( GetParamStr( str ) ) ) )
		{
			Log.Error( "GLEVEL( " + file + " ): B³êdny plik graficzny!" );
			Free();
			fclose( fp );
			return false;
		}

		str = GetString( fp );
		if( str != "END TEXTURES" )
		{
			Log.Error( "GLEVEL( " + file + " ): Brak zakoñczenia liczby tesktur!" );
			Free();
			fclose( fp );
			return false;
		}
	}

	/*	Teraz tworzymy tablicê odpowiednich
		rozmiarów zdoln¹ pomieœciæ nastêpne
		dane. S¹ to liczby ca³kowite jednoznacznie
		okreœlaj¹ce zawarte w nich œciany ( patrz
		definicje makrowe )
	*/
	str = GetString( fp );
	if( str != "WALLS" )
	{
		Log.Error( "GLEVEL( " + file + " ): Brak listy œcian!" );
		Free();
		fclose( fp );
		return false;
	}
	block = new gameBlockInfo[rows*cols];

	for( i = 0; i < rows*cols; i++ )
	{
		str = GetString( fp );
		
		// Wpisujemy t¹ liczbê do sk³adowej walls
		sscanf_s( str.c_str(), "%d", &block[i].walls );
		block[i].CornerCount = 0;
	}

	str = GetString( fp );
	if( str != "END WALLS" )
	{
		Log.Error( "GLEVEL( " + file + " ): Brak koñca listy œcian!" );
		Free();
		fclose( fp );
		return false;
	}
	
	if( WeapCount > 0 )
	{
		str = GetString( fp );

		if( str != "WEAPLIST" )
		{
			Log.Error( "GLEVEL( " + file + " ): Brak listy broni!" );
			Free();
			fclose( fp );
			return false;
		}

		for( i = 0; i < WeapCount; i++ )
		{
			str = GetString( fp );

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
			Weap->Init();
			Weap->Pos = GetBlockPos( j );
			Weap->NextPos = Weap->Pos;
			WManager.AddWeapon( Weap );
		}

		str = GetString( fp );
		if( str != "END WEAPLIST" )
		{
			Log.Error( "GLEVEL( " + file + " ): Brak koñca listy broni!" );
			Free();
			fclose( fp );
			return false;
		}
	}

	if( EnemyTypeCount > 0 )
	{
		str = GetString( fp );
		EnemyType = new std::string[EnemyTypeCount];

		if( str != "ENEMYTYPELIST" )
		{
			Log.Error( "GLEVEL( " + file + " ): Brak listy typów wroga!" );
			Free();
			fclose( fp );
			return false;
		}

		for( i = 0; i < EnemyTypeCount; i++ )
		{
			str = GetString( fp );
			EnemyType[i] = str;
		}

		str = GetString( fp );
		if( str != "END ENEMYTYPELIST" )
		{
			Log.Error( "GLEVEL( " + file + " ): Brak koñca listy typów wroga!" );
			Free();
			fclose( fp );
			return false;
		}
	}

	if( EnemyCount > 0 && EnemyTypeCount > 0)
	{
		str = GetString( fp );

		if( str != "ENEMYLIST" )
		{
			Log.Error( "GLEVEL( " + file + " ): Brak listy wrogów!" );
			Free();
			fclose( fp );
			return false;
		}

		for( i = 0; i < EnemyCount; i++ )
		{
			str = GetString( fp );

			sscanf_s( str.c_str(), "%d=%d,%d", &j, &k, &l );

			CEnemy* Enemy = new CEnemy;
			Enemy->SetStartPos( this->GetBlockPos( j ) );
			Enemy->SetStartAngle( (float)l );
			Enemy->LoadEnemy( EnemyType[k] );
			Enemy->Reset();
			ThingManager.AddThing( Enemy );
		}

		str = GetString( fp );
		if( str != "END ENEMYLIST" )
		{
			Log.Error( "GLEVEL( " + file + " ): Brak koñca listy wrogów!" );
			Free();
			fclose( fp );
			return false;
		}
	}

	if( StatObjCount > 0 )
	{
		str = GetString( fp );

		if( str != "STATOBJLIST" )
		{
			Log.Error( "GLEVEL( " + file + " ): Brak listy statycznych objektów!" );
			Free();
			fclose( fp );
			return false;
		}

		for( i = 0; i < StatObjCount; i++ )
		{
			str = GetString( fp );

			sscanf_s( str.c_str(), "%d,%d=", &k, &l );

			//gameStatObj *obj = new gameStatObj;
			//obj->LoadObj( GetParamStr( str ) );
			//obj->Pos = this->GetBlockPos( k );
			//obj->SetAngle( l );
			//ThingManager.AddThing( obj );
		}

		str = GetString( fp );
		if( str != "END STATOBJLIST" )
		{
			Log.Error( "GLEVEL( " + file + " ): Brak koñca listy statycznych objektów!" );
			Free();
			fclose( fp );
			return false;
		}
	}

	if( BonusCount > 0 )
	{
		str = GetString( fp );

		if( str != "BONUSLIST" )
		{
			Log.Error( "GLEVEL( " + file + " ): Brak listy bonusów!" );
			Free();
			fclose( fp );
			return false;
		}

		for( i = 0; i < BonusCount; i++ )
		{
			int a, b;
			str = GetString( fp );

			sscanf_s( str.c_str(), "%d,%d/", &k, &l );
			weBonus* Bonus;
			switch( l )
			{
			case BONUS_TYPE_AMMO :
				sscanf_s( str.c_str(), "%d,%d/%d,%d=", &k, &l, &a, &b );
				Bonus = new weAmmo;
				((weAmmo*)Bonus)->Init( a, b, GetParamStr( str ) );
				break;
			case BONUS_TYPE_HEALTH :
				sscanf_s( str.c_str(), "%d,%d/%d=", &k, &l, &a );
				Bonus = new weHealth;
				((weHealth*)Bonus)->Init( a, GetParamStr( str ) );
				break;
			case BONUS_TYPE_ARMOR :
				sscanf_s( str.c_str(), "%d,%d/%d=", &k, &l, &a );
				Bonus = new weArmor;
				((weArmor*)Bonus)->Init( a, GetParamStr( str ) );
				break;
			case BONUS_TYPE_UNKNOWN :
			default:
				continue;
			}

			Bonus->Pos = GetBlockPos( k );
			BonusMan.AddBonus( Bonus );
		}

		str = GetString( fp );
		if( str != "END BONUSLIST" )
		{
			Log.Error( "GLEVEL( " + file + " ): Brak koñca listy bonusów!" );
			Free();
			fclose( fp );
			return false;
		}
	}

	// Zamykamy plik
	str = GetString( fp );
	if( str != "END E3DTLEV" )
	{
		Log.Error( "GLEVEL( " + file + " ): Brak koñca pliku!" );
	}
	fclose( fp );

	MainPlayer.SetStartPos( this->GetBlockPos( PlayerStartBlock ) );
	MainPlayer.SetStartAngle( (float)PlayerStartAngle );
	MainPlayer.Reset();

	loaded = true;

	return true;
}

/*	METODA RYSUJ¥CA - ŒCIANY
	W zale¿noœci od tego jak¹
	œciane chcemy, tak¹ rysuje
	razem z koordynatami tekstur
*/
void gameLevel::DrawWall( unsigned int wall )
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

/*	METODA RYSUJ¥CA - POD£OGA I SUFIT
	Metody podobne do poprzedniej - rysuj¹
	pod³oge i sufit razem z koordynatami.
*/
void gameLevel::DrawFloor()
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

void gameLevel::DrawTop()
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

/*	Ta metoda jednorazowo wywo³uje dwie
	inne metody
*/
void gameLevel::InitLevel()
{
	// Gdy poziom ju¿ jest za³adowany, to trzeba przerwaæ inicjalizacje
	if( !loaded )
		return;

	WinFlags.VerifyFlags();
	LoseFlags.VerifyFlags();
	BuildVisual();
	BuildPhisic();
}

/*	W tej metodzie budujemy czêœæ wizualn¹
	poziomu, oddzielnie od reszty logiki. W programowaniu gier jest ZAWSZE
	zasada odzielania tych czêœci.
*/
void gameLevel::BuildVisual()
{
	if( !loaded )
		return;

	GUI.SendConMsg( "Poziom: Tworzenie czesci wizualnej...", false );
	// Zmienne pomocnicze
	int i, j, blk;

	// Generujemy listê
	Top = glGenLists( 1 );
	Wall = glGenLists( 1 );
	Floor = glGenLists( 1 );
	/*	Teraz tworzymy listê. Listy wyœwietlania
		maj¹ te zalete, ¿e raz stworzone BARDZO
		przyspieszaj¹ wyœwietlanie, dlatego op³aca
		siê je stosowaæ. W zamian nie mo¿na nic zmieniæ
		w liœcie, no chyba, ¿e tworz¹c j¹ od nowa.
	*/
	/*	Ta czêœæ kodu tysuje pod³oge, na ca³ej
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
	// To samo co poprzednio, tylko, ¿e z sufitem
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
	/*	W tej pêtli s¹ rysowane œciany
		w zale¿noœci jaka œciana jest zawarta
		w fladze, tak¹ rysujemy.
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

/*	Ta metoda jest odpowiedzilna, za czêœæ fizyczn¹ poziomu
	buduje, m.in. punkty i p³aszczyzny, w oparciu o które
	bêd¹ wykrywane kolizje.
*/
void gameLevel::BuildPhisic()
{
	if( !loaded )
		return;

	GUI.SendConMsg( "Poziom: Tworzenie czesci fizycznej...", false );
	// Zmienne pomocznicze
	int i, j, k, ai, aj;
	gameBlockInfo* blk;
	gameBlockInfo* SideBlock[4];
	float nx, ny;

	/*	Pêtla sprawdza ka¿de 4 œciany
		w ka¿dym bloku, czy nale¿y stworzyæ
		w tym miejscu przeszkode.
	*/
	for( i = 0; i < rows; i++ )
	{
		for( j = 0; j < cols; j++ )
		{
			blk = &block[ i * cols + j ];
			
			for( k = 0; k < 4; k++ )
			{
				/*	BTW: tu mo¿na by³o by wymyœleæ jakiœ algorytm,
					ale nie mia³em pomys³u, wiêc po prostu pos³u¿y³em
					siê switch'em.
				*/
				switch( k )
				{
					/*	Wybieramy jakie ustawienia wybraæ dla danej œciany.
						nx, ny - wektor normalny œciany ( wskazuje gdzie jest przód )
						ai, aj - dodatek do punktu dolnego, prawego ( bo jakoœ trzeba ustawiæ rogi
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
				// Tworzymy wektor normalny p³aszczyzny œciany
				blk->WPlane[k].Normal.Set( nx, 0.0f, ny );
				// Wyliczamy przesuniêcie p³aszczyzny, w oparciu o Punkt ( w tym przypadku róg bloku ) i Wektor Normalny
				blk->WPlane[k].D = -blk->Corner[k].Dot( blk->WPlane[k].Normal );
			}
		}
	}

	/*	Zauwa¿y³em, ¿e kiedy teraz próbowa³bym 
		wejœæ na jakiœ róg na œcianach, to bym 
		po prostu... przeszed³ na druga stronê!
		Coœ takiego nie mo¿e mieæ miejsca, dlatego
		tworzymy odpowiednie rogi "kolizyjne". 
		A dlaczego nie wykorzystamy ju¿ wczeœniej
		stworzonych?? Bo sytuacja gdy siê blokujemy
		na œrodku pustej przestrzenie, te¿ jest 
		absurdalna :)
	*/
	for( i = 0; i < rows; i++ )
	{
		for( j = 0; j < cols; j++ )
		{
			blk = &block[ i * cols + j ];

			/*	Znajdujemy bloki s¹siednie, by mo¿na
				by³o znaleŸæ rogi œcian.
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
			/*	Teraz na ka¿d¹ œciany ( i jednoczeœnie blok s¹siedni ze œcian¹ )
				szukamy zetkniêcia lub rogu. Gdy taki znajdujemy, to zapisujemy
				ten róg i zwiêkszamy liczbê rogów do sprawdzenia.
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

/*	Tu odbywa siê rysowanie levelu. 
	( takie proste, ¿e a¿ g³upie :) )
*/
void gameLevel::DrawLevel()
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

void gameLevel::DrawAllTop()
{
	if( !loaded )
		return;

	Tex[1]->Activate( GUI.GetTexDLevel() );
	glCallList( Top );
}

void gameLevel::DrawAllWall()
{
	if( !loaded )
		return;

	Tex[0]->Activate( GUI.GetTexDLevel() );
	glCallList( Wall );
}

void gameLevel::DrawAllFloor()
{
	if( !loaded )
		return;

	Tex[2]->Activate( GUI.GetTexDLevel() );
	glCallList( Floor );
}

void gameLevel::DrawReflect()
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

/*	Ta metoda zwraca nazwê poziomu wyczytan¹ z pliku
	(lub domyœln¹).
*/
std::string gameLevel::GetLevelName()
{
	return LevName;
}

/*	Ta zwraca dany block z okreœlonego wiersza i kolumny
*/
gameBlockInfo* gameLevel::GetBlock( unsigned int i, unsigned int j )
{
	if( i >= 0 && j >= 0 )
	{
		if( j < rows && i < cols )
		{
			return &block[ j * cols + i ];
		}
	}
	return NULL;
}

gameBlockInfo* gameLevel::GetBlock( Vector3f Pos )
{
	return GetBlock( (unsigned int)Pos.X, (unsigned int)Pos.Z );
}

gameBlockInfo* gameLevel::GetBlock( unsigned int i )
{
	return &block[i];
}

unsigned int gameLevel::GetBlockCount()
{
	return rows*cols;
}

Vector3f gameLevel::GetBlockPos( unsigned int i )
{
	return Vector3f( (float)(i % cols)*10.0f+5.0f, 0.0f, -(float)(i / cols)*10.0f-5.0f );
}

void gameLevel::CheckWLFlags()
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


/*	Metoda czyœci pamiêc zarezerwowan¹ dla
	tej klasy.
*/
void gameLevel::Free()
{
	if( !loaded )
		return;

	GUI.SendConMsg( "Poziom: Zwalnianie pamieci...", false );

	if( block != NULL )
	{
		delete[] block;
		block = NULL;
	}

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

/*	To jest odzielna funkcja sprawdzaj¹ca kolizje
	obiektu CEntity, ze œcianami podanego bloku.
	Jest to samodzielna funkcja, by mo¿na by³o j¹ wygonie
	u¿yæ, nie korzystaj¹c z klasy gameLevel.
*/
bool TestCollBlock( CEntity* Dum, gameBlockInfo* Block, bool testthing )
{
	if( Block == NULL )
		return false;


	// zmienne pomocnicze
	int i;
	Vector3f A, B, Normal, V, P;
	float wynik, D, W;
	bool is = false;

	/*	A teraz troche trygonometrii :)
		Aby wykryæ kolizje, w grach 3d u¿ywa
		sie sprawdzenia przeciêcia p³aszczyzny przez
		sferê ( kulê ). Polega ona, ¿e do po³orzenia kuli
		dodajemy wektor o d³ugoœci równej promieniowy kuli.
		Ca³e sprawdzenie opiera siê na dwóch takich punktach
		aktualnej pozycji i nastêpnej pozycji. Nastepnie spraw-
		dzamy po³orzenie tych dwóch punktów wzglêdem p³aszczyzny
		œciany. Je¿eli le¿¹ po tej samej stronie, to nie ma kolizji,
		a je¿eli po ró¿nych, to kolizja nast¹pi(³a). W grach 3D dodatkowo
		sprawdza siê p³aszczyzny na krawêdziach œciany, by stwierdziæ czy 
		kolizja nast¹pi³a na œcianie ( bo sama p³aszczyzna nie jest
		ograniczona i mo¿e siê roci¹gaæ poza œcian¹ ), ale tu
		operujemy na 4 œcianach, w dodatku pewnie ograniczonych, wiêc
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

		/* FIXME: nie wiem dlaczego, ale je¿eli aktualna
			pozycja ma dodany promieñ to czêsto kolizja
			nie jest wykryta (?) kiedyœ to musze naprawiæ...
		*/
		// Aktualna pozycja
		A = Dum->Pos;// + Normal.Reverse() * R;
		// Nastêpna pozycja
		B = Dum->NextPos + Normal.Reverse() * Dum->Radius;

		// Wyra¿enie liczy, gdzie s¹ punkty wzglêdem p³aszczyzny
		wynik = ( Normal.Dot( A ) + D ) * ( Normal.Dot( B ) + D );

		// wynik > 0 - po tej samej stronie
		// wynik < 0 - po ró¿nych stronach
		if( wynik >= 0 ) 
			continue;

		/*	No dobra, wiemy, ¿e nast¹pi(³a) kolizja. Co dalej??
			Najlepiej wyliczyæ punkt przeciêcia p³aszczyzny i 
			wyliczyæ now¹ pozycjê nastêpnego ruchu w oparciu
			o wektor normalny œciany o d³ugoœci promienia.
			To dzia³a i nie wygl¹da sztucznie :)
		*/
		// Wyliczamy punkt przeciêcia
		V = A - B;
		W = ( Normal.Dot( B ) + D ) / Normal.Dot( V );
		P = B - V * W;

		is = true;
		// i tworzymy now¹ pozycjê.
		Dum->NextPos = P + Normal * Dum->Radius;
	}

	/*	No dobra, wykrywamy kolizje na œcianach, ale
		co zrobiæ gdy block nie ma œcian, a w s¹siednich
		jest róg? Najlepiej sprawdziæ czy odleg³oœæ
		rogu od nastêpnej pozycji nie jest mniejsza od
		promienia :). Wtedy wystarczy stworzyæ now¹ p³aszczyzne
		na podstawie rogu, a pod wektor normalny podstawiæ wektor
		od pozycji gracza, do rogu. Potem powtarzamy regu³ke i 
		wszystko piêknie dzia³a :)
	*/
	
	for( i = 0; i < Block->CornerCount; i++ )
	{
		if( mathDistSq( Dum->NextPos, Block->TCorner[i] ) > POW(Dum->Radius) )
			continue;
	
		// liczymy wektor od rogu, do pozycji
		V = Dum->NextPos - Block->TCorner[i];
		// i go normalizujemy, by siê nadawa³ do wyliczenia odleg³oœci
		V.Normalize();
		Normal = V;
		D = -Block->TCorner[i].Dot( V );

		// Aktualna pozycja
		A = Dum->Pos;// + Normal.Reverse() * R;
		// Nastêpna pozycja
		B = Dum->NextPos + Normal.Reverse() * Dum->Radius;

		// Powtarzamy poprzedni¹ regu³ke...
		wynik = ( Normal.Dot( A ) + D ) * ( Normal.Dot( B ) + D );
		if( wynik >= 0 ) 
			continue;
		V = A - B;
		W = ( Normal.Dot( B ) + D ) / Normal.Dot( V );
		P = B - V * W;

		is = true;
		// i tworzymy now¹ pozycjê.
		Dum->NextPos = P + Normal * Dum->Radius;
	}

	if( !testthing )
		return is;

	/*	No dobra, wykrywamy kolizje na œcianach i na rogach, ale co z innymi graczami/przeciwnikami?
		Tu jest trochê odmienna sprawa, ale te¿ da siê wyjœæ z sytuacji. Najpierw sprawdzamy, który z
		przeciwników jest na tyle blisko, ¿e mo¿e pojawiæ siê kolizja. Potem usyskujemy wektor normalny
		od przeciwnika do badanej kuk³y. Dobra mamy ju¿ wektor, ale ¿eby zbudowaæ p³aszczyzne, jest jeszcze
		potrzebny punkt przeciêcia sfer. Mo¿na to uzyskaæ mno¿¹c wektor normalny przez promieñ przeciwnika
		i dodaj¹c to do jego po³orzenia. No œwietnie, mamy doœæ informacji, by skonstruowaæ p³aszczyzne.
		Kiedy ju¿ stwierdzimy wyst¹pienie kolizji, to oddalamy kuk³e na odleg³oœæ sumy promieni pomno¿onej
		przez wektor normalny. I tyle :)
	*/
	CActor* Thing;
	for( i = 0; i < ThingManager.Count(); i++ )
	{
		Thing = ThingManager.GetThing( i );
		// Po kiego maja sprawdzaæ kolizje na martwym przeciwniku??
		if( Thing->IsDead() )
			continue;
		// No chyba sam ze sob¹ nie mo¿na siê zderzyæ... :D
		if( Dum == Thing )
			continue;
		if( Dum != &MainPlayer && Thing->GetType() == GAME_THING_ENEMY )
			continue;
		// Sprawdzamy odleg³oœæ ( ja jeszcze doda³em dwie jednostki, by sprawdzanie rozpocze³o siê wczeœniej )
		if( mathDistSq( Thing->Pos, Dum->NextPos ) > POW( Thing->Radius + Dum->Radius + 2.0f ) )
			continue;

		// Tworzymy p³aszczyzne i punkt przeciêcia.
		Normal = Dum->NextPos - Thing->NextPos;
		Normal.Normalize();
		V = Thing->Pos + Normal * Thing->Radius;
		D = -V.Dot( Normal );

		// Aktualna pozycja
		A = Dum->Pos;// + Normal.Reverse() * R;
		// Nastêpna pozycja
		B = Dum->NextPos + Normal.Reverse() * Dum->Radius;

		// Powtarzamy poprzedni¹ regu³ke...
		wynik = ( Normal.Dot( A ) + D ) * ( Normal.Dot( B ) + D );
		if( wynik >= 0 ) 
			continue;
		V = A - B;
		W = ( Normal.Dot( B ) + D ) / Normal.Dot( V );
		P = B - V * W;

		is = true;
		// i tworzymy now¹ pozycjê.
		Dum->NextPos = P + Normal * Dum->Radius;
	}
	
	return is;
}

/*	Ta funkcja wykorzystuje technikê zwan¹ Ray Casting
	czyli wypuszczanie/puszczanie promieni. Polega ona
	na tym, by z punktu pocz¹tkowego w danym kierunku,
	co okreœlony krok badaæ kolizje, a¿ do znalezienia
	punktu kolizji. Kolizje s¹ sprawdzane za pomoc¹
	poprzedniej funkcji.
*/
Vector3f RayCast( Vector3f Pos, Vector3f Veloc, float Step, gameLevel* Level )
{
	if(Veloc.LeangthSq() == 0.0f)
		return Pos;

	CEntity Dum;
	Dum.Radius = 0.1f;
	Dum.NextPos = Pos;
	gameBlockInfo* Block = NULL;

	do
	{
		Dum.Pos = Dum.NextPos;
		Dum.NextPos = Dum.Pos + Veloc * Step;

		Block = Level->GetBlock( Dum.GetBlockPos() );
		/*	Takie ma³e zabezpieczenie.
			Kiedy promieñ znajdzie siê poza poziomem
			( gdzie nie ma ¿adnych œcian ) to zamiast
			siê zawiesiæ, to niech zwróci bezpieczny
			punkt œrodka (0, 0, 0).
		*/
		if( Block == NULL )
		{
			return Vector3f( 0.0f, 0.0f, 0.0f );
		}
	}
	while( !TestCollBlock( &Dum, Block ) );

	return Dum.NextPos;
}

/*	Ta funckja jest tak ³atwa, ¿e a¿ œmieszna.
	sprawdza ona odleg³oœæ miêdzy dwoma kuk³ami i
	zwraca czy siê zde¿y³y, czy nie.
*/
bool TestCollDum( CEntity* Dum, CEntity* Dum2 )
{
	Vector3f V1 = ClosestPoint( Dum2->Pos, Dum2->NextPos, Dum->NextPos );

	if( mathDistSq( V1, Dum->NextPos ) < POW(Dum->Radius) )
		return true;
	else return false;
}

/*	Tutaj coœ lepszego - ta funckja sprawdza czy miêdzy
	dowama punktami nie ma przeszkód. Wykorzystuje to m.in.
	do sprawdzania widocznoœci.
*/
bool IsCollOnRay( Vector3f V1, Vector3f V2, int Steps )
{
	Vector3f Veloc = V2 - V1;
	Vector3f Step = Veloc / (float)Steps;
	CEntity Dum;
	Dum.Radius = 0.1f;
	Dum.NextPos = V1;
	gameBlockInfo* Block = NULL;

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