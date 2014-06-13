#include "stdafx.h"
#include "SolidCube.h"

CSolidCube::CSolidCube( const glm::vec3& size, const glm::vec4& color, const glm::vec3& origin ) :
	m_Color( color )
{
	this->Create( size, origin );
}

CSolidCube::CSolidCube( const glm::vec3& size, const glm::vec4& color ) :
	m_Color( color )
{
	this->Create( size, size / 2.0f );
}

CSolidCube::~CSolidCube() {
}

void	CSolidCube::Render() {
	this->Render( this->m_Color );
}

void	CSolidCube::Render( const glm::vec4& color ) {
	glColor4fv( glm::value_ptr( color ) );

	glEnableClientState( GL_VERTEX_ARRAY );

	glVertexPointer( 3, GL_FLOAT, 0, &this->m_Verts[0] );

	glDrawElements( GL_TRIANGLES, this->m_Indices.size(), GL_UNSIGNED_SHORT, &this->m_Indices[0] );

	glDisableClientState( GL_VERTEX_ARRAY );
}

void	CSolidCube::Create( const glm::vec3& size, const glm::vec3& origin ) {
	auto start = -origin;

	this->m_Verts.push_back( start + size * glm::vec3( 0.0f, 0.0f, 0.0f ) );
	this->m_Verts.push_back( start + size * glm::vec3( 0.0f, 0.0f, 1.0f ) );
	this->m_Verts.push_back( start + size * glm::vec3( 1.0f, 0.0f, 1.0f ) );
	this->m_Verts.push_back( start + size * glm::vec3( 1.0f, 0.0f, 0.0f ) );

	this->m_Verts.push_back( start + size * glm::vec3( 0.0f, 1.0f, 0.0f ) );
	this->m_Verts.push_back( start + size * glm::vec3( 1.0f, 1.0f, 0.0f ) );
	this->m_Verts.push_back( start + size * glm::vec3( 1.0f, 1.0f, 1.0f ) );
	this->m_Verts.push_back( start + size * glm::vec3( 0.0f, 1.0f, 1.0f ) );

	this->AddQuad( 0, 1, 2, 3 );
	this->AddQuad( 4, 5, 6, 7 );
	this->AddQuad( 0, 4, 7, 1 );
	this->AddQuad( 1, 7, 6, 2 );
	this->AddQuad( 2, 6, 5, 3 );
	this->AddQuad( 3, 5, 4, 0 );
}

void	CSolidCube::AddQuad( const unsigned v0, const unsigned v1, const unsigned v2, const unsigned v3 ) {
	this->AddTriangle( v0, v1, v2 );
	this->AddTriangle( v0, v2, v3 );
}

void	CSolidCube::AddTriangle( const unsigned v0, const unsigned v1, const unsigned v2 ) {
	this->m_Indices.push_back( v0 );
	this->m_Indices.push_back( v1 );
	this->m_Indices.push_back( v2 );
}