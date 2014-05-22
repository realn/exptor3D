#pragma once

#define GLEW_STATIC

#include <gl/glew.h>
#ifdef _WIN32
#include <gl/wglew.h>
#else
#include <GL/glxew.h>
#endif

#include <wx/wxprec.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <fstream>