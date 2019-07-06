#include "stdafx.h"

#include "OpenGLContext.h"
#include "graphics/Window.h"

#include <Windows.h>

#include <GL/glew.h>

#include "OpenGLRenderer.h"

using HandleGLRenderingContext = HGLRC;

OpenGLContext::OpenGLContext(Window* window)
{
	spdlog::info("Using DirectX 11");

	m_hdc = GetDC((HWND)window->GetHandle());

	// Handle to GL Rendering Context
	HandleGLRenderingContext hrc = wglCreateContext(m_hdc);
	if (hrc)
	{
		if (!wglMakeCurrent(m_hdc, hrc))
			ThrowContextException("Failed to set OpenGL context!");
	}
	else
		ThrowContextException("Failed to create OpenGL context!");

	if (glewInit())
	{
		ThrowException("Failed to initialize glew");
	}
}

OpenGLContext::~OpenGLContext() = default;

void OpenGLContext::Initialize()
{
	m_renderer = std::make_unique<OpenGLRenderer>();
}

void OpenGLContext::Present()
{
	SwapBuffers(m_hdc);
}

OpenGLContext::API OpenGLContext::GetApiType() const
{
	return API::OPENGL;
}
