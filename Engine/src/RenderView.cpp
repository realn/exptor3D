#include "stdafx.h"
#include "RenderView.h"

CRenderView::CRenderView(wxWindow* pParent, const CRenderViewConfig& config) :
	wxGLCanvas(pParent, wxID_ANY, config.GetAttribs()),
	m_initialized(false)
{
	//this->Bind(wxEVT_SHOW, &CRenderView::Initialize, this);
}

CRenderView::~CRenderView() {
}

void	CRenderView::Initialize(const wxShowEvent& event) {
	if(m_initialized)
		return;
	this->m_initialized = true;

	auto err = glewInit();

	wxASSERT(err == GLEW_OK);

	auto szVersion = glGetString(GL_VERSION);

	wxASSERT(GLEW_VERSION_1_1);
	wxASSERT(GLEW_VERSION_1_2);
	wxASSERT(GLEW_VERSION_1_3);
	wxASSERT(GLEW_VERSION_1_4);

	glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
	glClearDepth(0.0f);
	glClearStencil(1);
}

void	CRenderView::RenderFrame() {
	glClear(GL_COLOR_BUFFER_BIT);



	this->SwapBuffers();
}