#include "stdafx.h"
#include "RenderWindow.h"
#include "MainWindow.h"
#include "EditorTool.h"

#include <RenderView.h>
#include <RenderViewConfig.h>
#include <RenderContext.h>
#include <LevelBlock.h>

CRenderWindow::CRenderWindow( CMainWindow* const parent ) :
	wxMDIChildFrame( parent, wxID_ANY, _("Render View") ),
	m_View( nullptr ),
	m_Context( nullptr ),
	m_Tool( nullptr ),
	m_WireGrid( 
		glm::vec2( 11.0f, 11.0f), 
		glm::uvec2( 11, 11 ), 
		glm::vec4( 0.8f, 0.8f, 0.8f, 1.0f ),
		glm::vec3( 5.5f, 0.5f, 5.5f )
	),
	m_WireCube( 
		glm::vec3( 1.0f, 1.0f, 1.0f ),
		glm::vec4( 0.4f, 0.4f, 1.0f, 1.0f )
	),
	m_GPlane( 
		PLANE_ORIENTATION::AXIS_XZ, 
		glm::vec2( 1.0f, 1.0f ), 
		glm::vec4( 1.0f, 1.0f, 0.0f, 0.5f ),
		glm::vec3( 0.5f, 0.5f, 0.5f )
	)
{
	CRenderViewConfig config;
	
	this->m_View = new CRenderView( this, config );
	this->m_View->Show();

	this->m_View->Bind(wxEVT_PAINT, &CRenderWindow::OnPaintEvent, this);
	this->m_View->Bind(wxEVT_SIZE, &CRenderWindow::OnSizeEvent, this );
	this->m_View->Bind(wxEVT_LEFT_DOWN, &CRenderWindow::OnMouseDownEvent, this );
	this->m_View->Bind(wxEVT_RIGHT_DOWN, &CRenderWindow::OnMouseDownEvent, this );
	this->m_View->Bind(wxEVT_LEFT_UP, &CRenderWindow::OnMouseUpEvent, this );
	this->m_View->Bind(wxEVT_RIGHT_UP, &CRenderWindow::OnMouseUpEvent, this );
	this->m_View->Bind(wxEVT_MOTION, &CRenderWindow::OnMouseMoveEvent, this );

	glm::vec3	v1(1.0f, -0.5f, 1.0f ), v2( 1.0f, -0.5f, -1.0f ), v3( -1.0f, -0.5f, -1.0f );

	this->m_Plane.Set( v1, v2, v3 );
}

CRenderWindow::~CRenderWindow() {
}

void	CRenderWindow::SetContext( CRenderContext* const context ) {
	this->m_Context = context;
}

void	CRenderWindow::SetTool( IEditorTool* const tool ) {
	this->m_Tool = tool;
}

const bool	CRenderWindow::CheckContext() {
	if( this->m_Context == nullptr )
		return false;

	return this->m_Context->SetCurrent( *this->m_View );
}

void	CRenderWindow::OnPaintEvent( wxPaintEvent& event ) {
	if(!this->CheckContext())
		return;

	this->Render();
}

void	CRenderWindow::OnSizeEvent( wxSizeEvent& event ) {
	if(!this->CheckContext())
		return;

	auto size = this->GetClientSize();
	this->m_Viewport = glm::ivec4( 0, 0, size.GetWidth(), size.GetHeight() );
	glViewport( this->m_Viewport.x, this->m_Viewport.y, this->m_Viewport.z, this->m_Viewport.w );

	this->Update();
	this->Render();
}

void	CRenderWindow::OnMouseDownEvent( wxMouseEvent& event ) {
	if( event.Button( wxMOUSE_BTN_LEFT ) ) {
		this->m_Tool->Execute( true );
	}

	this->m_MouseStart.x = event.GetX();
	this->m_MouseStart.y = event.GetY();

	this->Update();
	this->Refresh();
}

void	CRenderWindow::OnMouseUpEvent( wxMouseEvent& event ) {
	if( event.Button( wxMOUSE_BTN_LEFT ) ) {
		this->m_Tool->Execute( false );
	}

	this->Update();
	this->Refresh();
}

void	CRenderWindow::OnMouseMoveEvent( wxMouseEvent& event ) {	
	if( event.ButtonIsDown( wxMOUSE_BTN_RIGHT ) ) {
		this->m_Rotation.y += event.GetX() - this->m_MouseStart.x;
		this->m_Rotation.x += event.GetY() - this->m_MouseStart.y;
	}

	this->m_MouseStart.x = event.GetX();
	this->m_MouseStart.y = event.GetY();

	this->Update();
	this->Refresh();
}

const float	CRenderWindow::GetAspectRatio() const {
	auto size = this->m_View->GetClientSize();

	return (float)size.GetWidth() / (float)size.GetHeight();
}

const glm::mat4	CRenderWindow::CreateProjectionMatrix() const {
	return glm::perspective( 70.0f, this->GetAspectRatio(), 1.0f, 100.0f );
}

const glm::mat4	CRenderWindow::CreatePositionMatrix() const {
	return glm::mat4(1.0f);
}

const glm::mat4	CRenderWindow::CreateRotationMatrix() const {
	return glm::rotate(glm::mat4(1.0f), this->m_Rotation.y, glm::vec3(0.0f, 1.0f, 0.0f) ) *
		glm::rotate(glm::mat4(1.0f), this->m_Rotation.x, glm::vec3(1.0f, 0.0f, 0.0f) );	
}

const glm::mat4	CRenderWindow::CreateZoomMatrix() const {
	return glm::translate( glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -2.0f ) );
}

void	CRenderWindow::Update() {
	auto proj = this->CreateProjectionMatrix();
	auto modelview = this->CreateZoomMatrix() * this->CreateRotationMatrix() * 
		this->CreatePositionMatrix();

	auto pos1 = glm::unProject( 
		glm::vec3( this->m_MouseStart.x, m_Viewport.w - this->m_MouseStart.y, 0.0f ),
		modelview,
		proj,
		m_Viewport
		);

	auto pos2 = glm::unProject( 
		glm::vec3( this->m_MouseStart.x, m_Viewport.w - this->m_MouseStart.y, 1.0f ),
		modelview,
		proj,
		m_Viewport
		);

	if( this->m_Tool != nullptr ) {
		this->m_Tool->Update( pos1, pos2 - pos1 );
	}
}

void	CRenderWindow::Render() {
	glDisable( GL_LIGHTING );
	glDepthFunc( GL_LEQUAL );
	glClearDepth( 1.0f );
	glEnable( GL_DEPTH_TEST );
	glLineWidth( 2.0f );
	glClearColor( 0.2f, 0.2f, 0.2f, 1.0f );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	auto proj = this->CreateProjectionMatrix();
	auto modelview = this->CreateZoomMatrix() * this->CreateRotationMatrix() * 
		this->CreatePositionMatrix();

	glMatrixMode( GL_PROJECTION );
	glLoadMatrixf( glm::value_ptr( proj ) );
	
	glMatrixMode( GL_MODELVIEW );
	glLoadMatrixf( glm::value_ptr( modelview ) );

	this->m_WireGrid.Render();

	if( this->m_Tool != nullptr ) {
		this->m_Tool->Render( proj, modelview );
	}

	this->RenderUI();

	this->m_View->SwapBuffers();
}

void	CRenderWindow::RenderSelection() {
}

void	CRenderWindow::RenderUI() {
	glClear( GL_DEPTH_BUFFER_BIT );

	auto proj = glm::ortho( -0.5f, this->GetAspectRatio() * 6.0f, -0.5f, 6.0f, -2.0f, 2.0f );
	auto modelview = this->CreateRotationMatrix();

	glMatrixMode( GL_PROJECTION );
	glLoadMatrixf( glm::value_ptr( proj ) );
	
	glMatrixMode( GL_MODELVIEW );
	glLoadMatrixf( glm::value_ptr( modelview ) );

	this->m_AOCross.Render();
}