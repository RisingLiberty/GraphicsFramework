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

#include "OpenGLCommandQueue.h"
#include "OpenGLCommandList.h"

OpenGLContext::OpenGLContext(Window* window):
	m_window(window)
{
	spdlog::info("Using OpenGL");

	m_hdc = GetDC((HWND)window->GetHandle());

	// Handle to GL Rendering Context
	m_hglrc = wglCreateContext(m_hdc);
	if (m_hglrc)
	{
		if (!wglMakeCurrent(m_hdc, m_hglrc))
			ThrowContextException("Failed to set OpenGL context!");
	}
	else
		ThrowContextException("Failed to create OpenGL context!");

	if (glewInit())
	{
		ThrowException("Failed to initialize glew");
	}
}

OpenGLContext::~OpenGLContext()
{
	this->CleanUp();
	wglMakeCurrent(NULL, NULL);
	ReleaseDC((HWND)m_window->GetHandle(), m_hdc);
	wglDeleteContext(m_hglrc);

	wglGetCurrentContext();
	wglGetCurrentDC();
}

void OpenGLContext::Initialize()
{
	m_renderer = std::make_unique<OpenGLRenderer>();
	m_command_queue = std::make_unique<OpenGLCommandQueue>();
	m_command_list = m_command_queue->GetCommandList();

	this->InitializeRasterizer();
}

void OpenGLContext::Present()
{
	SwapBuffers(m_hdc);
}

API OpenGLContext::GetApiType() const
{
	return API::OPENGL;
}

void OpenGLContext::InitializeRasterizer()
{
	m_command_list->SetRasterizerState(m_rasterizer_settings);
}

OpenGLCommandList* OpenGLContext::GetCommandList() const
{
	return m_command_list;
}

void OpenGLContext::BindIndexBufferInternal(const IndexBuffer* indexBuffer)
{
	const OpenGLIndexBuffer* ib = static_cast<const OpenGLIndexBuffer*>(indexBuffer);
	m_command_list->BindIndexBuffer(ib->GetId());
}

void OpenGLContext::UnbindIndexBufferInternal(const IndexBuffer* indexBuffer)
{
	m_command_list->BindIndexBuffer(0);
}

void OpenGLContext::BindVertexArrayInternal(const VertexArray* vertexArray)
{
	static_cast<const OpenGLVertexBuffer*>(vertexArray->GetVertexBuffer())->GLBind();
	m_command_list->BindVertexArray(static_cast<const OpenGLVertexArray*>(vertexArray)->GetId());
}

void OpenGLContext::UnbindVertexArrayInternal(const VertexArray* vertexArray)
{
	m_command_list->BindVertexArray(0);
}

void OpenGLContext::BindShaderProgramInternal(const ShaderProgram* shaderProgram)
{
	m_command_list->BindShaderProgram(static_cast<const OpenGLShaderProgram*>(shaderProgram)->GetId());
}

void OpenGLContext::UnbindShaderProgramInternal(const ShaderProgram* shaderProgram)
{
	m_command_list->BindShaderProgram(0);
}