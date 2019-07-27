#pragma once

#include <GL/glew.h>

#include "OpenGLContext.h"

void GLClearError();
bool GLLogCall(const char* glFunction, const char* function, const char* file, int line);

#define GLCALL(function)\
	GLClearError();\
	function;\
	ASSERT(GLLogCall(#function, __FUNCTION__, __FILE__, __LINE__),"")

OpenGLContext* GetOpenGLContext();