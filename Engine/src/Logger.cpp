#include "stdafx.h"
#include "Logger.h"
#include <wx/msgdlg.h>

Logger Log;

Logger::Logger() {
}

Logger::~Logger() {
}

void Logger::Init( const wxString& filename, const wxString& str )
{
	m_FileName = filename;
	m_FirstTick = wxDateTime::UNow().GetValue();
	m_OutStream.Open(filename, wxFile::write);

	SaveToFile( "======" + this->m_FirstTick.FormatISOCombined() + " LOG START" + str + "======", wxTimeSpan(0));
}

void	Logger::SaveToFile( const wxString& str, const wxTimeSpan& time )
{
	//	TODO - change format to one with miliseconds
	auto outStr = "[" + time.Format() + "]" + str;

	this->m_OutStream.Write(outStr);
	this->m_OutStream.Flush();
}

void	Logger::Log( const wxString& str )
{
	if( !this->m_OutStream.IsOpened() )
		return;

	SaveToFile( str, wxDateTime::UNow() - this->m_FirstTick );
}

void	Logger::Report( const wxString& str )
{
	if( !this->m_OutStream.IsOpened() )
		return;

	SaveToFile( "[R]" + str, wxDateTime::UNow() - this->m_FirstTick );	
}

void	Logger::Error( const wxString& str )
{
	if( !this->m_OutStream.IsOpened() )
		return;

	SaveToFile( "[!]" + str, wxDateTime::UNow() - this->m_FirstTick );
}

void	Logger::FatalError( const wxString& str )
{
	if( !this->m_OutStream.IsOpened() )
		return;

	SaveToFile( "[!!!]" + str, wxDateTime::UNow() - this->m_FirstTick );
	wxMessageBox(str, _("FATAL ERROR"));
}

void	Logger::Note( const wxString& str )
{
	if( !this->m_OutStream.IsOpened() )
		return;

	SaveToFile( "[N]" + str, wxDateTime::UNow() - this->m_FirstTick );
}