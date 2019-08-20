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
	m_command_queue = std::make_unique<OpenGLCommandQueue>(MAX_NR_OF_FRAMES_IN_FLIGHT);
	m_command_list = m_command_queue->As<OpenGLCommandQueue>()->GetCommandList();

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
	m_command_list->As<OpenGLCommandList>()->SetRasterizerState(m_rasterizer_settings);
}

OpenGLCommandList* OpenGLContext::GetCommandList() const
{
	return m_command_list->As<OpenGLCommandList>();
}

void OpenGLContext::BindIndexBufferInternal(const IndexBuffer* indexBuffer)
{
	const OpenGLIndexBuffer* ib = indexBuffer->As<OpenGLIndexBuffer>();
	m_command_list->As<OpenGLCommandList>()->BindIndexBuffer(ib->GetId());
}

void OpenGLContext::UnbindIndexBufferInternal(const IndexBuffer* indexBuffer)
{
	m_command_list->As<OpenGLCommandList>()->BindIndexBuffer(0);
}

void OpenGLContext::BindVertexArrayInternal(const VertexArray* vertexArray)
{
	vertexArray->GetVertexBuffer()->As<OpenGLVertexBuffer>()->GLBind();
	m_command_list->As<OpenGLCommandList>()->BindVertexArray(vertexArray->As<OpenGLVertexArray>()->GetId());
}

void OpenGLContext::UnbindVertexArrayInternal(const VertexArray* vertexArray)
{
	m_command_list->As<OpenGLCommandList>()->BindVertexArray(0);
}

void OpenGLContext::BindShaderProgramInternal(const ShaderProgram* shaderProgram)
{
	m_command_list->As<OpenGLCommandList>()->BindShaderProgram(shaderProgram->As<OpenGLShaderProgram>()->GetId());
}

void OpenGLContext::UnbindShaderProgramInternal(const ShaderProgram* shaderProgram)
{
	m_command_list->As<OpenGLCommandList>()->BindShaderProgram(0);
}