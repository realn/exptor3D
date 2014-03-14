#pragma once

#include "Texture.h"
#include "3dMath.h"

class CTextRenderer
{
private:
	CTexture *Tex;
	unsigned int base;
	Vector4f	Color;

public:
	CTextRenderer( CTexManager& texManager );
	~CTextRenderer();

	void	SetColor( const float R, const float G, const float B, const float Alpha = 1.0f );
	void	SetColor( const Vector3f& color );
	void	SetColor( const Vector4f& color );
	void	StartPrint( const float width, const float height );
	void	EndPrint();
	void	Print( const float x, const float y, const std::string& text, const float ScaleX = 1.0f, const float ScaleY = 1.0f );
	void	Print( const Vector2f& pos, const std::string& text, const Vector2f& scale = Vector2f( 1.0f, 1.0f ) );

	const Vector2f	GetTextSize( const std::string& text ) const;
};
