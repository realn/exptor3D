#pragma once

#include <wx/app.h>

#include "SelectionContext.h"
#include "SelectionTool.h"
#include "SurfaceContext.h"
#include "SurfaceTool.h"

class CMainWindow;
class CRenderWindow;
class CRenderContext;

class CApplication :
	public wxApp
{
private:
	CMainWindow*	m_MainWindow;
	CRenderWindow*	m_RenderWindow;
	CRenderContext*	m_RenderContext;

	CSelectionContext	m_Selection;
	CSelectionTool		m_SelectTool;

	CSurfaceContext	m_ctxSurface;
	CSurfaceTool	m_SurfaceTool;

	IEditorTool*	m_CurrentTool;

public:
	CApplication();
	virtual ~CApplication();

	virtual bool	OnInit() override;

	void	OnButtonEvent( wxCommandEvent& event );
};