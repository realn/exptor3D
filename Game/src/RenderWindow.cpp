#include "stdafx.h"
#include "RenderWindow.h"
#include "Log.h"
#include "EventInput.h"

CRenderWindow::CRenderWindow(CEventManager& eventManager ) :
	wxFrame(nullptr, wxID_ANY, _("Main Window")),
	m_EventManager(eventManager)
{
	this->Bind(wxEVT_KEY_DOWN, &CRenderWindow::OnKeyDown, this);
	this->Bind(wxEVT_KEY_UP, &CRenderWindow::OnKeyUp, this);
	this->Bind(wxEVT_MOTION, &CRenderWindow::OnMouseMove, this);
	this->Bind(wxEVT_LEFT_UP, &CRenderWindow::OnMouseKeyUp, this);
	this->Bind(wxEVT_RIGHT_UP, &CRenderWindow::OnMouseKeyUp, this);
	this->Bind(wxEVT_LEFT_DOWN, &CRenderWindow::OnMouseKeyDown, this);
	this->Bind(wxEVT_RIGHT_DOWN, &CRenderWindow::OnMouseKeyDown, this);
}

CRenderWindow::~CRenderWindow() {
}

void	CRenderWindow::OnKeyDown(const wxKeyEvent& event) {
	CEventKey KeyEvent( EVENT_INPUT_TYPE::KEYDOWN, event.GetKeyCode() );
	m_EventManager.AddEvent( KeyEvent );				
}

void	CRenderWindow::OnKeyUp(const wxKeyEvent& event) {
	CEventKey keyEvent( EVENT_INPUT_TYPE::KEYUP, event.GetKeyCode() );
	this->m_EventManager.AddEvent( keyEvent );
}

void	CRenderWindow::OnMouseMove(const wxMouseEvent& event) {
	CEventMouse mouseEvent( EVENT_INPUT_TYPE::MOUSEMOVEABS, event.GetX(), event.GetY());
	this->m_EventManager.AddEvent( mouseEvent );	
}

void	CRenderWindow::OnMouseKeyDown(const wxMouseEvent& event){
	switch (event.GetButton())
	{
	case wxMOUSE_BTN_LEFT:
		{
			CEventKey mouseEvent( EVENT_INPUT_TYPE::KEYDOWN, WXK_LBUTTON );
			this->m_EventManager.AddEvent( mouseEvent );
		}
		break;

	case wxMOUSE_BTN_RIGHT:
		{
			CEventKey mouseEvent( EVENT_INPUT_TYPE::KEYDOWN, WXK_RBUTTON );
			this->m_EventManager.AddEvent( mouseEvent );
		}
		break;

	default:
		break;
	}
}

void	CRenderWindow::OnMouseKeyUp(const wxMouseEvent& event){
	switch (event.GetButton())
	{
	case wxMOUSE_BTN_LEFT:
		{
			CEventKey mouseEvent( EVENT_INPUT_TYPE::KEYUP, WXK_LBUTTON );
			this->m_EventManager.AddEvent( mouseEvent );
		}
		break;

	case wxMOUSE_BTN_RIGHT:
		{
			CEventKey mouseEvent( EVENT_INPUT_TYPE::KEYUP, WXK_RBUTTON );
			this->m_EventManager.AddEvent( mouseEvent );
		}
		break;

	default:
		break;
	}
}