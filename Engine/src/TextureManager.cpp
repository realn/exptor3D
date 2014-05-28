#include "stdafx.h"
#include "TextureManager.h"
#include "Texture.h"
#include "Logger.h"

#include <wx/image.h>
#include <wx/rawbmp.h>
#include <wx/filename.h>

CTexManager::CTexManager( const wxString& strDataDir ) :
	m_DataDir( strDataDir )
{
}

CTexManager::~CTexManager() {
	Clear();
}

CTexture* const	CTexManager::Get( const wxString& strId ) {
	if( strId.empty() )
	{
		Log.Error( "TEXMANAGER(): Pusty ci¹g znaków!" );
		return nullptr;
	}

	CTexture* pData = this->FindTexture( strId );
	if( pData == nullptr ) {
		pData = this->LoadData( strId );

		if( pData != nullptr ) {
			AddTexture( pData );
			Log.Log( "TEXMANAGER(): Dodano now¹ teksture: " + strId );
		}
		else {
			Log.Error( "TEXMANAGER(): Nieudane za³adowanie tekstury: " + strId );

			return nullptr;
		}
	}
	
	return pData;
}

CTexture* const	CTexManager::AddTexture( CTexture* const texture ) {
	m_List.push_back( texture );
	return texture;
}

CTexture* const CTexManager::FindTexture( const wxString& strId ) {
	if(strId.IsEmpty())
		return nullptr;

	for( auto it = this->m_List.begin(); it != this->m_List.end(); it++ ) {
		if( (*it)->GetId().IsSameAs(strId, false) )
			return (*it);
	}

	return nullptr;
}

void CTexManager::Clear() {
	for( auto it = this->m_List.begin(); it != this->m_List.end(); it++ ) {
		delete (*it);
	}
	this->m_List.clear();
}

CTexture* const	CTexManager::LoadData( const wxString& strId ) {
	wxFileName path( this->m_DataDir, strId );
	if( !wxImage::CanRead( path.GetPath() ) )
		return nullptr;
	
	wxImage img( path.GetPath() );

	if( !img.IsOk() )
		return nullptr;

	CTexture* pData = new CTexture( img.GetSize(), img.HasAlpha() ? GL_RGBA8 : GL_RGB8, strId );

	bool result = false;
	if( !img.HasAlpha() ) {
		result = pData->Load( GL_RGB, GL_UNSIGNED_BYTE, img.GetData() );
	}
	else {
		std::vector<unsigned char> data;
		wxImagePixelData imgData( img );

		data.resize( imgData.GetWidth() * imgData.GetHeight() * 4 );
		unsigned i = 0;
		for( auto it = imgData.GetPixels(); it.IsOk(); it++ ){
			data[i * 4 + 0] = it.Red();
			data[i * 4 + 1] = it.Green();
			data[i * 4 + 2] = it.Blue();
			data[i * 4 + 3] = it.Alpha();
			i++;
		}

		result = pData->Load( GL_RGBA, GL_UNSIGNED_BYTE, data );
	}

	if( !result ) {
		delete pData;
		return nullptr;
	}

	return pData;
}