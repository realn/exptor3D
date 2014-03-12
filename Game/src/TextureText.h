#pragma once

#include "Texture.h"
#include "3dMath.h"

class CTextRenderer
{
private:
	CTexture *Tex;

	unsigned int base;

	float C[4];
public:
	CTextRenderer( CTexManager& texManager );
	~CTextRenderer();

	void SetColor( float R, float G, float B, float Alpha = 1.0f );
	void StartPrint( const float width, const float height );
	void EndPrint();
	void Print( float x, float y, std::string text, float ScaleX = 1.0f, float ScaleY = 1.0f );

	const Vector2f	GetTextSize( const std::string& text ) const;
};
