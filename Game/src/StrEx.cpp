/*///////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
Plik:	StrEx.cpp
Autor:	Real_Noname (realnoname@coderulers.info)
(C):	CODE RULERS (Real_Noname)
WWW:	www.coderulers.info
Opis:	Patrz -> StrEx.h.

/////////////////////////////////////////////////////////
///////////////////////////////////////////////////////*/
#include "StrEx.h"
#include <sstream>

std::string IntToStr(const int x) 
{
	std::ostringstream oss;
	oss << x;
	return oss.str();
}

std::string FloatToStr(const float x) 
{
	std::ostringstream oss;
	oss << x;
	return oss.str();
}

std::string DoubleToStr(const double x) 
{
	std::ostringstream oss;
	oss << x;
	return oss.str();
}

std::string BoolToStr(const bool x) 
{
	if(x) 
		return "TRUE";
	return "FALSE";
}
