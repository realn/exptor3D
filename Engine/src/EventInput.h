#pragma once

enum class EVENT_INPUT_TYPE : unsigned
{
	MOUSEMOVEABS = 0x10,
	MOUSEMOVEDIF,

	KEYDOWN,
	KEYUP,

	CHARPRESS,
};


class CEventKey
{
public:
	EVENT_INPUT_TYPE	Type = EVENT_INPUT_TYPE::KEYDOWN;
	unsigned			Key = 0;

	CEventKey() = default;
	CEventKey( const EVENT_INPUT_TYPE type, const unsigned key ) :
		Type( type ), Key( key )
	{}
};

class CEventChar
{
public:
	EVENT_INPUT_TYPE	Type = EVENT_INPUT_TYPE::CHARPRESS;
	char	Character = 0;
	wchar_t	WideCharacter = 0;

	CEventChar() = default;
	CEventChar( const EVENT_INPUT_TYPE type, const char character, const wchar_t wideCharacter ) :
		Type( type ), Character( character ), WideCharacter( wideCharacter )
	{}
};

class CEventMouse
{
public:
	EVENT_INPUT_TYPE	Type = EVENT_INPUT_TYPE::MOUSEMOVEABS;
	int	X = 0;
	int	Y = 0;

	CEventMouse() = default;
	CEventMouse( const EVENT_INPUT_TYPE type, const int x, const int y ) :
		Type( type ), X( x ), Y( y )
	{}
};