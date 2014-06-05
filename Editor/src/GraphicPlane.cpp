#include "stdafx.h"
#include "GraphicPlane.h"

CGraphicPlane::CGraphicPlane( const PLANE_ORIENTATION ori, const glm::vec2& size, const glm::vec4& color) :
	m_Color( color )
{
	this->Create( ori, size, MakeVertex( ori, glm::vec3(0.0f), size / 2.0f, 1, 1 ) );
}

CGraphicPlane::CGraphicPlane( const PLANE_ORIENTATION ori, const glm::vec2& size, const glm::vec4& color, const glm::vec3& origin ) :
	m_Color( color )
{
	this->Create( ori, size, origin );
}

CGraphicPlane::~CGraphicPlane() {

}

void	CGraphicPlane::Render() {
	glColor4fv( glm::value_ptr( this->m_Color) );
	glVertexPointer( 3, GL_FLOAT, 0, &this->m_Verts[0] );

	glEnableClientState( GL_VERTEX_ARRAY );

	glDrawArrays( GL_TRIANGLES, 0, this->m_Verts.size() );

	glDisableClientState( GL_VERTEX_ARRAY );
}

void	CGraphicPlane::Create( const PLANE_ORIENTATION ori, const glm::vec2& size, const glm::vec3& origin ) {
	const glm::vec3 start = -origin;

	this->m_Verts.push_back( this->MakeVertex( ori, start, size, 0, 0 ) );
	this->m_Verts.push_back( this->MakeVertex( ori, start, size, 0, 1 ) );
	this->m_Verts.push_back( this->MakeVertex( ori, start, size, 1, 1 ) );

	this->m_Verts.push_back( this->MakeVertex( ori, start, size, 0, 0 ) );
	this->m_Verts.push_back( this->MakeVertex( ori, start, size, 1, 1 ) );
	this->m_Verts.push_back( this->MakeVertex( ori, start, size, 1, 0 ) );
}

const glm::vec3	CGraphicPlane::MakeVertex( const PLANE_ORIENTATION ori, const glm::vec3& start, const glm::vec2& size, const unsigned x, const unsigned y ) {
	switch (ori)
	{
	case PLANE_ORIENTATION::AXIS_XY:
		return start + glm::vec3( size.x, size.y, 0.0f ) * glm::vec3( (float)x, (float)y, 0.0f );

	case PLANE_ORIENTATION::AXIS_XZ:
		return start + glm::vec3( size.x, 0.0f, size.y ) * glm::vec3( (float)x, 0.0f, (float)y );

	case PLANE_ORIENTATION::AXIS_YZ:
		return start + glm::vec3( 0.0f, size.y, size.x ) * glm::vec3( 0.0f, (float)y, (float)x );

	default:
		return start;
	}
}