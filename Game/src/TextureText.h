#pragma once

#include "Texture.h"

class CTextRenderer
{
private:
	CTexture *Tex;

	unsigned int base;
	unsigned int list;

	unsigned int Height;
	unsigned int Width;

	float C[4];

	bool loaded;
public:
	CTextRenderer();
	~CTextRenderer();

	void Init( CTexture *font );
	void Free();
	void SetSize( unsigned int wid, unsigned int hei );
	void SetColor( float R, float G, float B, float Alpha = 1.0f );
	void StartPrint( const float aspect = 4.0f/3.0f );
	void EndPrint();
	void Print( float x, float y, std::string text, float ScaleX = 1.0f, float ScaleY = 1.0f );
};
