#pragma once

#include <wx/string.h>
#include <map>
#include <glm/glm.hpp>

class CTexture;

class CMaterial {
private:
	wxString	m_Name;
	std::map<wxString, CTexture* const>	m_Textures;
	glm::vec4	m_Diffuse;

public:
	CMaterial( const wxString& name );

	const wxString	GetName() const { return m_Name; }
	const glm::vec4	GetDiffuse() const { return m_Diffuse; }

	void			AddTexture( const wxString& slot, CTexture* const texture );
	CTexture* const	GetTexture( const wxString& slot ) const;
	CTexture* const	RemoveTexture( const wxString& slot );
	void			ClearTextures();
};