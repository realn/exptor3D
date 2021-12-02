#include <CBCore/Defines.h>
#include <CBCore/StringConvert.h>

#include "event_Event.h"

#include "ScriptParser.h"
#include "StrEx.h"
#include "event_Manager.h"

namespace {
	const cb::string EVENT_ACTION_VAR_ADD = L"script_var_add";
	const cb::string EVENT_ACTION_VAR_SET = L"script_var_set";
	const cb::string EVENT_ACTION_FUNC_ADD = L"script_func_add";
	const cb::string EVENT_ACTION_FUNC_CALL = L"script_func_call";
}

CScriptParser::CScriptParser(std::shared_ptr<event::Manager> eventManager) 
	: eventManager(eventManager)
{
}

CScriptParser::~CScriptParser() = default;

void	CScriptParser::AddVar( const std::string& name, const std::string& value )
{
	auto pVar = FindVar( name );
	if( pVar == nullptr )
	{
		CScriptVar var( name, value );
		VarList.push_back( var );

		eventManager->addActionEvent(EVENT_ACTION_VAR_ADD, { cb::fromUtf8(name) });
	}
}

void	CScriptParser::AddFunc( const std::string& name, SCRIPT_FUNC pFunc, const unsigned minParams, void* pUserData )
{
	auto pSFunc = FindVar( name );
	if( pSFunc == nullptr )
	{
		CScriptFunc func( name, pFunc, minParams, pUserData );
		FuncList.push_back( func );

		eventManager->addActionEvent(EVENT_ACTION_FUNC_ADD, { cb::fromUtf8(name) });
	}
}

void	CScriptParser::SetVar( const std::string& name, const std::string& value )
{
	auto pVar = FindVar( name );
	if( pVar != nullptr )
	{
		pVar->Value = value;

		eventManager->addActionEvent(EVENT_ACTION_VAR_SET, { cb::fromUtf8(name), cb::fromUtf8(value) });
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

void	CScriptParser::Execute( const std::string& text, const bool printResult )
{
	ExecuteFunc( text, printResult );
	ExecuteVar( text, printResult );
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

void	CScriptParser::ExecuteVar( const std::string& text, const bool printResult )
{
	if( ClearWhiteSpaceFront( text ).find( "var" ) == 0 )
	{
		auto name = ClearWhiteSpace( text ).substr( 3 );
		auto pVar = FindVar( name );
		if( pVar == nullptr )
		{
			AddVar( name );
			if( printResult )
				ExecuteFunc( "Print(Defined var " + name + ")", printResult );
		}
	}
	auto pos = text.find( "=" );
	if( pos != std::string::npos )
	{
		auto name = ClearWhiteSpace( text.substr( 0, pos ) );
		auto pVar = FindVar( name );
		if( pVar != nullptr )
		{
			auto value = ClearWhiteSpaceFront( text.substr( pos + 1 ) );
			SetVar( name, value );
			if( printResult )
				ExecuteFunc( "Print(Var " + name + " was set to " + value + ")", printResult );
		}
	}
	else
	{
		auto name = ClearWhiteSpace( text );
		auto pVar = FindVar( name );
		if( pVar != nullptr && printResult )
			ExecuteFunc( "Print(Var " + pVar->Name + " has value " + pVar->Value + ")", printResult );
	}
}

void	CScriptParser::ExecuteFunc( const std::string& text, const bool printResult )
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

	eventManager->addActionEvent(EVENT_ACTION_FUNC_CALL, { cb::fromUtf8(pFunc->Name) });
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