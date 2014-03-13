#pragma once

#include "TextureText.h"

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
	Vector2f	Margin;

protected:
	CGUIElement( const SCREEN_ELEMENT_TYPE type, const ELEMENT_HALIGN alignH, const ELEMENT_VALIGN alignV, const Vector2f& margin );

public:
	virtual ~CGUIElement();

	const SCREEN_ELEMENT_TYPE	GetType() const;
	const ELEMENT_HALIGN	GetAlignH() const;
	const ELEMENT_VALIGN	GetAlignV() const;

	virtual void	Render( const Vector2f& screenSize ) = 0;
	virtual void	Update( const float fTD ) = 0;

	const Vector2f	GetMargin() const;
	virtual const Vector2f	GetSize() const = 0;

	virtual void	SetScale( const Vector2f& scale ) = 0;
	virtual void	SetColor( const float R, const float G, const float B, const float Alpha = 1.0f ) = 0;

protected:
	const Vector2f	CreatePos( const Vector2f& size, const Vector2f& screenSize ) const;
};

class CGUITextElement :
	public CGUIElement
{
protected:
	CTextRenderer&	TextRender;
	std::string		Text;
	Vector2f		Scale;
	float			Color[4];

public:
	CGUITextElement( const ELEMENT_HALIGN alignH, const ELEMENT_VALIGN alignV, const Vector2f& margin, CTextRenderer& textRender );
	virtual ~CGUITextElement();

	virtual void	Render( const Vector2f& screenSize ) override;
	virtual void	Update( const float fTD ) override;

	const Vector2f	GetSize() const override;

	void	SetText( const std::string& text );
	void	SetScale( const Vector2f& scale ) override;
	void	SetColor( const float R, const float G, const float B, const float Alpha = 1.0f ) override;

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
	CTextRenderer&	TextRender;
	std::vector<CGUIElement*>	ElementList;
	std::vector<CValueSync>		SyncList;
	Vector2f	Margin;

public:
	CGUIScreen( CTextRenderer& textRender );
	~CGUIScreen();

	void	Render( const Vector2f& screenSize );
	void	Update( const float fTD );

	void	SetMargin( const Vector2f& margin );

	CGUITextElement*	AddTextElement( const std::string& text, const ELEMENT_HALIGN alignH, const ELEMENT_VALIGN alignV, const Vector2f& margin );

	void	AddValueSync( CGUIElement* pElement, const std::string& valueName, const std::string& prefix = "", const std::string& sufix = "" ); 

	const bool	IsVarMonitored( const std::string& name ) const;
	void	OnVarChanged( const std::string& name, const std::string& value );

	const bool	Load( const std::string& filename );
};

extern const ELEMENT_HALIGN			GetElementAlignH( const std::string& align );
extern const ELEMENT_VALIGN			GetElementAlignV( const std::string& align );