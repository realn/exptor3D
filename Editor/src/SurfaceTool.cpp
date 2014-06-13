#include "stdafx.h"
#include "SurfaceTool.h"
#include "SelectionContext.h"
#include "SurfaceContext.h"

CSurfaceTool::CSurfaceTool( CSelectionContext* const ctxSelection, CSurfaceContext* const ctxSurfaces ) :
	m_ctxSelection( ctxSelection ),
	m_ctxSurfaces( ctxSurfaces ),
	m_Cube( glm::vec3( 1.0f ), glm::vec4( 1.0f, 0.0f, 0.0f, 0.2f ) )
{

}

CSurfaceTool::~CSurfaceTool() {

}

void	CSurfaceTool::Render( const glm::mat4& proj, const glm::mat4& modelview ) {
	glMatrixMode( GL_PROJECTION );
	glLoadMatrixf( glm::value_ptr( proj ) );
	glMatrixMode( GL_MODELVIEW );

	auto positions = this->m_ctxSurfaces->GetLocations();

	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	glEnable( GL_BLEND );
	glDisable( GL_DEPTH_TEST );

	for( auto it = positions.begin(); it != positions.end(); it++ ) { 
		glLoadMatrixf( glm::value_ptr( modelview * glm::translate( glm::mat4(1.0f), (*it) ) ) );

		this->m_Cube.Render();
	}

	glLoadMatrixf( glm::value_ptr( modelview * glm::translate( glm::mat4(1.0f), m_Point) ) );
	this->m_Cube.Render( glm::vec4( 1.0f, 1.0f, 0.0f, 0.8f ));

	glLoadMatrixf( glm::value_ptr( modelview ) );
	glColor4f( 1.0f, 1.0f, 1.0f, 0.5f );

	this->m_Mesh.Render( GL_TRIANGLES );

	glDisable( GL_BLEND );
	glEnable( GL_DEPTH_TEST );
}

void	CSurfaceTool::Update( const glm::vec3& mouseOrigin, const glm::vec3& mouseVector ) {
	CDictionary<glm::vec3, CSurface> hits;

	glm::vec3 pos;
	for( auto it = this->m_outerSurfaces.begin(); it != this->m_outerSurfaces.end(); it++ ) {
		if( it->first.Intersects( mouseOrigin, mouseVector, pos) ) {
			hits.push_back( pos, it->first );
		}
	}
	m_outerSurfaces.begin()->first.GetPlane().Intersects( mouseOrigin, mouseVector, m_Point );

	float max = 0.0f;
	for( auto it = hits.begin(); it != hits.end(); it++ ) {
		float dist = glm::distance( mouseOrigin, it->first );
		if( dist > max ){
			max = dist;
			pos = it->first;
		}
	}

	this->m_Mesh.Clear();
	if( max != 0.0f ) {
		CSurface surface = hits.get_value(pos);

		this->m_Mesh.AddTriQuad( 
			surface.GetVert( 0 ),
			surface.GetVert( 1 ),
			surface.GetVert( 2 ),
			surface.GetVert( 3 )
			);
	}
}

void	CSurfaceTool::Prepare() {
	this->m_ctxSurfaces->SetEdition( this->m_ctxSelection->GetSelection() );

	this->m_outerSurfaces = this->m_ctxSurfaces->GetOuterSurfaces();
}

void	CSurfaceTool::Done() {
	
}

void	CSurfaceTool::Execute( const bool down ) {
}

const wxString	CSurfaceTool::GetName() const {
	return _( "Surface" );
}

const wxString	CSurfaceTool::GetIconPath() const {
	return wxT( "Data/Editor/Tool_Surface.png" );
}
