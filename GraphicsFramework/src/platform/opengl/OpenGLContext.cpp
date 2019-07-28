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

	this->InitializeRasterizer();
}

void OpenGLContext::Present()
{
	SwapBuffers(m_hdc);
}

OpenGLContext::API OpenGLContext::GetApiType() const
{
	return API::OPENGL;
}

void OpenGLContext::InitializeRasterizer()
{
	m_rasterizer_settings.enable_depth_clamp ? glEnable(GL_DEPTH_CLAMP) : glDisable(GL_DEPTH_CLAMP);
	m_rasterizer_settings.is_discarded ? glEnable(GL_RASTERIZER_DISCARD) : glDisable(GL_RASTERIZER_DISCARD);
	
	PolygonMode polygon_mode = m_rasterizer_settings.polygon_mode;
	CullMode cull_mode = m_rasterizer_settings.cull_mode;

	glPolygonMode(cull_mode.ToOpenGL(), polygon_mode.ToOpenGL());
	glLineWidth(m_rasterizer_settings.line_width);
	glFrontFace(m_rasterizer_settings.front_face_orientation.ToOpenGL());

	if (m_rasterizer_settings.enable_depth_bias)
	{
		switch (m_rasterizer_settings.polygon_mode)
		{
		case EPolygonMode::UNDEFINED:	glEnable(-1);
		case EPolygonMode::FILL:		glEnable(GL_POLYGON_OFFSET_FILL);
		case EPolygonMode::WIREFRAME:	glEnable(GL_POLYGON_OFFSET_LINE);
		case EPolygonMode::VERTEX:		glEnable(GL_POLYGON_OFFSET_POINT);
		}
		glPolygonOffset(m_rasterizer_settings.depth_bias_constant_factor, m_rasterizer_settings.depth_bias_slope_factor);
	}
	glDepthRangef(0, m_rasterizer_settings.depth_bias_clamp);
	m_rasterizer_settings.enable_multi_sample ? glEnable(GL_MULTISAMPLE) : glDisable(GL_MULTISAMPLE);

	if (m_rasterizer_settings.enable_scissor)
		spdlog::warn("OpenGL scissoring not implemented yet!");
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