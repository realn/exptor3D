#pragma once

#include <wx/glcanvas.h>

#include "RenderViewConfig.h"

class CRenderView :
	public wxGLCanvas
{
public:
	CRenderView(wxWindow* pParent, const CRenderViewConfig& config);
	virtual ~CRenderView();

	void	Initialize(const wxShowEvent& event);
	void	RenderFrame();

protected:
	bool	m_initialized;
};