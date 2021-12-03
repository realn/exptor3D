#pragma once

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

#include "GUITextPrinter.h"
#include "GUIRenderContext.h"

enum class SCREEN_ELEMENT_TYPE
{
	UNKNOWN = 0,
	TEXT,
	BAR,
};

enum class ELEMENT_HALIGN
{
	LEFT,
	CENTER,
	RIGHT,
};

enum class ELEMENT_VALIGN
{
	TOP,
	CENTER,
	BOTTOM,
};

class CGUIElement
{
protected:
	SCREEN_ELEMENT_TYPE	Type;
	ELEMENT_HALIGN	AlignH;
	ELEMENT_VALIGN	AlignV;
	glm::vec2	Margin = glm::vec2(0.0f);
	glm::vec2	Scale = glm::vec2(1.0f);
	glm::vec4	Color = glm::vec4(1.0f);

protected:
	CGUIElement( const SCREEN_ELEMENT_TYPE type );

public:
	virtual ~CGUIElement();

	virtual void	Render(gui::RenderContext& ctx, gui::TextPrinter& printer, const glm::vec2& screenSize) = 0;
	virtual void	Update( const float fTD ) = 0;

	void	SetAlign( const ELEMENT_HALIGN alignH, const ELEMENT_VALIGN alignV );
	void	SetMargin( const glm::vec2& margin );
	void	SetScale( const glm::vec2& scale );
	void	SetColor( const glm::vec4& color );

	const SCREEN_ELEMENT_TYPE	GetType() const;
	const ELEMENT_HALIGN	GetAlignH() const;
	const ELEMENT_VALIGN	GetAlignV() const;
	const glm::vec2	GetMargin() const;
	const glm::vec2	GetScale() const;
	virtual const glm::vec2	GetSize() const = 0;

protected:
	const glm::vec2	CreatePos( const glm::vec2& size, const glm::vec2& screenSize ) const;
};

class CGUITextElement :
	public CGUIElement
{
protected:
	std::string		Text;
	glm::vec2 textSize;

public:
	CGUITextElement();
	virtual ~CGUITextElement();

	virtual void	Render(gui::RenderContext& ctx, gui::TextPrinter& printer, const glm::vec2& screenSize) override;
	virtual void	Update( const float fTD ) override;

	void	SetText( const std::string& text );

	const glm::vec2		GetSize() const override;
	const std::string	GetText() const;
};

class CValueSync
{
protected:
	CGUIElement*	Element;
	std::string		VarName;
	std::string		Prefix;
	std::string		Sufix;

public:
	CValueSync( CGUIElement* element, const std::string& name, const std::string& prefix = "", const std::string& sufix = "" );

	const std::string	GetVarName() const;

	void	OnValueChange( const std::string& value );
};

class CGUIScreen
{
private:
	std::vector<CGUIElement*>	ElementList;
	std::vector<CValueSync>		SyncList;
	glm::vec2	Margin;

public:
	CGUIScreen();
	~CGUIScreen();

	void	Render(gui::RenderContext& ctx, gui::TextPrinter& printer, const glm::vec2& screenSize );
	void	Update( const float fTD );

	void	SetMargin( const glm::vec2& margin );

	CGUITextElement*	AddTextElement( const std::string& text = "" );

	void	AddValueSync( CGUIElement* pElement, const std::string& valueName, const std::string& prefix = "", const std::string& sufix = "" ); 

	const bool	IsVarMonitored( const std::string& name ) const;
	void	OnVarChanged( const std::string& name, const std::string& value );

	const bool	Load( const std::string& filename );
};

extern const ELEMENT_HALIGN			GetElementAlignH( const std::string& align );
extern const ELEMENT_VALIGN			GetElementAlignV( const std::string& align );