#pragma once

#include "EditorTool.h"
#include "SolidCube.h"
#include "GenericMesh.h"

#include <Surface.h>
#include <Dictionary.h>

class CSelectionContext;
class CSurfaceContext;
class CEditionBlock;

class CSurfaceTool :
	public IEditorTool
{
private:
	CSelectionContext*	m_ctxSelection;
	CSurfaceContext*	m_ctxSurfaces;

	CSolidCube		m_Cube;
	CGenericMesh	m_Mesh;

	CDictionary<CSurface, CEditionBlock*> m_outerSurfaces;
	glm::vec3 m_Point;

public:
	CSurfaceTool( CSelectionContext* const ctxSelection, CSurfaceContext* const ctxSurfaces );
	virtual ~CSurfaceTool();

	virtual	void	Render( const glm::mat4& proj, const glm::mat4& modelview ) override;
	virtual void	Update( const glm::vec3& mouseOrigin, const glm::vec3& mouseVector ) override;

	virtual void	Prepare() override;
	virtual void	Done() override;

	virtual void	Execute( const bool down ) override;

	virtual const wxString	GetName() const override;
	virtual const wxString	GetIconPath() const override;
};