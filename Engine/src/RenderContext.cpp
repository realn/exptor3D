#include "stdafx.h"
#include "RenderContext.h"

CRenderContext::CRenderContext(
	CRenderView* view, 
	const unsigned minMajor, 
	const unsigned minMinor, 
	const wxGLContext* sharedContext) : wxGLContext(view, sharedContext)
{
	
}

CRenderContext::~CRenderContext() {

}