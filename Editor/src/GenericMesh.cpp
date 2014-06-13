#include "stdafx.h"
#include "GenericMesh.h"

CGenericMesh::CGenericMesh() {
}

CGenericMesh::~CGenericMesh() {
}

void	CGenericMesh::Render( GLenum primitiveMode ) {
	if( this->m_Verts.empty() )
		return;

	glVertexPointer( 3, GL_FLOAT, 0, &this->m_Verts[0] );
	glEnableClientState( GL_VERTEX_ARRAY );

	glDrawElements( primitiveMode, this->m_Indices.size(), GL_UNSIGNED_SHORT, &this->m_Indices[0] );

	glDisableClientState( GL_VERTEX_ARRAY );
}

void	CGenericMesh::Clear() {
	this->m_Verts.clear();
	this->m_Indices.clear();
}

void	CGenericMesh::AddPoint( const glm::vec3& v0 ) {
	this->m_Indices.push_back( this->InsertVertex( v0 ) );
}

void	CGenericMesh::AddLine( const glm::vec3& v0, const glm::vec3& v1 ) {
	this->AddPoint( v0 );
	this->AddPoint( v1 );
}

void	CGenericMesh::AddTriangle( const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2 ) {
	this->AddPoint( v0 );
	this->AddPoint( v1 );
	this->AddPoint( v2 );
}

void	CGenericMesh::AddTriQuad( const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& v3 ) {
	this->AddTriangle( v0, v1, v2 );
	this->AddTriangle( v0, v2, v3 );
}

const unsigned short	CGenericMesh::InsertVertex( const glm::vec3& vertex ) {
	for( unsigned short i = 0; i < this->m_Verts.size(); i++ ) {
		if( this->m_Verts[i] == vertex )
			return i;
	}

	this->m_Verts.push_back( vertex );
	return this->m_Verts.size() - 1;
}