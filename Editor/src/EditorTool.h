#pragma once

#include <glm/glm.hpp>
#include <wx/string.h>

class IEditorTool {
protected:
	IEditorTool() { }

public:
	virtual ~IEditorTool() { }

	virtual	void	Render( const glm::mat4& proj, const glm::mat4& modelview ) = 0;
	virtual void	Update( const glm::vec3& mouseOrigin, const glm::vec3& mouseVector ) = 0;

	virtual void	Prepare() { }
	virtual void	Done() { }

	virtual void	Execute( const bool down ) = 0;
	virtual void	ExecuteAlt( const bool down ) { }

	virtual void	SetModifier( const bool set ) { }
	virtual void	SetModifierAlt( const bool set ) { }

	virtual const wxString	GetName() const = 0;
	virtual const wxString	GetIconPath() const = 0;
};