#include "stdafx.h"
#include "MainWindow.h"
#include "EditorTool.h"

#include <wx/menu.h>
#include <wx/toolbar.h>

CMainWindow::CMainWindow() :
	wxMDIParentFrame( nullptr, wxID_ANY, _("Editor") )
{
	this->InitMenu();
	this->InitToolBar();
}

CMainWindow::~CMainWindow() {

}

void	CMainWindow::RegisterTool( IEditorTool* const pTool ) {
	int toolId = 100;
	if( !this->m_ToolMap.empty() )
		toolId = (*this->m_ToolMap.rbegin()).first + 1;

	this->m_ToolMap.emplace( toolId, pTool );

	wxBitmap icon( pTool->GetIconPath(), wxBITMAP_TYPE_ANY );
	this->m_Toolbar->AddTool( toolId, pTool->GetName(), icon );
	this->m_Toolbar->Realize();
}

IEditorTool* const	CMainWindow::FindTool( const int id ) const {
	auto it = this->m_ToolMap.find( id );
	if( it == this->m_ToolMap.cend() )
		return nullptr;

	return it->second;
}

void	CMainWindow::InitMenu() {
	auto menu = new wxMenuBar();

	{
		auto menuFile = new wxMenu();
		menuFile->Append(0, _("New"));

		menu->Append( menuFile, _("File") );
	}

	this->SetMenuBar( menu );
}

void	CMainWindow::InitToolBar() {
	this->m_Toolbar = this->CreateToolBar();
	
}