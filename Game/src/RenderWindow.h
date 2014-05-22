#pragma once

#include <wx/frame.h>

#include "EventManager.h"

class CRenderWindow : 
	public wxFrame
{
private:
	CEventManager&	m_EventManager;

public:
	CRenderWindow(CEventManager& eventManager);
	virtual ~CRenderWindow();

	void	OnKeyDown(const wxKeyEvent& event);
	void	OnKeyUp(const wxKeyEvent& event);
	void	OnMouseMove(const wxMouseEvent& event);
	void	OnMouseKeyDown(const wxMouseEvent& event);
	void	OnMouseKeyUp(const wxMouseEvent& event);
};