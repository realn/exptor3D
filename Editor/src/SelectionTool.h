#pragma once

#include "EditorTool.h"
#include "WireCube.h"

#include <Plane.h>
#include <vector>
#include <wx/string.h>

class CSelectionContext;

class CSelectionTool : 
	public IEditorTool
{
private:
	CSelectionContext*	const m_Context;
	CWireCube	m_Cube;
	CPlane		m_Plane;
	glm::vec3	m_wsCursorPos;
	glm::vec3	m_wsSelectPos;
	bool		m_Visible;
	bool		m_Selecting;
	std::vector<glm::vec3>	m_Selection;

public:
	CSelectionTool( CSelectionContext* const context );
	virtual ~CSelectionTool();

	virtual void	Render( const glm::mat4& proj, const glm::mat4& modelview ) override;
	virtual void	Update( const glm::vec3& cursorOrigin, const glm::vec3& cursorVector ) override;
	virtual void	Execute( const bool down ) override;

	virtual const wxString	GetName() const override;
	virtual const wxString	GetIconPath() const override;
};