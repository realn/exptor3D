#pragma once

#include <wx/glcanvas.h>

#include "RenderView.h"

class CRenderContext : 
	public wxGLContext
{
public:
	CRenderContext(CRenderView* deviceView, 
		const unsigned minMajor = 1, 
		const unsigned minMinor = 1, 
		const wxGLContext* sharedContext = nullptr);
	virtual ~CRenderContext();
};