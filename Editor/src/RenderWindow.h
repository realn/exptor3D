#pragma once

#include <wx/mdi.h>
#include <glm/glm.hpp>

#include <Plane.h>

#include "AOCross.h"
#include "WireGrid.h"
#include "WireCube.h"
#include "GraphicPlane.h"

class CMainWindow;
class CRenderView;
class CRenderContext;
class IEditorTool;

class CRenderWindow :
	public wxMDIChildFrame
{
private:
	CRenderView*	m_View;
	CRenderContext*	m_Context;
	IEditorTool*	m_Tool;

	CAOCross	m_AOCross;
	CWireGrid	m_WireGrid;
	CWireCube	m_WireCube;
	CGraphicPlane	m_GPlane;

	glm::ivec2	m_MouseStart;
	glm::vec3	m_Rotation;
	glm::ivec4	m_Viewport;
	glm::vec3	m_CursorPos;
	CPlane		m_Plane;

	bool	m_ShowCursor;

public:
	CRenderWindow(CMainWindow* const parent);
	virtual ~CRenderWindow();

	CRenderView* const	GetView() const { return m_View; }
	void	SetContext( CRenderContext* const context );

	void	SetTool( IEditorTool* const tool );

	void	Render();
	void	Update();

private:

	const bool	CheckContext();

	void	OnPaintEvent( wxPaintEvent& event );
	void	OnSizeEvent( wxSizeEvent& event );
	void	OnMouseDownEvent( wxMouseEvent& event );
	void	OnMouseUpEvent( wxMouseEvent& event );
	void	OnMouseMoveEvent( wxMouseEvent& event );

	const float	GetAspectRatio() const;

	const glm::vec4	CreateViewportVector() const;
	const glm::mat4	CreateProjectionMatrix() const;
	const glm::mat4	CreatePositionMatrix() const;
	const glm::mat4	CreateRotationMatrix() const;
	const glm::mat4	CreateZoomMatrix() const;

	void	RenderSelection();
	void	RenderUI();
};