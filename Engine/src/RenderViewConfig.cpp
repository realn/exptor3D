#include "stdafx.h"
#include "RenderViewConfig.h"

#include <wx/glcanvas.h>

CRenderViewConfig::CRenderViewConfig() :
	ColorBits(32),
	DepthBits(24),
	StencilBits(8)
{
}

CRenderViewConfig::~CRenderViewConfig(){
}

int*	CRenderViewConfig::GetAttribs() const {
	this->m_Attribs.clear();
	
	this->AddAttrib(WX_GL_DOUBLEBUFFER);
	this->AddAttrib(WX_GL_RGBA);
	this->AddAttrib(WX_GL_BUFFER_SIZE, this->ColorBits);
	this->AddAttrib(WX_GL_DEPTH_SIZE, this->DepthBits);
	this->AddAttrib(WX_GL_STENCIL_SIZE, this->StencilBits);
	this->AddAttrib(0, 0);

	return &this->m_Attribs[0];
}

void	CRenderViewConfig::AddAttrib(int attrib) const {
	this->m_Attribs.push_back(attrib);
}

void	CRenderViewConfig::AddAttrib(int attrib, int value) const {
	this->m_Attribs.push_back(attrib);
	this->m_Attribs.push_back(value);
}