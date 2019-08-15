#include "stdafx.h"

#include "OpenGLHelperMethods.h"

void GLClearError()
{
	int err = glGetError();
	while (err != GL_NO_ERROR) { spdlog::error("Opengl error: {}", err); err = glGetError(); }
}

bool GLLogCall(const char* glFunction, const char* function, const char* file, int line)
{
	while (GLenum error = glGetError())
	{
		spdlog::error("[OpenGL Error] ({}) {} in {}: {}", error, function, file, line);
		return false;
	}
	return true;
}

OpenGLContext* GetOpenGLContext()
{
	return static_cast<OpenGLContext*>(Context::GetCurrent());
}

OpenGLCommandList* GetOpenGLCommandList()
{
	return GetOpenGLContext()->GetCommandList();
}