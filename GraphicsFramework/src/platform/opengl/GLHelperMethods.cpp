#include "stdafx.h"

#include "GLHelperMethods.h"

std::string GLErrorToString(int error)
{
	switch (error)
	{
	case GL_INVALID_ENUM: return "Invalid enum";
	case GL_INVALID_VALUE: return "Invalid value";
	case GL_INVALID_OPERATION: return "Invalid operation";
	case GL_STACK_OVERFLOW: return "Stack overflow";
	case GL_STACK_UNDERFLOW: return "Stack underflow";
	case GL_OUT_OF_MEMORY: return "Out of memory";

	}
	return "Undefined error";
}

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
		spdlog::error("GL Error: {}", GLErrorToString(error));
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