#include "stdafx.h"

#include "OpenGLContext.h"
#include "graphics/Window.h"

#include <Windows.h>

#include <GL/glew.h>

#include "OpenGLRenderer.h"
#include "OpenGLHelperMethods.h"

#include "OpenGLIndexBuffer.h"
#include "OpenGLVertexArray.h"
#include "OpenGLVertexBuffer.h"
#include "OpenGLShaderProgram.h"

using HandleGLRenderingContext = HGLRC;

OpenGLContext::OpenGLContext(Window* window)
{
	spdlog::info("Using OpenGL");

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

void OpenGLContext::BindIndexBufferInternal(const IndexBuffer* indexBuffer)
{
	const OpenGLIndexBuffer* ib = static_cast<const OpenGLIndexBuffer*>(indexBuffer);
	GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib->GetId()));
}

void OpenGLContext::UnbindIndexBufferInternal(const IndexBuffer* indexBuffer)
{
	GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

void OpenGLContext::BindVertexArrayInternal(const VertexArray* vertexArray)
{
	GLCALL(glBindBuffer(GL_ARRAY_BUFFER, static_cast<const OpenGLVertexBuffer*>(vertexArray->GetVertexBuffer())->GetId()));
	GLCALL(glBindVertexArray(static_cast<const OpenGLVertexArray*>(vertexArray)->GetId()));
}

void OpenGLContext::UnbindVertexArrayInternal(const VertexArray* vertexArray)
{
	GLCALL(glBindVertexArray(0));
}

void OpenGLContext::BindShaderProgramInternal(const ShaderProgram* shaderProgram)
{
	GLCALL(glUseProgram(static_cast<const OpenGLShaderProgram*>(shaderProgram)->GetId()));
}

void OpenGLContext::UnbindShaderProgramInternal(const ShaderProgram* shaderProgram)
{
	GLCALL(glUseProgram(0));
}