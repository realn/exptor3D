#include "stdafx.h"
#include "Application.h"

#include "MainWindow.h"
#include "RenderWindow.h"

#include "../../Engine/src/RenderContext.h"

wxIMPLEMENT_APP(CApplication);

CApplication::CApplication() :
	m_MainWindow( nullptr ),
	m_RenderWindow( nullptr ),
	m_RenderContext( nullptr ),
	m_SelectTool( &m_Selection ),
	m_SurfaceTool( &m_Selection, &m_ctxSurface ),
	m_CurrentTool( &m_SelectTool )
{
}

CApplication::~CApplication() {
}

bool	CApplication::OnInit() {
	wxInitAllImageHandlers();

	this->m_MainWindow = new CMainWindow();
	this->m_MainWindow->Show();

	this->m_RenderWindow = new CRenderWindow( this->m_MainWindow );
	this->m_RenderWindow->Show();

	this->m_RenderContext = new CRenderContext( this->m_RenderWindow->GetView() );
	this->m_RenderWindow->SetContext( this->m_RenderContext );

	this->m_MainWindow->RegisterTool( &this->m_SelectTool );
	this->m_MainWindow->RegisterTool( &this->m_SurfaceTool );

	this->m_RenderWindow->SetTool( &this->m_SelectTool );

	this->m_MainWindow->Bind( wxEVT_MENU, &CApplication::OnButtonEvent, this );

	return true;
}

void	CApplication::OnButtonEvent( wxCommandEvent& event ) {
	auto pTool = this->m_MainWindow->FindTool( event.GetId() );
	if( pTool != nullptr && this->m_CurrentTool != pTool ) {
		this->m_CurrentTool->Done();
		this->m_CurrentTool = pTool;
		this->m_CurrentTool->Prepare();

		this->m_RenderWindow->SetTool( this->m_CurrentTool );
	}
}