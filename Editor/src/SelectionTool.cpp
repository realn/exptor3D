#include "stdafx.h"
#include "SelectionTool.h"
#include "SelectionContext.h"

#include <LevelBlock.h>

CSelectionTool::CSelectionTool( CSelectionContext* const context ) :
	m_Cube( glm::vec3( 1.0f ), glm::vec4( 1.0f ) ),
	m_Visible( false ),
	m_Selecting( false ),
	m_Context( context )
{
	glm::vec3	v1(1.0f, -0.5f, 1.0f ), v2( 1.0f, -0.5f, -1.0f ), v3( -1.0f, -0.5f, -1.0f );

	this->m_Plane.Set( v1, v2, v3 );
}

CSelectionTool::~CSelectionTool() {
}

void	CSelectionTool::Render( const glm::mat4& proj, const glm::mat4& modelview ) {
	this->m_Context->Render( proj, modelview );

	if( !this->m_Visible )
		return;

	glMatrixMode( GL_PROJECTION );
	glLoadMatrixf( glm::value_ptr( proj ) );

	if( m_Selecting ) {
		for( auto it = this->m_Selection.cbegin(); it != this->m_Selection.cend(); it++ ) {
			const glm::mat4 trans = glm::translate( glm::mat4(1.0f), *it );

			glMatrixMode( GL_MODELVIEW );
			glLoadMatrixf( glm::value_ptr( modelview * trans ) );

			this->m_Cube.Render();
		}
	}
	else {
		const glm::mat4 trans = glm::translate( glm::mat4(1.0f), this->m_wsSelectPos );

		glMatrixMode( GL_MODELVIEW );
		glLoadMatrixf( glm::value_ptr( modelview * trans ) );

		this->m_Cube.Render();
	}
}

void	CSelectionTool::Update( const glm::vec3& cursorOrigin, const glm::vec3& cursorVector ) {
	if( this->m_Selecting ) {
		if( this->m_Plane.Intersects( cursorOrigin, cursorVector, this->m_wsCursorPos ) ) {
			const glm::vec3 blockSize = glm::vec3(1.0f);

			auto toPos = ClampBlockPosWS( this->m_wsCursorPos ) * glm::vec3( 1.0f, 0.0f, 1.0f );
			auto sizeVector = (toPos - this->m_wsSelectPos) / blockSize;
			auto step = glm::sign(sizeVector) * blockSize;

			auto maxSize = glm::uvec3( glm::abs( sizeVector ) ) + glm::uvec3(1);
			this->m_Selection.clear();
			for( unsigned y = 0; y < maxSize.z; y++ ) {
				for( unsigned x = 0; x < maxSize.x; x++ ) {
					auto pos = this->m_wsSelectPos + step * glm::vec3( (float)x, 0.0f, (float)y );

					this->m_Selection.push_back( pos );
				}
			}
		}
	}
	else {
		this->m_Visible = this->m_Plane.Intersects( cursorOrigin, cursorVector, this->m_wsCursorPos );
		if( this->m_Visible ) {
			this->m_wsSelectPos = ClampBlockPosWS( this->m_wsCursorPos ) * glm::vec3( 1.0f, 0.0f, 1.0f );
		}
	}
}

void	CSelectionTool::Execute( const bool down ) {
	if( !this->m_Selecting && down ) {
		this->m_Selecting = true;
		this->m_Visible = true;
	}
	else if( this->m_Selecting && !down ) {
		this->m_Selecting = false;
		
		this->m_Context->SetSelection( this->m_Selection );
	}
}

const wxString	CSelectionTool::GetName() const{
	return _( "Selection" );
}

const wxString	CSelectionTool::GetIconPath() const {
	return wxT( "Data/Editor/Tool_Selection.png" );
}