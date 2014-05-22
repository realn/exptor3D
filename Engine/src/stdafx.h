#pragma once

#define GLEW_STATIC
#include <GL/glew.h>

#include <wx/wxprec.h>
#include <wx/wx.h>

#include <string>
#include <vector>

#ifdef _MSC_VER
#pragma comment(lib, "glew32sd.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#endif