#include "Application.h"

void LoadLevel( void* pData, const std::vector<std::string>& params )
{
	((CApplication*)pData)->LoadLevel( params[0] );
}

void	Print( void* pData, const std::vector<std::string>& params )
{
	std::string text = JoinString( params, ", " );
	((CApplication*)pData)->Print( text );
}

void	CApplication::RegScript()
{
	scriptParser->AddFunc( "Print", ::Print, 1, this );
	scriptParser->AddFunc( "LoadLevel", ::LoadLevel, 1, this );
}