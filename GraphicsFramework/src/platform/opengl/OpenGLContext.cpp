#include "stdafx.h"

#include "OpenGLContext.h"

#include <Windows.h>

#include <GL/glew.h>

using HandleGLRenderingContext = HGLRC;

OpenGLContext::OpenGLContext(void* windowHandle)
{
	m_hdc = GetDC((HWND)windowHandle);

	// Handle to GL Rendering Context
	HandleGLRenderingContext hrc = wglCreateContext(m_hdc);
	if (hrc)
	{
		if (!wglMakeCurrent(m_hdc, hrc))
			ThrowContextException(L"Failed to set OpenGL context!");
	}
	else
		ThrowContextException(L"Failed to create OpenGL context!");

	if (glewInit())
	{
		ThrowException(L"Failed to initialize glew");
	}
}

OpenGLContext::~OpenGLContext() = default;

void OpenGLContext::Present()
{
	SwapBuffers(m_hdc);
}

OpenGLContext::API OpenGLContext::GetApiType() const
{
	return API::OpenGL;
}