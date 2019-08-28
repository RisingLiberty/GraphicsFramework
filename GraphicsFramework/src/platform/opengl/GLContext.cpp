#include "stdafx.h"

#include "GLContext.h"
#include "graphics/Window.h"

#include <Windows.h>

#include <GL/glew.h>

#include "GLRenderer.h"
#include "GLHelperMethods.h"

#include "GLIndexBuffer.h"
#include "GLVertexArray.h"
#include "GLVertexBuffer.h"
#include "GLShaderProgram.h"

#include "GLCommandQueue.h"
#include "GLCommandList.h"

#include "GLCommands.h"

GLContext::GLContext(Window* window):
	m_window(window)
{
	spdlog::info("Using OpenGL");

	m_hdc = GetDC((HWND)window->GetHandle());

	// Handle to GL Rendering Context
	m_hglrc = wglCreateContext(m_hdc);
	if (m_hglrc)
	{
		if (!wglMakeCurrent(m_hdc, m_hglrc))
			ThrowContextException("Failed to set GL context!");
	}
	else
		ThrowContextException("Failed to create GL context!");

	if (glewInit())
	{
		ThrowException("Failed to initialize glew");
	}
}

GLContext::~GLContext()
{
	m_command_list->Open();
	this->CleanUp();
	m_command_list->Close();
	m_command_list->Execute();

	wglMakeCurrent(NULL, NULL);
	ReleaseDC((HWND)m_window->GetHandle(), m_hdc);
	wglDeleteContext(m_hglrc);

	wglGetCurrentContext();
	wglGetCurrentDC();
}

void GLContext::Initialize()
{
	m_renderer = std::make_unique<GLRenderer>();
	m_command_queue = std::make_unique<GLCommandQueue>(MAX_NR_OF_FRAMES_IN_FLIGHT);
	m_command_list = m_command_queue->As<GLCommandQueue>()->GetCommandList();
	m_command_list->Open();

	this->InitializeRasterizer();
}

void GLContext::Begin()
{
	m_command_list->Open();
}

void GLContext::Present()
{
	SwapBuffers(m_hdc);
}

void GLContext::End()
{
	m_command_list->Close();
	m_command_list->Execute();
}

API GLContext::GetApiType() const
{
	return API::OPENGL;
}

void GLContext::InitializeRasterizer()
{
	m_command_list->As<GLCommandList>()->SetRasterizerState(m_rasterizer_settings);
}

GLCommandList* GLContext::GetCommandList() const
{
	return m_command_list->As<GLCommandList>();
}

void GLContext::BindIndexBufferInternal(const IndexBuffer* indexBuffer)
{
	m_command_list->Push(std::make_unique<GLBindIndexBufferCommand>(indexBuffer));
}

void GLContext::UnbindIndexBufferInternal(const IndexBuffer* indexBuffer)
{
	//m_command_list->As<GLCommandList>()->BindIndexBuffer(0);
}

void GLContext::BindVertexArrayInternal(const VertexArray* vertexArray)
{
	m_command_list->Push(std::make_unique<GLBindVertexArrayCommand>(vertexArray));
}

void GLContext::UnbindVertexArrayInternal(const VertexArray* vertexArray)
{
	//m_command_list->As<GLCommandList>()->BindVertexArray(0);
}

void GLContext::BindShaderProgramInternal(const ShaderProgram* shaderProgram)
{
	m_command_list->As<GLCommandList>()->BindShaderProgram(shaderProgram->As<GLShaderProgram>()->GetId());
}

void GLContext::UnbindShaderProgramInternal(const ShaderProgram* shaderProgram)
{
	m_command_list->As<GLCommandList>()->BindShaderProgram(0);
}