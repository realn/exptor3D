#pragma once

enum class EVENT_INPUT_TYPE : unsigned
{
	MOUSEMOVE,

	KEYDOWN,
	KEYUP,

	CHARPRESS,
};


class CEventKey
{
public:
	EVENT_INPUT_TYPE	Type;
	unsigned			Key;

	CEventKey();
	CEventKey( const EVENT_INPUT_TYPE type, const unsigned key ) :
		Type( type ), Key( key )
	{}
};

class CEventChar
{
public:
	EVENT_INPUT_TYPE	Type;
	char	Character;
	wchar_t	WideCharacter;

	CEventChar();
	CEventChar( const EVENT_INPUT_TYPE type, const char character, const wchar_t wideCharacter ) :
		Type( type ), Character( character ), WideCharacter( wideCharacter )
	{}
};

class CEventMouse
{
public:
	EVENT_INPUT_TYPE	Type;
	int	X;
	int	Y;

	CEventMouse();
	CEventMouse( const EVENT_INPUT_TYPE type, const int x, const int y ) :
		Type( type ), X( x ), Y( y )
	{}
};