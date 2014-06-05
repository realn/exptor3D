#include "stdafx.h"
#include "SelectionContext.h"

CSelectionContext::CSelectionContext() :
	m_Cube( glm::vec3(1.0f), glm::vec4(1.0f, 0.0f, 0.0f, 0.7f))
{

}

void	CSelectionContext::AddSelection( const std::vector<glm::vec3>& selection ) {
	for( auto it = selection.cbegin(); it != selection.cend(); it++ ) {
		this->m_Selection.push_back( *it );
	}
}

void	CSelectionContext::SetSelection( const std::vector<glm::vec3>& selection ) {
	this->Clear();
	this->AddSelection( selection );
}

const std::vector<glm::vec3>&	CSelectionContext::GetSelection() const {
	return this->m_Selection;
}

void	CSelectionContext::Clear() {
	this->m_Selection.clear();
}

void	CSelectionContext::Render( const glm::mat4& proj, const glm::mat4& modelview ) {
	glMatrixMode( GL_PROJECTION );
	glLoadMatrixf( glm::value_ptr( proj ) );
	glMatrixMode( GL_MODELVIEW );

	for( auto it = this->m_Selection.cbegin(); it != this->m_Selection.cend(); it++ ) {
		const glm::mat4 trans = glm::translate( glm::mat4(1.0f), *it );
		glLoadMatrixf( glm::value_ptr( modelview * trans ) );

		this->m_Cube.Render();
	}
}