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
	m_SelectTool( &m_Selection )
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

	this->m_RenderWindow->SetTool( &this->m_SelectTool );

	return true;
}