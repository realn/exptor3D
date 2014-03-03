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

class specEffect
{
public:
	bool Visible;
	bool CanDelete;
	specEffect();
	virtual void Update( const float fTD );
	virtual void Render();
};

class specRay : public specEffect
{
private:
	float Alpha;
public:
	Vector3f FromPos;
	Vector3f ToPos;

	void Create( Vector3f Pos, Vector3f Veloc );

	void Render();
	void Update( const float fTD ) override;
};

class specExplode : public specEffect
{
private:
	float toPower;
	float thisPower;
	float Step;
	float Alpha;	
public:
	Vector3f FromPos;

	void Create( Vector3f Pos, float Power, float fStep );

	void Render();
	void Update( const float fTD ) override;
};

class specSprite : public specEffect
{
private:
	float Alpha;
	ioTexture Tex;
	float C[3];
public:
	Vector3f AtPos;

	void Create( Vector3f Pos, float R = 1.0f, float G = 1.0f, float B = 1.0f );

	void Render();
	void Update( const float fTD ) override;
};

class specManager
{
private:
	std::vector<specEffect*> List;
public:
	int MaxSpec;
	
	void AddEffect( specEffect* effect );
	void DeleteEffect( unsigned int index );
	specEffect* GetEffect( unsigned int index );
	
	void Update( const float fTD );
	void Render();
};

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

extern specManager SEManager;
extern ioTexture Part;
extern specMotionBlur SMBlur;

#endif