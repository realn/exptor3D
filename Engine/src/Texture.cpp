#include "stdafx.h"
#include "Texture.h"

CTexture::CTexture( const wxSize& size, const GLenum format ) :
	m_Size( size ),
	m_Format( format ),
	m_Texture( 0 )
{
	Create();
}

CTexture::CTexture( const wxSize& size, const GLenum format, const wxString& id ) :
	m_Size( size ),
	m_Format( format ),
	m_Texture( 0 ),
	m_Id( id )
{
	Create();
}

CTexture::~CTexture() {
	if(m_Texture != 0 && glIsTexture( m_Texture ))
	{
		glDeleteTextures( 1, &m_Texture );
		m_Texture = 0;
	}
}

bool CTexture::Load( const GLenum inputFormat, const GLenum inputType, const void* pData ) {
	glBindTexture( GL_TEXTURE_2D, this->m_Texture );

	glTexSubImage2D( GL_TEXTURE_2D, 0, 0, 0, m_Size.GetWidth(), m_Size.GetHeight(), inputFormat, inputType, pData );
	if(GLEW_VERSION_3_3) {
		glGenerateMipmap( GL_TEXTURE_2D );
	}

	glBindTexture( GL_TEXTURE_2D, 0 );

	return true;
}

bool	CTexture::Read( const GLenum outputFormat, const GLenum outputType, void* pData ) const {
	glBindTexture( GL_TEXTURE_2D, this->m_Texture );

	glGetTexImage( GL_TEXTURE_2D, 0, outputFormat, outputType, pData );

	glBindTexture( GL_TEXTURE_2D, 0 );

	return true;
}

void	CTexture::Create() {
	glGenTextures( 1, &m_Texture );
	glBindTexture( GL_TEXTURE_2D, m_Texture );

	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR );

	if( GLEW_EXT_texture_filter_anisotropic ) {
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 4 );
	}
	if( !GLEW_VERSION_3_3 && GLEW_SGIS_generate_mipmap ) {
		glTexParameteri( GL_TEXTURE_2D, GL_GENERATE_MIPMAP_SGIS, GL_TRUE );
	}

	glTexImage2D( GL_TEXTURE_2D, 0, m_Format, m_Size.GetWidth(), m_Size.GetHeight(), 0, 0, 0, nullptr );
	glBindTexture( GL_TEXTURE_2D, 0 );
}