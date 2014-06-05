#include "stdafx.h"
#include "WireGrid.h"

CWireGrid::CWireGrid( const glm::vec2& size, const glm::uvec2& slices, const glm::vec4& color ) :
	m_Color(color) 
{
	this->Create( size, slices, glm::vec3( size.x, 0.0f, size.y ) / 2.0f );
}

CWireGrid::CWireGrid( const glm::vec2& size, const glm::uvec2& slices, const glm::vec4& color, const glm::vec3& origin ) :
	m_Color(color)
{
	this->Create( size, slices, origin );
}

CWireGrid::~CWireGrid() {
}

void	CWireGrid::Render() {
	glColor4fv( glm::value_ptr( this->m_Color) );
	glVertexPointer( 3, GL_FLOAT, 0, &this->m_Verts[0] );

	glEnableClientState( GL_VERTEX_ARRAY );

	glDrawArrays( GL_LINES, 0, this->m_Verts.size() );

	glDisableClientState( GL_VERTEX_ARRAY );
}

void	CWireGrid::Create( const glm::vec2& size, const glm::uvec2& slices, const glm::vec3& origin ) {

	glm::vec3 start = -origin;
	glm::vec3 step = glm::vec3( size.x / (float)slices.x, 0.0f, size.y / (float)slices.y );

	for( unsigned y = 0; y < slices.y; y++ ) {
		for( unsigned x = 0; x < slices.x; x++ ) {
			m_Verts.push_back( start + step * glm::vec3( (float)(x + 0), 0.0f, (float)(y + 0)) );
			m_Verts.push_back( start + step * glm::vec3( (float)(x + 1), 0.0f, (float)(y + 0)) );
			m_Verts.push_back( start + step * glm::vec3( (float)(x + 0), 0.0f, (float)(y + 0)) );
			m_Verts.push_back( start + step * glm::vec3( (float)(x + 0), 0.0f, (float)(y + 1)) );
		
			if( x == slices.x - 1 ) {
				m_Verts.push_back( start + step * glm::vec3( (float)(x + 1), 0.0f, (float)(y + 0)) );
				m_Verts.push_back( start + step * glm::vec3( (float)(x + 1), 0.0f, (float)(y + 1)) );
			}

			if( y == slices.y - 1 ) {
				m_Verts.push_back( start + step * glm::vec3( (float)(x + 0), 0.0f, (float)(y + 1) ) );
				m_Verts.push_back( start + step * glm::vec3( (float)(x + 1), 0.0f, (float)(y + 1) ) );
			}
		}
	}
}