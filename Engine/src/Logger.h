#pragma once

#include <wx/file.h>
#include <wx/time.h>

class Logger {
private:
	wxDateTime		m_FirstTick;
	wxString		m_FileName;
	wxFile			m_OutStream;

public:
	Logger();
	~Logger();

	void	Init( const wxString& filename, const wxString& str = "" );
	void	Report( const wxString& str );
	void	Log( const wxString& str );
	void	Note( const wxString& str );
	void	Error( const wxString& str );
	void	FatalError( const wxString& str );

private:
	void SaveToFile( const wxString& str, const wxTimeSpan& time );
};

extern Logger Log;