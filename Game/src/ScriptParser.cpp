#include "ScriptParser.h"
#include "StrEx.h"
#include "EventScript.h"

CScriptParser::CScriptParser( CEventManager& eventManager ) :
	EventManager( eventManager )
{
}

void	CScriptParser::AddVar( const std::string& name, const std::string& value )
{
	auto pVar = FindVar( name );
	if( pVar == nullptr )
	{
		CScriptVar var( name, value );
		VarList.push_back( var );

		CEventVar varEvent( EVENT_SCRIPT_TYPE::VAR_ADD, name );
		EventManager.AddEvent( varEvent );
	}
}

void	CScriptParser::AddFunc( const std::string& name, SCRIPT_FUNC pFunc, const unsigned minParams, void* pUserData )
{
	auto pSFunc = FindVar( name );
	if( pSFunc == nullptr )
	{
		CScriptFunc func( name, pFunc, minParams, pUserData );
		FuncList.push_back( func );

		CEventFunc funcEvent( EVENT_SCRIPT_TYPE::FUNC_ADD, name );
		EventManager.AddEvent( funcEvent );
	}
}

void	CScriptParser::SetVar( const std::string& name, const std::string& value )
{
	auto pVar = FindVar( name );
	if( pVar != nullptr )
	{
		pVar->Value = value;

		CEventVar varEvent( EVENT_SCRIPT_TYPE::VAR_SET, name );
		EventManager.AddEvent( varEvent );
	}
}

const bool	CScriptParser::GetVarValue( const std::string& name, std::string& outValue ) const
{
	auto pVar = FindVar( name );
	if( pVar == nullptr )
		return false;

	outValue = pVar->Value;
	return true;
}

const std::string	CScriptParser::GetVarValue( const std::string& name ) const
{
	auto pVar = FindVar( name );
	if( pVar == nullptr )
		return "";

	return pVar->Value;
}

void	CScriptParser::Execute( const std::string& text )
{
	ExecuteFunc( text );
	ExecuteVar( text );
}

void	CScriptParser::SearchFuncNames( const std::string& what, std::vector<std::string>& nameList ) const
{
	for( unsigned i = 0; i < FuncList.size(); i++ )
	{
		auto& name = FuncList[i].Name;
		if( name.substr( 0, what.length() ) == what )
			nameList.push_back( name );
	}
}

void	CScriptParser::ExecuteVar( const std::string& text )
{
	auto pos = text.find( "=" );
	if( pos != std::string::npos )
	{
		auto name = ClearWhiteSpace( text.substr( 0, pos ) );
		auto pVar = FindVar( name );
		if( pVar != nullptr )
		{
			auto value = ClearWhiteSpaceFront( text.substr( pos + 1 ) );
			SetVar( name, value );
		}
	}
	else
	{
		auto name = ClearWhiteSpace( text );
		auto pVar = FindVar( name );
		if( pVar != nullptr )
			ExecuteFunc( "Print(" + pVar->Name + " = " + pVar->Value + ")" );
	}
}

void	CScriptParser::ExecuteFunc( const std::string& text )
{
	auto pos = text.find( "(" );
	if( pos == std::string::npos )
		return;

	auto name = ClearWhiteSpace( text.substr( 0, pos ) );
	auto pFunc = FindFunc( name );
	if( pFunc == nullptr )
		return;

	pos++;
	auto endpos = text.find(")");
	if( endpos == std::string::npos )
		return;

	std::vector<std::string> params;
	SplitString( text.substr( pos, endpos - pos ), ",", params );
	if( params.size() < pFunc->MinParams )
		return;

	pFunc->pFunc( pFunc->pUserData, params );

	CEventFunc funcEvent( EVENT_SCRIPT_TYPE::FUNC_INVOKE, pFunc->Name );
	EventManager.AddEvent( funcEvent );
}

CScriptVar*		CScriptParser::FindVar( const std::string& name ) const
{
	for( unsigned i = 0; i < VarList.size(); i++ )
	{
		if( VarList[i].Name == name )
			return const_cast<CScriptVar*>(&VarList[i]);
	}
	return nullptr;
}

CScriptFunc*	CScriptParser::FindFunc( const std::string& name ) const
{
	for( unsigned i = 0; i < FuncList.size(); i++ )
	{
		if( FuncList[i].Name == name )
			return const_cast<CScriptFunc*>(&FuncList[i]);
	}
	return nullptr;
}