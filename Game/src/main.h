/*--------------------------------------------
  Nazwa:    Nag³ówek Main.h
  Autor:    Real_Noname (real_noname@wp.pl)
  WWW:      www.coderulers.prv.pl
  Data:		11-01-2005 18:21
  Opis:     Obiekty i nag³ówki potrzebny do uruchomienia programu
			w funkcji WinMain. Patrz -> Main.cpp
--------------------------------------------*/
#ifndef _MAIN_H_
#define _MAIN_H_

#include	"defines.h"
#include    <windows.h>     //Biblioteka funkcji Windows (WinApi)
#include    <GL/gl.h>       //Biblioteka OpenGL
#include    <GL/glu.h>      //Biblioteka Pomocnicza OpenGL
#include	<math.h>
#include	"Log.h"
#include	"Render.h"
#include	"Level.h"
#include	"Game.h"
#include	"Weapon.h"
#include	"gui.h"
#include	"Special.h"
#include	"glm.h"
#include	"inifile.h"

HINSTANCE   hInstance;      //Uchwyt instancji
ioTexture font[1];

GLModel*	MenuModel;

bool    Keys[255];           //Tablica bool od klawiatury
bool    fullscreen = true;   //Bool od pe³nego ekranu
bool    active=true;          

void Mouse();
void LoadLevel( std::string filename );

#endif