#pragma once

#include "TextureText.h"

class CGUIScreen
{
private:
	CTextRenderer&	TextRender;

public:
	CGUIScreen( CTextRenderer& textRender, const float height, const float aspectRatio );
	~CGUIScreen();
};