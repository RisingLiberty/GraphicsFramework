#include "stdafx.h"

#include "GLHelperMethods.h"

void GLClearError()
{
	int err = glGetError();
	while (err != GL_NO_ERROR) 
	{ 
		spdlog::error("GL error: {}", err); 
		err = glGetError(); 
	}
}

bool GLLogCall(const char* glFunction, const char* function, const char* file, int line)
{
	while (GLenum error = glGetError())
	{
		spdlog::error("[GL Error] ({}) {} in {}: {}", error, function, file, line);
		return false;
	}
	return true;
}

GLContext* GetGLContext()
{
	return static_cast<GLContext*>(Context::GetCurrent());
}

GLCommandList* GetGLCommandList()
{
	return GetGLContext()->GetCommandList();
}