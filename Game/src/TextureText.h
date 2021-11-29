#pragma once

#include <glm/vec4.hpp>

#include "Texture.h"
#include "3dMath.h"

class CTextRenderer
{
private:
	CTexture *Tex;
	unsigned int base;
	glm::vec4	Color;

public:
	CTextRenderer( CTexManager& texManager );
	~CTextRenderer();

	void	SetColor( const float R, const float G, const float B, const float Alpha = 1.0f );
	void	SetColor( const glm::vec3& color );
	void	SetColor( const glm::vec4& color );
	void	StartPrint( const float width, const float height );
	void	EndPrint();
	void	Print( const float x, const float y, const std::string& text, const float ScaleX = 1.0f, const float ScaleY = 1.0f );
	void	Print( const glm::vec2& pos, const std::string& text, const glm::vec2& scale = glm::vec2( 1.0f, 1.0f ) );

	const glm::vec2	GetTextSize( const std::string& text ) const;
};
