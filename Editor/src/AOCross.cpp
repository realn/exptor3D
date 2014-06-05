#include "stdafx.h"
#include "AOCross.h"

CAOCross::CAOCross() {
	const float halfSize = 0.5f;

	m_Verts[0] = glm::vec3(-halfSize, 0.0f, 0.0f );
	m_Verts[1] = glm::vec3( halfSize, 0.0f, 0.0f );
	m_Verts[2] = glm::vec3( 0.0f,-halfSize, 0.0f );
	m_Verts[3] = glm::vec3( 0.0f, halfSize, 0.0f );
	m_Verts[4] = glm::vec3( 0.0f, 0.0f,-halfSize );
	m_Verts[5] = glm::vec3( 0.0f, 0.0f, halfSize );

	m_Colors[0] = glm::vec4( 1.0f, 0.0f, 0.0f, 1.0f );
	m_Colors[1] = glm::vec4( 1.0f, 0.0f, 0.0f, 1.0f );
	m_Colors[2] = glm::vec4( 0.0f, 1.0f, 0.0f, 1.0f );
	m_Colors[3] = glm::vec4( 0.0f, 1.0f, 0.0f, 1.0f );
	m_Colors[4] = glm::vec4( 0.0f, 0.0f, 1.0f, 1.0f );
	m_Colors[5] = glm::vec4( 0.0f, 0.0f, 1.0f, 1.0f );
}

CAOCross::~CAOCross() {
}

void	CAOCross::Render() {
	glVertexPointer( 3, GL_FLOAT, 0, this->m_Verts );
	glColorPointer( 4, GL_FLOAT, 0, this->m_Colors );

	glEnableClientState( GL_VERTEX_ARRAY );
	glEnableClientState( GL_COLOR_ARRAY );

	glDrawArrays( GL_LINES, 0, 6 );

	glDisableClientState( GL_VERTEX_ARRAY );
	glDisableClientState( GL_COLOR_ARRAY );
}