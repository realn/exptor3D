#pragma once

#include <wx/mdi.h>
#include <map>

class wxToolBar;

class IEditorTool;

class CMainWindow :
	public wxMDIParentFrame
{
private:
	wxToolBar*	m_Toolbar;

	std::map<int, IEditorTool*>	m_ToolMap;

public:
	CMainWindow();
	virtual ~CMainWindow();

	void	RegisterTool( IEditorTool* const pTool );
	IEditorTool* const	FindTool( const int id ) const;

private:
	void	InitMenu();
	void	InitToolBar();
};