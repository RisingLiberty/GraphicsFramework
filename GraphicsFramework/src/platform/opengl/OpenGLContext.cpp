#include "stdafx.h"

#include "OpenGLContext.h"
#include "graphics/Window.h"

#include <Windows.h>

#include <GL/glew.h>

#include "OpenGLRenderer.h"

using HandleGLRenderingContext = HGLRC;

OpenGLContext::OpenGLContext(Window* window)
{
	m_hdc = GetDC((HWND)window->GetHandle());

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

	m_renderer = std::make_unique<OpenGLRenderer>();

	this->InitializeImGui();
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

void OpenGLContext::InitializeImGui() const
{
	ImGui::CreateContext();
	ImGui_ImplOpenGL3_Init("#version 330");

}
