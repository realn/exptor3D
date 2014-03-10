/*///////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
Plik:	Special.h
Autor:	Real_Noname (real_noname@wp.pl)
(C):	CODE RULERS (Real_Noname)
WWW:	www.coderulers.prv.pl
Opis:	Zawiera definicje klas odpowiedzialnych za efekty specjalne
		typu wybuchy, promienie, czy sprite'y.

/////////////////////////////////////////////////////////
///////////////////////////////////////////////////////*/
#ifndef _SPECIAL_H_
#define _SPECIAL_H_

#include "gui.h"
#include "3dMath.h"
#include "Level.h"
#include "vector"

#include "Effect.h"

//class CSpecialEffectManager
//{
//private:
//	CTexManager*	TexManager;
//	std::vector<CEffect*> List;
//
//public:
//	int MaxSpec;
//
//	CSpecialEffectManager();
//	virtual ~CSpecialEffectManager();
//	
//	void Init( CTexManager& texManager );
//	CTexManager&	GetTexMng();
//
//	void AddEffect( CEffect* effect );
//	void DeleteEffect( unsigned int index );
//	CEffect* GetEffect( unsigned int index );
//	
//	void Update( const float fTD );
//	void Render();
//};

class specMotionBlur
{
private:
	unsigned int texture[5];
	bool inited;
	bool CanCopy;
	unsigned short CurTex;
	unsigned short tick;
	unsigned int list;
public:
	specMotionBlur();
	~specMotionBlur();
	void Init();
	void CopyImage();
	void Update( const float fTD );
	void Render();
	void Free();
};

//extern CSpecialEffectManager SEManager;
extern specMotionBlur SMBlur;

#endif