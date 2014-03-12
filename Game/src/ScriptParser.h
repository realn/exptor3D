#pragma once

#include <string>
#include <vector>

#include "EventManager.h"

typedef void	(*SCRIPT_FUNC)( void* pUserData, const std::vector<std::string>& params );

class CScriptVar
{
public:
	std::string	Name;
	std::string	Value;

	CScriptVar( const std::string& name, const std::string& value ) :
		Name( name ), Value( value )
	{}
};

class CScriptFunc
{
public:
	std::string		Name;
	SCRIPT_FUNC		pFunc;
	void*			pUserData;
	unsigned		MinParams;

	CScriptFunc( const std::string& name, SCRIPT_FUNC pFunc, const unsigned minParams, void* pData ) :
		Name( name ), pFunc( pFunc ), pUserData( pData ), MinParams( minParams )
	{}
};

class CScriptParser
{
private:
	CEventManager&	EventManager;
	std::vector<CScriptFunc>	FuncList;
	std::vector<CScriptVar>		VarList;

public:
	CScriptParser( CEventManager& eventManager );

	void	AddVar( const std::string& name, const std::string& value = "" );
	void	AddFunc( const std::string& name, SCRIPT_FUNC pFunc, const unsigned minParams, void* pUserData = nullptr );
	void	SetVar( const std::string& name, const std::string& value );
	const std::string	GetVarValue( const std::string& name ) const;
	const bool			GetVarValue( const std::string& name, std::string& outValue ) const;

	void	Execute( const std::string& text );

	void	SearchFuncNames( const std::string& what, std::vector<std::string>& nameList ) const;

private:
	void	ExecuteVar( const std::string& text );
	void	ExecuteFunc( const std::string& text );

	CScriptVar*		FindVar( const std::string& name ) const;
	CScriptFunc*	FindFunc( const std::string& name ) const;
};