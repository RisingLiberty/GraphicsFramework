#include "stdafx.h"

#include "HelperMethods.h"

void GLClearError()
{
	int err = glGetError();
	while (err != GL_NO_ERROR) { err = glGetError(); std::cout << err << "\n"; }
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
