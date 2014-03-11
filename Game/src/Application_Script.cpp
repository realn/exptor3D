#include "Application.h"

#include "gui.h"

void LoadLevel( void* pData, const std::vector<std::string>& params )
{
	((CApplication*)pData)->LoadLevel( params[0] );
}


void	CApplication::RegScript()
{
	GUI.ScriptParser.AddFunc( "LoadLevel", ::LoadLevel, 1, this );
}