#pragma once

#include <wx/wx.h>
#include <vector>

class CTexture;

/*	Klasa zarz¹daj¹ca wieloma teksturami
*/
class CTexManager
{
private:
	const wxString	m_DataDir;
	std::vector<CTexture*> m_List;

public:
	CTexManager( const wxString& strDataDir );
	~CTexManager();

	CTexture* const	Get( const wxString& strId );
	CTexture* const AddTexture( CTexture* const texture );

	void Clear();

	CTexture* const	LoadData( const wxString& strId );

private:
	CTexture* const	FindTexture( const wxString& strId );
};