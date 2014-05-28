#pragma once

#define GLEW_STATIC
#include <GL/glew.h>
#include <wx/wx.h>

class CTexture {
private:
	GLuint		m_Texture;
	wxSize		m_Size;
	GLenum		m_Format;
	wxString	m_Id;

public:
	CTexture( const wxSize& size, const GLenum format );
	CTexture( const wxSize& size, const GLenum format, const wxString& id );
	~CTexture();

	bool	Load( const GLenum inputFormat, const GLenum inputType, const void* pData );
	template<typename _T>
	bool	Load( const GLenum inputFormat, const GLenum inputType, const std::vector<_T>& data ) {
		return Load( inputFormat, inputType, &data[0] );
	}

	bool	Read( const GLenum outputFormat, const GLenum outputType, void* pData ) const;
	template<typename _T>
	bool	Read( const GLenum outputFormat, const GLenum outputType, std::vector<_T>& data ) const {
		return Read( outputFormat, outputType, &data[0] );
	}

	const GLuint	GetTexture() const { return m_Texture; }
	const wxSize	GetSize() const { return m_Size; }
	const GLenum	GetFormat() const { return m_Format; }
	const wxString	GetId() const { return m_Id; }

private:
	void	Create();
};