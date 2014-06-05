#include "stdafx.h"
#include "WireCube.h"

CWireCube::CWireCube( const glm::vec3& size, const glm::vec4& color ) :
	m_Color( color )
{
	this->Create( size, size / 2.0f );
}

CWireCube::CWireCube( const glm::vec3& size, const glm::vec4& color, const glm::vec3& origin ) :
	m_Color( color )
{
	this->Create( size, origin );
}

CWireCube::~CWireCube() {

}

void	CWireCube::Render() {
	glColor4fv( glm::value_ptr( this->m_Color) );
	glVertexPointer( 3, GL_FLOAT, 0, &this->m_Verts[0] );

	glEnableClientState( GL_VERTEX_ARRAY );

	glDrawArrays( GL_LINES, 0, this->m_Verts.size() );

	glDisableClientState( GL_VERTEX_ARRAY );
}

void	CWireCube::Create( const glm::vec3& size, const glm::vec3& origin ) {
	const glm::vec3 start = -origin;

	//	Bottom
	this->m_Verts.push_back( start + size * glm::vec3( 0.0f, 0.0f, 0.0f ) );
	this->m_Verts.push_back( start + size * glm::vec3( 1.0f, 0.0f, 0.0f ) );

	this->m_Verts.push_back( start + size * glm::vec3( 1.0f, 0.0f, 0.0f ) );
	this->m_Verts.push_back( start + size * glm::vec3( 1.0f, 0.0f, 1.0f ) );

	this->m_Verts.push_back( start + size * glm::vec3( 0.0f, 0.0f, 1.0f ) );
	this->m_Verts.push_back( start + size * glm::vec3( 1.0f, 0.0f, 1.0f ) );

	this->m_Verts.push_back( start + size * glm::vec3( 0.0f, 0.0f, 0.0f ) );
	this->m_Verts.push_back( start + size * glm::vec3( 0.0f, 0.0f, 1.0f ) );

	//	Top
	this->m_Verts.push_back( start + size * glm::vec3( 0.0f, 1.0f, 0.0f ) );
	this->m_Verts.push_back( start + size * glm::vec3( 1.0f, 1.0f, 0.0f ) );

	this->m_Verts.push_back( start + size * glm::vec3( 1.0f, 1.0f, 0.0f ) );
	this->m_Verts.push_back( start + size * glm::vec3( 1.0f, 1.0f, 1.0f ) );

	this->m_Verts.push_back( start + size * glm::vec3( 0.0f, 1.0f, 1.0f ) );
	this->m_Verts.push_back( start + size * glm::vec3( 1.0f, 1.0f, 1.0f ) );

	this->m_Verts.push_back( start + size * glm::vec3( 0.0f, 1.0f, 0.0f ) );
	this->m_Verts.push_back( start + size * glm::vec3( 0.0f, 1.0f, 1.0f ) );

	//	Corners
	this->m_Verts.push_back( start + size * glm::vec3( 0.0f, 0.0f, 0.0f ) );
	this->m_Verts.push_back( start + size * glm::vec3( 0.0f, 1.0f, 0.0f ) );

	this->m_Verts.push_back( start + size * glm::vec3( 1.0f, 0.0f, 0.0f ) );
	this->m_Verts.push_back( start + size * glm::vec3( 1.0f, 1.0f, 0.0f ) );

	this->m_Verts.push_back( start + size * glm::vec3( 1.0f, 0.0f, 1.0f ) );
	this->m_Verts.push_back( start + size * glm::vec3( 1.0f, 1.0f, 1.0f ) );

	this->m_Verts.push_back( start + size * glm::vec3( 0.0f, 0.0f, 1.0f ) );
	this->m_Verts.push_back( start + size * glm::vec3( 0.0f, 1.0f, 1.0f ) );
}