#pragma once

#include <string>
#include <vector>

typedef void	(*SCRIPT_FUNC)( void* pUserData, const std::vector<std::string>& params );

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
	std::vector<CScriptFunc>	FuncList;

public:
	CScriptParser();

	void	AddFunc( const std::string& name, SCRIPT_FUNC pFunc, const unsigned minParams, void* pUserData = nullptr );

	void	Execute( const std::string& text );

private:
	CScriptFunc*	FindFunc( const std::string& name );
};