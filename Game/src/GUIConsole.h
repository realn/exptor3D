#pragma once

#include "TextureText.h"
#include "ScriptParser.h"

class CGUIConsole
{
private:
	CScriptParser&	ScriptParser;
	CTextRenderer&	TextRender;
	unsigned	ScreenHeight;
	float		AspectRatio;
	float		Scroll;
	bool		Visible;
	std::vector<std::string>	TextLog;
	std::string	CurrentText;

public:
	CGUIConsole( CScriptParser& scriptParser, CTextRenderer& textRender, const unsigned height, const float aspectRatio );

	void	Render();
	void	Update( const float fTD );

	void	SetVisible( const bool set, const bool animate );
	const bool	IsVisible() const;
	const bool	IsAnimating() const;

	void	ParseKey( const unsigned uKey, const bool down );
	void	ParseChar( const char Character );

	void	Print( const std::string& msg );
};