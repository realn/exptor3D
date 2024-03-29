#pragma once

#include "logic_ScriptParser.h"

#include "GUITextPrinter.h"
#include "GUIRenderContext.h"

class CGUIConsole
{
private:

	std::shared_ptr<logic::ScriptParser>	ScriptParser;
	unsigned	ScreenHeight;
	float		AspectRatio;
	float		Scroll;
	bool		Visible;
	std::vector<std::string>	TextLog;
	std::string	CurrentText;

public:
	CGUIConsole( std::shared_ptr<logic::ScriptParser> scriptParser, const unsigned height, const float aspectRatio );

	void	Render(gui::RenderContext& ctx, gui::TextPrinter& printer);
	void	Update( const float fTD );

	void	SetVisible( const bool set, const bool animate );
	const bool	IsVisible() const;
	const bool	IsAnimating() const;

	void	ParseKey( const unsigned uKey, const bool down );
	void	ParseChar( const char Character );

	void	Print( const std::string& msg );
};