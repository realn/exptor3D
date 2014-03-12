#pragma once

enum class EVENT_SCRIPT_TYPE : unsigned
{
	FUNC_ADD = 0x20,
	FUNC_INVOKE,
	VAR_ADD,
	VAR_SET,
};

class CEventFunc
{
public:
	EVENT_SCRIPT_TYPE	Type;
	char	Name[64];

	CEventFunc() :
		Type( EVENT_SCRIPT_TYPE::FUNC_ADD )
	{
		memset( Name, 0, sizeof(char) * 64 );
	}
	CEventFunc( const EVENT_SCRIPT_TYPE type, const std::string& name ) :
		Type( type )
	{
		memset( Name, 0, sizeof(char) * 64 );
		unsigned nameLen = name.length() > 63 ? name.length() : 63;
		memcpy( Name, name.c_str(), nameLen * sizeof(char) );
	}
};

class CEventVar
{
public:
	EVENT_SCRIPT_TYPE	Type;
	char	Name[64];

	CEventVar() :
		Type( EVENT_SCRIPT_TYPE::VAR_ADD )
	{
		memset( Name, 0, sizeof(char) * 64 );
	}
	CEventVar( const EVENT_SCRIPT_TYPE type, const std::string& name ) :
		Type( type )
	{
		memset( Name, 0, sizeof(char) * 64 );
		unsigned nameLen = name.length() > 63 ? name.length() : 63;
		memcpy( Name, name.c_str(), nameLen * sizeof(char) );
	}
};