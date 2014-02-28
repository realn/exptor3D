/*///////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
Plik:	Game.cpp
Autor:	Real_Noname (real_noname@wp.pl)
(C):	CODE RULERS (Real_Noname)
WWW:	www.coderulers.prv.pl
Opis:	Patrz -> Game.h

/////////////////////////////////////////////////////////
///////////////////////////////////////////////////////*/
#include "Game.h"

/*=========================	
	KLASA gameThing


=========================*/
gameThing::gameThing()
{
	AIflags = AI_MOVE_AROUND;
	Health = 100.0f;
	MaxHealth = 100.0f;
	Armor = 0.0f;
	MaxArmor = 0.0f;
	Dead = false;
	Dying = false;
	Speed = 0.1f;
	RotSpeed = 1.0f;
	Angle = 180.0f;
	StartAngle = 180.0f;
	ToAngle = 180.0f;
	Pos.Set( 5.0f, 0.0f, -25.0f );
	R = 2.0f;
	Type = GAME_THING_OTHER;
	Model = NULL;
	Enemy = NULL;
	DyingTime = 60.0f;
	ThisDTime = 0.0f;
	AIState = AI_STATE_NOTHING;
}

gameThing::~gameThing()
{
	Free();
	Model = NULL;
}

void gameThing::Init()
{
}

Vector3f gameThing::CreatePos( float ang )
{
	Vector3f zwrot;

	zwrot.X = sinf( ang * PIOVER180 );
	zwrot.Z = -cosf( ang * PIOVER180 );

	return zwrot;
}

void gameThing::Move( unsigned int flags, const float fTD )
{
	Vector3f temp;

	if( flags & GAME_MOVE_FORWARD )
		temp += CreatePos( Angle );

	if( flags & GAME_MOVE_BACK )
		temp += CreatePos( Angle - 180 );

	if( flags & GAME_MOVE_STRAFE_L )
		temp += CreatePos( Angle - 90 );

	if( flags & GAME_MOVE_STRAFE_R )
		temp += CreatePos( Angle + 90 );

	if(temp.X == 0.0f && temp.Y == 0.0f && temp.Z == 0.0f)
		NextPos = Pos;
	else
	{
		temp.Normalize();
		NextPos = Pos + temp * Speed * GUI.GetSpeed() * fTD;
	}

	MoveVector = CreatePos( Angle );
	MoveVector.Normalize();
}

void gameThing::DoEngine( const float fTD )
{
	if( Dead )
		return;

	if( Health <= 0.0f && !Dying)
	{
		this->OnDie();
		this->Dying = true;
	}

	if( Dying && !Dead )
		ThisDTime += 1.0f * fTD;

	if( ThisDTime >= DyingTime && !Dead)
	{
		this->OnDead();
		Dead = true;
	}

	DoAI();

	Move( Actions, fTD );

	Actions = 0;

	TestCollBlock( this, GLevel.GetBlock( this->GetBlockPos() ), true );
	Pos = NextPos;
}

void gameThing::DoDraw()
{
	// Dla niezdefiniowanych obiektów to pole pozostaje puste
}

void gameThing::OnDie()
{
	// Nic
}

void gameThing::OnDead()
{
	// Nic
}

bool gameThing::IsEnemyInFront()
{
	if( Enemy == NULL )
		return false;

	if( fabsf( GetAngle( Enemy->NextPos, Pos )- Angle )  < 3.0f )
		return true;

	return false;
}

void gameThing::DoAI()
{
	if( this->AIflags == AI_NO_AI )
		return;

	if( this->AIflags & AI_PLAYER_ENEMY )
	{
		if( this->AIFindTarget() )
		{
			HasTarget = true;
			Target = Enemy->NextPos;

			if( mathDist( Pos, Target ) < 70.0f )
			{
				if( this->AIflags & AI_ATTACK_CLOSE )
				{
					if( mathDist( Pos, Target ) > 5.0f )
						this->AIWalk();
					else this->AIAttackTarget();
				}
				else if( this->AIflags & AI_ATTACK_FAR )
					this->AIAttackTarget();

				return;
			}
		}
	}

	if( this->AIflags & AI_MOVE_AROUND )
	{
		if( this->AIState == AI_STATE_STAND )
		{
			Target = this->AIFindWalkTarget();
			HasTarget = true;

			this->AIWalk();
		}
		else if( this->AIState == AI_STATE_WALK )
			this->AIWalk();
	}

	if( AIState == AI_STATE_NOTHING )
		AIStand();
}

bool gameThing::AIFindTarget()
{
	if( this->AIflags & AI_PLAYER_ENEMY )
	{
		Enemy = &MainPlayer;

		if( !IsCollOnRay( Enemy->NextPos, Pos ) )
			return true;
		else 
		{
			if( AIState == AI_STATE_FIRE )
				AIState = AI_STATE_NOTHING;
			Enemy = NULL;
			return false;
		}
	}

	if( ThingManager.Count() == 0 )
		return false;

	unsigned int i = 0;
	for( i = 0; i < ThingManager.Count(); i++ )
	{
		Enemy = ThingManager.GetThing( i );

		if( !IsCollOnRay( Enemy->NextPos, Pos ) )
			return true;
	}

	Enemy = NULL;
	return false;
}

Vector3f gameThing::AIFindWalkTarget()
{
	gameBlockInfo* Block = GLevel.GetBlock( this->GetBlockPos() );
	if( Block == NULL )
		return Vector3f( 0.0f ,0.0f, 0.0f );
	
	bool waysok[4] = { false, false, false, false };
	int waycount = 0;
	if( !( Block->walls & LEV_WALL_LEFT ) )
	{
		waycount++;
		waysok[0] = true;
	}
	if( !( Block->walls & LEV_WALL_RIGHT ) )
	{
		waycount++;
		waysok[1] = true;
	}
	if( !( Block->walls & LEV_WALL_TOP ) )
	{
		waycount++;
		waysok[2] = true;
	}
	if( !( Block->walls & LEV_WALL_BOTTOM ) )
	{
		waycount++;
		waysok[3] = true;
	}

	if( waycount == 0 )
		return Vector3f( 0.0f ,0.0f, 0.0f );

	Vector3f BPos = this->GetBlockPos();
	do
	{
		int way = rand() % 4;
		if( !waysok[way] )
			continue;

		switch( way )
		{
		case 0 :
			return Vector3f( ( BPos.X - 1.0f ) * 10.0f + 5.0f, 0.0f, ( BPos.Z ) * -10.0f - 5.0f );
		case 1 :
			return Vector3f( ( BPos.X + 1.0f ) * 10.0f + 5.0f, 0.0f, ( BPos.Z ) * -10.0f - 5.0f );
		case 2 :
			return Vector3f( ( BPos.X ) * 10.0f + 5.0f, 0.0f, ( BPos.Z + 1.0f ) * -10.0f - 5.0f );
		case 3 :
			return Vector3f( ( BPos.X ) * 10.0f + 5.0f, 0.0f, ( BPos.Z - 1.0f ) * -10.0f - 5.0f );		
		}
	}
	while( true );
}

void gameThing::AIWalk()
{
	if( !HasTarget )
	{
		AIState = AI_STATE_NOTHING;
		return;
	}

	if( IsCollOnRay( Target, NextPos ) )
	{
		AIStand();
		return;
	}

	AIState = AI_STATE_WALK;

	float Ang = SwapAngle( GetAngle( Target, NextPos ) );

	this->GoToAngle( Ang );

	if( mathDist( Pos, Target ) < 0.5f )
	{
		AIState = AI_STATE_NOTHING;
		return;
	}

	if( fabsf( Ang - Angle ) < 1.0f )
	{
		Actions |= GAME_MOVE_FORWARD;
	}

}

void gameThing::AIStand()
{
	AIState = AI_STATE_STAND;
}

void gameThing::AIAttackTarget()
{
	if( IsEnemyInFront() )
	{
		AIState = AI_STATE_FIRE;
		Fire( Target );
	}
	else
	{
		if( Enemy != NULL )
		{
			GoToAngle( GetAngle( Enemy->NextPos, Pos ) );
		}
	}
}

bool gameThing::IsDying()
{
	return Dying;
}

bool gameThing::IsDead()
{
	return Dead;
}

void gameThing::Fire( Vector3f Target )
{
	// Dla niezdefiniwanych obiektów to pole jest puste
}

float gameThing::GetAng()
{
	return Angle;
}

void gameThing::ModAngle( float mod )
{
	Angle += mod;
	Angle = SwapAngle( Angle );
}

void gameThing::SetAngle( float set )
{
	Angle = set;
	Angle = SwapAngle( Angle );
}

void gameThing::GoToAngle( float ang )
{
	ang = SwapAngle( ang );
	if( ang > Angle )
	{
		if( ang - Angle < 180.0f )
		{
			ModAngle( RotSpeed * GUI.GetSpeed() );
		}
		else
		{
			ModAngle( -RotSpeed * GUI.GetSpeed() );
		}
	}
	else if( ang < Angle )
	{
		if( Angle - ang < 180.0f )
		{
			ModAngle( -RotSpeed * GUI.GetSpeed() );
		}
		else
		{
			ModAngle( RotSpeed * GUI.GetSpeed() );
		}
	}
}

void gameThing::SetStartAngle( float set )
{
	StartAngle = set;
}

float gameThing::GetStartAngle()
{
	return StartAngle;
}

float gameThing::GetHealth()
{
	return Health;
}

void gameThing::ModHealth( float mod )
{
	Health += mod;
	if( Health > MaxHealth )
		Health = MaxHealth;
}

void gameThing::SetHealth( float set )
{
	Health = set;
}

float gameThing::GetArmor()
{
	return Armor;
}

void gameThing::ModArmor( float mod )
{
	Armor += mod;
	if( Armor > MaxArmor )
		Armor = MaxArmor;
	if( Armor < 0.0f )
		Armor = 0.0f;
}

void gameThing::SetArmor( float set )
{
	Armor = set;
}

void gameThing::SetStartPos( Vector3f set )
{
	StartPos = set;
}

Vector3f gameThing::GetStartPos()
{
	return StartPos;
}

unsigned int gameThing::GetType()
{
	return Type;
}

void gameThing::Reset()
{
	Pos = StartPos;
	NextPos = StartPos;
	Angle = StartAngle;
	Health = MaxHealth;
	//Armor = MaxArmor;
	Dying = false;
	Dead = false;
	Enemy = NULL;
	HasTarget = false;
}

void gameThing::Free()
{

}



/*=========================	
	KLASA gamePlayer


=========================*/
gamePlayer::gamePlayer()
{
	run = true;
	Angle = 0.0f;
	WalkStep = 1.1f;
	RunStep = 1.4f;
	R = 3.0f;
	Pos.Set( 5.0f, 0.0f, -5.0f );
	AIflags = AI_NO_AI;
	Type = GAME_THING_PLAYER;

	Health = 100.0f;
	MaxHealth = 100.0f;
	Armor = 0.0f;
	MaxArmor = 150.0f;
	CurrWeap = GAME_WEAP_PHAZER;
	Weapon[GAME_WEAP_SAW] = new weSaw();
	Weapon[GAME_WEAP_PISTOL] = new wePistol();
	Weapon[GAME_WEAP_MINIPZR] = new weMiniPhazer();
	Weapon[GAME_WEAP_MINIGUN] = new weMiniGun();
	Weapon[GAME_WEAP_ROCKETLUN] = new weRocketLuncher();
	Weapon[GAME_WEAP_PHAZER] = new wePhazer();
	Weapon[GAME_WEAP_ATOM_BOMB] = new weAtomBomb();
}

gamePlayer::~gamePlayer()
{
	delete Weapon[GAME_WEAP_SAW];
	delete Weapon[GAME_WEAP_PISTOL];
	delete Weapon[GAME_WEAP_MINIPZR];
	delete Weapon[GAME_WEAP_MINIGUN];
	delete Weapon[GAME_WEAP_ROCKETLUN];
	delete Weapon[GAME_WEAP_PHAZER];
	delete Weapon[GAME_WEAP_ATOM_BOMB];
}

void gamePlayer::ApplyNextPos()
{
	Pos = NextPos;
}

void gamePlayer::DoDraw()
{
	if( Weapon[CurrWeap]->GetHave() )
		Weapon[CurrWeap]->DoDraw();
}

void gamePlayer::DoEngine( const float fTD )
{

}

void gamePlayer::DoEngine( bool* Keys, const float fTD )
{
	Actions = 0;
	if( Keys['W'] || Keys[VK_UP] )
	{
		Actions |= GAME_MOVE_FORWARD;
	}
	if( Keys['S'] || Keys[VK_DOWN] )
	{
		Actions |= GAME_MOVE_BACK;
	}
	if( Keys[VK_LEFT] )
	{
		ModAngle( -1.5f );
	}
	if( Keys[VK_RIGHT] )
	{
		ModAngle( 1.5f );
	}
	if( Keys['A'] )
	{
		Actions |= GAME_MOVE_STRAFE_L;
	}
	if( Keys['D'] )
	{
		Actions |= GAME_MOVE_STRAFE_R;
	}
	if( Keys[VK_LBUTTON] || Keys[VK_CONTROL] )
	{
		Actions |= GAME_DO_FIREWEAPON;
	}
	if( Keys['0'] )
	{
		SwichWeap( 0 );
		Keys['0'] = false;
	}
	if( Keys['1'] )
	{
		SwichWeap( 1 );
		Keys['1'] = false;
	}
	if( Keys['3'] )
	{
		SwichWeap( 3 );
		Keys['3'] = false;
	}
	if( Keys['4'] )
	{
		SwichWeap( 4 );
		Keys['4'] = false;
	}
	if( Keys['5'] )
	{
		SwichWeap( 5 );
		Keys['5'] = false;
	}
	if( Keys['7'] )
	{
		SwichWeap( 7 );
		Keys['7'] = false;
	}
	if( Keys['9'] )
	{
		SwichWeap( 9 );
		Keys['9'] = false;
	}
	if( run )
		Speed = RunStep;
	else Speed = WalkStep;

	Move( Actions, fTD );

	float an = Angle;
	switch( GUI.GetHandPos() )
	{
	case GAME_RIGHT_HAND :
		an += 50.0f;
		break;
	case GAME_LEFT_HAND :
		an -= 50.0f;
		break;
	case GAME_CENTER_HAND :
	default:
		an = 0.0f;			
	}
	Vector3f temp;
	temp.X = sinf( an * PIOVER180 ) * 1.2f;
	temp.Z = -cosf( an * PIOVER180 ) * 1.2f;
	temp.Y = -1.0f;

	GUI.PInfo.WeapName = "---";
	if( Weapon[CurrWeap]->GetHave() )
	{
		Weapon[CurrWeap]->Pos = Pos + temp;
		Weapon[CurrWeap]->DoEngine();
		GUI.PInfo.AMMO = Weapon[CurrWeap]->GetAmmo();
		GUI.PInfo.CLIPS = Weapon[CurrWeap]->GetClip();
		GUI.PInfo.WeapName = Weapon[CurrWeap]->Name;
	}
	TestCollBlock( this, GLevel.GetBlock( this->GetBlockPos() ), true );
	ApplyNextPos();
}

unsigned int gamePlayer::GetHand()
{
	return Hand;
}

void gamePlayer::TestWeapon( weWeapon* Weap )
{
	if( mathDistSq( Pos, Weap->Pos ) <= POW(R + Weap->R) )
	{
		if( !Weapon[Weap->GetType()]->GetInited() )
			this->SwichWeap( Weap->GetType() );

		Weapon[Weap->GetType()]->PickUp( Weap, this );
	}
}

void gamePlayer::TestBonus( weBonus* Bonus )
{
	if( mathDistSq( Bonus->Pos, this->NextPos ) < POW( R ) )
	{
		switch( Bonus->GetType() )
		{
		case BONUS_TYPE_AMMO :
			if( Weapon[((weAmmo*)Bonus)->GetWeapType()]->ModAmmo( ((weAmmo*)Bonus)->GetAmmoCount() ) )
			{
				GUI.SendMsg( "Podniosles: " + guiIntToStr( ((weAmmo*)Bonus)->GetAmmoCount() ) + " Amunicji", 4000, 10.0f, -1.0f, 1.5f, 1.5f, 0.5f, 0.5f, 0.5f );
				Bonus->CanDelete = true;
			}
			break;
		case BONUS_TYPE_HEALTH :
			if( Health != MaxHealth )
			{
				GUI.SendMsg( "Podniosles: " + guiFloatToStr( ((weHealth*)Bonus)->GetHealth() ) + " Zdrowia", 4000, 10.0f, -1.0f, 1.5f, 1.5f, 1.0f, 0.3f, 0.3f );
				ModHealth( ((weHealth*)Bonus)->GetHealth() );
				Bonus->CanDelete = true;
			}
			break;
		case BONUS_TYPE_ARMOR :
			if( Armor != MaxArmor )
			{
				GUI.SendMsg( "Podniosles: " + guiFloatToStr( ((weArmor*)Bonus)->GetArmor() ) + " Pancerza", 4000, 10.0f, -1.0f, 1.5f, 1.5f, 1.0f, 0.3f, 0.3f );
				ModArmor( ((weArmor*)Bonus)->GetArmor() );
				Bonus->CanDelete = true;
			}
			break;
		}
	}
}
void gamePlayer::SwichWeap( unsigned int index )
{
	CurrWeap = index;
}

void gamePlayer::ModHealth( float mod )
{
	if( mod < 0.0f )
	{
		GUI.ActiveFScrColor( 1.0f, 0.0f, 0.0f, 0.25f );
		ModArmor( mod * 1.5f );
	}
	else if( mod > 0.0f )
	{
		GUI.ActiveFScrColor( 1.0f, 1.0f, 0.0f );
	}
	
	gameThing::ModHealth( mod );
	if( Health <= 0 )
		Dead = true;
}

void gamePlayer::Reset()
{
	gameThing::Reset();
	for( int i = 0; i < 10; i++ )
	{
		delete Weapon[i];
	}
	Weapon[GAME_WEAP_SAW] = new weSaw();
	Weapon[GAME_WEAP_PISTOL] = new wePistol();
	Weapon[GAME_WEAP_MINIPZR] = new weMiniPhazer();
	Weapon[GAME_WEAP_MINIGUN] = new weMiniGun();
	Weapon[GAME_WEAP_ROCKETLUN] = new weRocketLuncher();
	Weapon[GAME_WEAP_PHAZER] = new wePhazer();
	Weapon[GAME_WEAP_ATOM_BOMB] = new weAtomBomb();
	GUI.PInfo.FRAGS = 0;
}

gamePlayer MainPlayer;


/*=========================
	KLASA gameEnemy


=========================*/
std::string gameEnemy::GetStr( FILE* fp )
{
	char buf[256];
	std::string zwrot;

	fgets( buf, 256, fp );

	zwrot = buf;

	if( zwrot[zwrot.length()-1] == '\n' )
		zwrot = zwrot.substr( 0, zwrot.length()-1 );

	return  zwrot;
}

bool gameEnemy::LoadEnemy( std::string filename )
{
	// Sprawdzamy, czy argument nie jest pusty
	if( filename == "" )
	{
		Log.Error( "ENEMY( " + file + " ): Ci¹g znaków jest pusty" );
		return false;
	}

	// Otwieramy plik
	FILE* fp = 0;
	fopen_s( &fp, filename.c_str(), "rt" );

	// zmienna pomocnicza
	std::string str;

	// Sprawdzamy czy otwarcie siê uda³o
	if( !fp )
	{
		Log.Error( "ENEMY( " + file + " ): Niew³aœciwa œcie¿ka, lub plik nie istnieje" );
		return false;
	}

	if( loaded )
	{
		Log.Log( "ENEMY( " + file + " ): Prze³adowanie na: " + filename );
		Model = NULL;
	}

	// Przypisujemy nazwê pliku
	file = filename;

	// przypisujemy identyfikator
	str = GetStr( fp );
	ID = str;

	// przypisujemy nazwê wroga
	str = GetStr( fp );
	name = str;

	// przypisujemy, która czêœæ modelu ma siê obracaæ
	str = GetStr( fp );
	RotObj = atoi( str.c_str() );

	// przypisujemy promieñ kolizyjny
	str = GetStr( fp );
	R = atof( str.c_str() );
	
	// przypisujemy typ broni
	str = GetStr( fp );
	WeapType = atoi( str.c_str() );

	// przypisujemy korekcje pozycji broni
	str = GetStr( fp );
	WeapR = atof( str.c_str() );
	str = GetStr( fp );
	WeapY = atof( str.c_str() );
	str = GetStr( fp );
	WeapAngle = atof( str.c_str() );

	// przypisujemy statystyki
	str = GetStr( fp );
	Health = atof( str.c_str() );
	str = GetStr( fp );
	Armor = atof( str.c_str() );

	// przypisujemy AI
	str = GetStr( fp );
	AIflags = atoi( str.c_str() );

	// przypisujemy model
	str = GetStr( fp );
	Model = GLMManager.Get( str );

	str = GetStr( fp );
	FireTime = FirePause = atof( str.c_str() );

	fclose( fp );
	loaded = true;

	return true;
}

void gameEnemy::DoDraw()
{
	if( Dead )
		return;

	unsigned int i;
	glPushMatrix();
	if( Dying )
	{
		glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
		glEnable( GL_BLEND );
		glColor4f( 1.0f, 1.0f, 1.0f, 1.0f - ( ThisDTime * ( 1.0f / DyingTime ) ) );
	}

	glTranslatef( Pos.X, Pos.Y, Pos.Z );

	for( i = 0; i < Model->GetObjCount(); i++ )
	{
		if( i == RotObj )
		{
			glPushMatrix();
			glRotatef( Angle, 0.0f, 1.0f, 0.0f );
		}

		Model->CallObject( i );

		if( i == RotObj )
			glPopMatrix();
	}

	if( GUI.GetReflection() )
	{
		glPushMatrix();
		glTranslatef( 0.0f, -10.0f, 0.0f );
		glScalef( 1.0f, -1.0f, 1.0f );
		glDisable( GL_CLIP_PLANE0 );
		glEnable( GL_CLIP_PLANE1 );

		for( i = 0; i < Model->GetObjCount(); i++ )
		{
			if( i == RotObj )
			{
				glPushMatrix();
				glRotatef( Angle, 0.0f, 1.0f, 0.0f );
			}

			Model->CallObject( i );

			if( i == RotObj )
				glPopMatrix();
		}

		glDisable( GL_CLIP_PLANE1 );
		glEnable( GL_CLIP_PLANE0 );
		glPopMatrix();
	}
	if( Dying )
	{
		glDisable( GL_BLEND );
		glColor4f( 1.0f, 1.0f, 1.0f, 1.0f );
	}


	glPopMatrix();

	if( !GUI.GetShowAITarget() )
		return;

	GLUquadric* obj = gluNewQuadric();
	glPushMatrix();
	glTranslatef( Target.X, Target.Y, Target.Z );

	gluSphere( obj, 0.5f, 32, 32 );

	glPopMatrix();
	gluDeleteQuadric( obj );
}

void gameEnemy::Fire( Vector3f FireTarget )
{
	if( Dying )
		return;

	FireTime += 1.0f * GUI.GetSpeed();

	if( FireTime < FirePause )
		return;
	else FireTime = 0.0f;

	Vector3f temp = Pos;
	Vector3f add;
	Vector3f FireVeloc;

	add.X = sinf( ( Angle + WeapAngle ) * PIOVER180 ) * WeapR;
	add.Z = -cosf( ( Angle + WeapAngle ) * PIOVER180 ) * WeapR;
	add.Y = WeapY;

	temp += add;

	FireVeloc = FireTarget - temp;
	FireVeloc.Normalize();

	switch( WeapType )
	{
	case GAME_WEAP_PISTOL :
		{
			weBullet* Bull = new weBullet();
			Bull->Init( temp, FireVeloc, 1.0f );
			Bull->Owner = this;
			Bull->Damage = 1.0f;
			BManager.AddBullet( Bull );
			return;
		}
	case GAME_WEAP_MINIPZR:
	case GAME_WEAP_PHAZER :
		{
			weBullet* Bull = new weBullRay();
			Bull->Init( temp, FireVeloc, 0 );
			Bull->Owner = this;
			Bull->Damage = 50.0f;
			BManager.AddBullet( Bull );
			return;
		}
	case GAME_WEAP_ROCKETLUN :
		{
			weBullet* Bull = new weBullRocket();
			Bull->Init( temp, FireVeloc, 0.2f );
			Bull->Owner = this;
			Bull->Damage = 10.0f;
			BManager.AddBullet( Bull );
			return;
		}
	}
}

void gameEnemy::OnDie()
{
	specExplode* spec = new specExplode;
	spec->Create( Pos, 5.0f, 0.1f );
	SEManager.AddEffect( spec );
}

void gameEnemy::OnDead()
{
	GUI.PInfo.FRAGS++;
}

std::string gameEnemy::GetID()
{
	return ID;
}

/*======================
	KLASA gameStatObj

======================*/
std::string gameStatObj::GetStr( FILE* fp )
{
	char buf[512];
	fgets( buf, 512, fp );
	if( buf[strlen( buf )-1] == '\n' ) 
		buf[strlen( buf )-1] = 0;
	return buf;
}

void gameStatObj::DoEngine( const float fTD )
{
	AIflags = AI_NO_AI;
	TestCollBlock( this, GLevel.GetBlock( this->GetBlockPos() ), true );
}

void gameStatObj::DoDraw()
{
	if( !Model )
		return;

	glPushMatrix();
	glTranslatef( Pos.X, Pos.Y, Pos.Z );
	glRotatef( Angle, 0.0f, 1.0f, 0.0f );

	for( unsigned int i = 0; i < Model->GetObjCount(); i++ )
		Model->CallObject( i );

	glPopMatrix();
}

bool gameStatObj::LoadObj( std::string filename )
{
	if( filename == "" )
	{
		Log.Error( "STATOBJ( " + file + " ): Pusty ci¹g znaków!" );
		return false;
	}

	FILE* fp = 0;
	fopen_s( &fp, filename.c_str(), "rt" );
	std::string str;

	if( !fp )
	{
		Log.Error( "STATOBJ( " + file + " ): Nieprawid³owa œcie¿ka, lub plik nie istnieje: " + filename );
		return false;
	}

	str = GetStr( fp );
	Model = GLMManager.Get( str );
	if( !Model )
	{
		Log.Error( "STATOBJ( " + file + " ): Nieudane wczytanie modelu: " + str );
		return false;
	}

	str = GetStr( fp );
	R = atof( GetStr( fp ).c_str() );

	fclose( fp );
	return true;
}



gameThingManager::gameThingManager()
{
	
}

gameThingManager::~gameThingManager()
{
	Clear();
}

void gameThingManager::AddThing( gameThing* Thing )
{
	List.push_back( Thing );
}

void gameThingManager::DeleteThing( unsigned int index )
{
	if( index >= List.size() )
		return;

	delete List[index];
	List.erase( List.begin() + index );
}

gameThing* gameThingManager::GetThing( unsigned int index )
{
	if( index >= List.size() )
		return NULL;

	return List[index];
}

gameEnemy* gameThingManager::GetEnemyByID( std::string ID )
{
	gameEnemy* Enemy;
	for( unsigned int i = 0; i < List.size(); i++ )
	{
		if( GetThing( i )->GetType() != GAME_THING_ENEMY )
			continue;

		Enemy = (gameEnemy*)GetThing( i );
		if( Enemy->GetID() == ID )
			return Enemy;
	}
	return NULL;
}

Vector3f gameThingManager::GetThingPos( unsigned int index )
{
	if( index >= List.size() )
		return Vector3f( 0.0f, 0.0f, 0.0f );

	return List[index]->NextPos;	
}

Vector3f gameThingManager::GetThingBlockPos( unsigned int index )
{
	if( index >= List.size() )
		return Vector3f( 0.0f, 0.0f, 0.0f );

	return List[index]->GetBlockPos();	
}

gameBlockInfo* gameThingManager::GetThingBlock( unsigned int index )
{
	if( index >= List.size() )
		return NULL;

	return GLevel.GetBlock( List[index]->GetBlockPos() );
}

void gameThingManager::DoEngine( const float fTD )
{
	gameThing* Thing;
	this->all = List.size();
	this->dead = 0;
	this->life = 0;
	unsigned int i = 0;

	for( i = 0; i < List.size(); i++ )
	{
		Thing = GetThing( i );

		if( Thing->IsDead() )
		{
			this->dead++;
			continue;
		}

		if( mathDistSq( MainPlayer.Pos, Thing->Pos ) > POW(100.0f) )
			continue;

		Thing->ModHealth( -BManager.DoTest( Thing, Thing->GetArmor() ) );

		Thing->DoEngine( fTD );

		if( Thing->GetType() != GAME_THING_PLAYER )
			this->life++;
		else this->all--;
	}
}

void gameThingManager::DoDraw()
{
	gameThing* Thing;
	unsigned int i;

	for( i = 0; i < List.size(); i++ )
	{
		Thing = GetThing( i );

		if( mathDistSq( MainPlayer.Pos, Thing->Pos ) > POW(100.0f) )
			continue;

		Thing->DoDraw();
	}
}

void gameThingManager::ResetAll()
{
	unsigned int i;
	gameThing* Thing;

	for( i = 0; i < List.size(); i++ )
	{
		Thing = GetThing( i );
		Thing->Reset();
	}
}

void gameThingManager::ReCountStats()
{
	this->all = List.size();
	this->dead = 0;
	this->life = 0;
	gameThing* Thing;
	unsigned int i;

	for( i = 0; i < List.size(); i++ )
	{
		Thing = GetThing( i );
		if( Thing->IsDead() )
			this->dead++;
		else if( Thing->GetType() != GAME_THING_PLAYER )
			this->life++;
		else this->all--;
	}
}

unsigned int gameThingManager::Count()
{
	return List.size();
}

unsigned int gameThingManager::GetAllCount()
{
	return all;
}

unsigned int gameThingManager::GetLifeCount()
{
	return life;
}

unsigned int gameThingManager::GetDeadCount()
{
	return dead;
}

void gameThingManager::Clear()
{
	int i;

	for( i = List.size()-1; i >= 0; i-- )
	{
		if( GetThing( i )->GetType() == GAME_THING_PLAYER )
			continue;

		DeleteThing( i );
	}
}

gameThingManager ThingManager;

/*=========================	
	KLASA gameWeaponManager


=========================*/
gameWeaponManager::gameWeaponManager()
{

}

gameWeaponManager::~gameWeaponManager()
{
	Clear();
}

void gameWeaponManager::DoEngine( gamePlayer* Players, int PlayerCount )
{
	unsigned int i;
	int j;
	for( i = 0; i < List.size(); i++ )
	{
		List[i]->DoEngine();
	}
	for( i = 0; i < PlayerCount; i++ )
	{
		for( j = List.size()-1; j >= 0; j-- )
		{
			Players[i].TestWeapon( List[j] );
			if( List[j]->GetAllAmmo() == 0 )
			{
				DeleteWeapon( j );
			}
		}
	}
}

void gameWeaponManager::DoDraw()
{
	unsigned int i;
	for( i = 0; i < List.size(); i++ )
	{
		List[i]->DoDraw();
	}
	if( GUI.GetReflection() )
	{
		glPushMatrix();
		glTranslatef( 0.0f, -10.0f, 0.0f );
		glScalef( 1.0f, -1.0f, 1.0f );
		glDisable( GL_CLIP_PLANE0 );
		glEnable( GL_CLIP_PLANE1 );
		glFrontFace( GL_CW );
		for( i = 0; i < List.size(); i++ )
		{
			List[i]->DoDraw();
		}
		glFrontFace( GL_CCW );
		glDisable( GL_CLIP_PLANE1 );
		glEnable( GL_CLIP_PLANE0 );
		glPopMatrix();
	}
}

void gameWeaponManager::AddWeapon( weWeapon* weapon )
{
	if( weapon == NULL )
		return;

	if( !weapon->GetInited() )
		weapon->Init();

	List.push_back( weapon );
}

void gameWeaponManager::DeleteWeapon( unsigned int index )
{
	if( index >= List.size() )
		return;

	delete List[index];
	List.erase( List.begin() + index );
}

weWeapon* gameWeaponManager::GetWeapon( unsigned int index )
{
	if( index >= List.size() )
		return NULL;

	return List[index];
}

void gameWeaponManager::Clear()
{
	for( unsigned int i = 0; i < List.size(); i++ )
	{
		delete List[i];
	}
	List.clear();
}

void gameWeaponManager::LoadFromLevel()
{
}

gameWeaponManager WManager;
