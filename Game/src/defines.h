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

#define WEAP_FLAG_RESPAWN	1
#define	WEAP_FLAG_AMMOPACK	2
#define	WEAP_FLAG_AUTODEL	4


class guiMain;

class CActor;
class CPlayer;
class CEnemy;
class gameLevel;
class gameBlockInfo;
class gameWeaponManager;
class gameStatObj;
class CActorManager;

class weWeapon;
class weSaw;
class weMiniPhazer;
class wePistol;
class weMinigun;
class wePhazer;

class CBullet;
class CBullRay;
class CBullRocket;
class CBullExplosion;
class CBulletManager;

class CItem;
class CItemAmmo;
class CItemHealth;
class CItemArmor;
class CItemManager;

class specEffect;
class specRay;
class specExplode;
class specSprite;
class specManager;
class specMotionBlur;

class CTexture;
class CTexManager;

extern CBulletManager	BManager;
extern gameLevel		GLevel;
extern gameLevel*		pGLevel;
extern CTexManager		TManager;
extern specManager		SEManager;
extern CTexture		Part;
extern specMotionBlur	SMBlur;
extern guiMain			GUI;
extern CPlayer		MainPlayer;
extern gameWeaponManager WManager;
extern CActorManager ThingManager;
extern CItemManager	BonusMan;
extern bool	CanDoWLScr;

#endif