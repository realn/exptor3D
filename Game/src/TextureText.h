#pragma once

#include "Texture.h"

class guiTextureText
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
	guiTextureText();
	~guiTextureText();

	void Init( CTexture *font );
	void Free();
	void SetSize( unsigned int wid, unsigned int hei );
	void SetColor( float R, float G, float B, float Alpha = 1.0f );
	void StartPrint();
	void EndPrint();
	void Print( float x, float y, std::string text, float ScaleX = 1.0f, float ScaleY = 1.0f );
};
