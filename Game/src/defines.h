/*///////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
Plik:	defines.h
Autor:	Real_Noname (real_noname@wp.pl)
(C):	CODE RULERS (Real_Noname)
WWW:	www.coderulers.prv.pl
Opis:	Zawiera wiêkszoœæ definicji, makr oraz zapowiedzi klas.

/////////////////////////////////////////////////////////
///////////////////////////////////////////////////////*/
#ifndef _DEFINES_H_
#define _DEFINES_H_

/*	Jak ktoœ chce zobaczyæ testowe oœwietlenie
	to niech odkomentuje nastêpn¹ liniê.
*/
//#define LIGHT_TEST

#define GAME_VERSION 100

#define	GAME_RIGHT_HAND		0
#define	GAME_LEFT_HAND		1
#define	GAME_CENTER_HAND	2

#define GAME_SECOND			0
#define GAME_MINUTE			1
#define GAME_HOUR			2

#define MENU_ERROR			-1
#define MENU_NAME			0
#define MENU_ITEM_NAME		1
#define MENU_ITEM_POSX		2
#define MENU_ITEM_POSY		3
#define MENU_ITEM_ACTION	4
#define MENU_ITEM_GOTO		5

#define GAME_TEX_WALL		0
#define GAME_TEX_FLOOR		1
#define GAME_TEX_TOP		2

#define GAME_MOVE_FORWARD	1
#define GAME_MOVE_BACK		2
#define GAME_MOVE_STRAFE_R	4
#define	GAME_MOVE_STRAFE_L	8
#define GAME_DO_FIREWEAPON	16
#define GAME_DO_ATTACK		16

#define GAME_THING_PLAYER	0
#define GAME_THING_ENEMY	1
#define GAME_THING_OTHER	2

#define	GAME_WEAP_SAW		0
#define	GAME_WEAP_PISTOL	1
#define	GAME_WEAP_MINIPZR	3
#define	GAME_WEAP_MINIGUN	4
#define	GAME_WEAP_ROCKETLUN	5
#define	GAME_WEAP_PICKABOO	6
#define	GAME_WEAP_PHAZER	7
#define	GAME_WEAP_MINE		8
#define	GAME_WEAP_ATOM_BOMB	9

#define BULLET_TYPE_NORMAL	0
#define BULLET_TYPE_RAY		1
#define BULLET_TYPE_ROCKET	2
#define	BULLET_TYPE_EXPLODE	3
#define BULLET_TYPE_BOMB	4
#define BULLET_TYPE_SAW		5

#define BONUS_TYPE_UNKNOWN	0
#define BONUS_TYPE_AMMO		1
#define BONUS_TYPE_HEALTH	2
#define	BONUS_TYPE_ARMOR	3

#define WEAP_FLAG_RESPAWN	1
#define	WEAP_FLAG_AMMOPACK	2
#define	WEAP_FLAG_AUTODEL	4

#define	AI_NO_AI			0
#define AI_SIMPLE_AI		1
#define AI_PLAYER_ENEMY		2
#define AI_PLAYER_HELP		4
#define	AI_ATTACK_FAR		8
#define	AI_ATTACK_CLOSE		16
#define	AI_MOVE_AROUND		32
#define	AI_STAY_ON_ATTACK	64
#define	AI_MOVE_ON_ATTACK	128
#define AI_SEARCH_ENEMY		256
#define	AI_FIRE_AT_WILL		512
#define	AI_FIRE_IF_ATTACKED	1024
#define AI_TAKE_ITEMS		2048
#define	AI_DONT_TAKE_ITEMS	4096
#define	AI_ALLWAYS_WALK		8192
#define	AI_ALLWAYS_RUN		16384

#define AI_STATE_NOTHING	0
#define AI_STATE_STAND		1
#define AI_STATE_WALK		2
#define	AI_STATE_FIRE		3
#define	AI_STATE_WALKFIRE	4
#define	AI_STATE_STANDFIRE	5

class guiMain;

class gameThing;
class gamePlayer;
class gameEnemy;
class gameLevel;
class gameBlockInfo;
class gameWeaponManager;
class gameStatObj;
class gameThingManager;

class weWeapon;
class weSaw;
class weMiniPhazer;
class wePistol;
class weMinigun;
class wePhazer;

class weBullet;
class weBullRay;
class weBullRocket;
class weBullExplode;
class weBulletManager;

class weBonus;
class weAmmo;
class weHealth;
class weArmor;
class weBonusManager;

class specEffect;
class specRay;
class specExplode;
class specSprite;
class specManager;
class specMotionBlur;

class ioTexture;
class ioTexManager;

extern weBulletManager	BManager;
extern specManager		SEManager;
extern ioTexture		Part;
extern specMotionBlur	SMBlur;
extern guiMain			GUI;
extern gameWeaponManager WManager;
extern gameThingManager ThingManager;
extern weBonusManager	BonusMan;
extern bool	CanDoWLScr;

#endif