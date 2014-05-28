#include "stdafx.h"
#include "Material.h"

CMaterial::CMaterial( const wxString& name ) :
	m_Name( name )
{

}

void	CMaterial::AddTexture( const wxString& slot, CTexture* const texture ) {
	this->m_Textures.emplace( slot, texture );
}