#include "ScriptParser.h"
#include "StrEx.h"

CScriptParser::CScriptParser()
{
}

void	CScriptParser::AddFunc( const std::string& name, SCRIPT_FUNC pFunc, const unsigned minParams, void* pUserData )
{
	CScriptFunc func( name, pFunc, minParams, pUserData );
	FuncList.push_back( func );
}

void	CScriptParser::Execute( const std::string& text )
{
	auto clear = ClearWhiteSpace( text );

	auto pos = clear.find( "(" );
	if( pos == std::string::npos )
		return;

	auto pFunc = FindFunc( clear.substr( 0, pos ) );
	if( pFunc == nullptr )
		return;

	pos = text.find("(") + 1;
	auto endpos = text.find(")");
	if( endpos == std::string::npos )
		return;

	std::vector<std::string> params;
	SplitString( text.substr( pos, endpos - pos ), ",", params );
	if( params.size() < pFunc->MinParams )
		return;

	pFunc->pFunc( pFunc->pUserData, params );
}

CScriptFunc*	CScriptParser::FindFunc( const std::string& name )
{
	for( unsigned i = 0; i < FuncList.size(); i++ )
	{
		if( FuncList[i].Name == name )
			return &FuncList[i];
	}
}