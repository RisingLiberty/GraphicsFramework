#include "stdafx.h"

#include "Context.h"
#include "Renderer.h"

#include "platform/opengl/OpenGLContext.h"
#include "platform/directx11/Dx11Context.h"
#include "platform/directx12/Dx12Context.h"
#include "platform/vulkan/VkContext.h"

#include "IndexBuffer.h"
#include "VertexArray.h"

#include "controllers/ShaderController.h"
#include "controllers/VertexArrayController.h"
#include "controllers/VertexBufferController.h"
#include "controllers/VertexLayoutController.h"
#include "controllers/IndexBufferController.h"

std::unique_ptr<Context> Context::s_current = nullptr;

Context::Context()
{
	m_shader_controller = std::make_unique<ShaderController>();
	m_vertex_array_controller = std::make_unique<VertexArrayController>();
	m_vertex_buffer_controller = std::make_unique<VertexBufferController>();
	m_vertex_layout_controller = std::make_unique<VertexLayoutController>();
	m_index_buffer_controller = std::make_unique<IndexBufferController>();

	m_rasterizer_settings.InitializeAsDefault();
}

Context::~Context() = default;

void Context::Create(API api, Window* window)
{
	assert(!s_current);

	switch (api)
	{
	case Context::API::DIRECTX11: s_current = std::make_unique<Dx11Context>(window); break;
	case Context::API::DIRECTX12: s_current = std::make_unique<Dx12Context>(window); break;
	case Context::API::OPENGL: s_current = std::make_unique<OpenGLContext>(window); break;
	case Context::API::VULKAN: s_current = std::make_unique<VkContext>(window); break;
	}

	s_current->PreInitialize();
	s_current->Initialize();
}

void Context::PreInitialize()
{

}

void Context::PostInitialize()
{
}

void Context::Clear()
{
	m_bound_index_buffer = nullptr;
	m_bound_shader_program = nullptr;
	m_bound_vertex_array = nullptr;
}

void Context::BindIndexBuffer(const IndexBuffer* indexBuffer)
{
	if (!m_bound_index_buffer)
	{
		m_bound_index_buffer = indexBuffer;
		this->BindIndexBufferInternal(indexBuffer);
	}
	else if (m_bound_index_buffer->GetResourceId() != indexBuffer->GetResourceId())
	{
		m_bound_index_buffer = indexBuffer;
		this->BindIndexBufferInternal(indexBuffer);
	}
	else
		spdlog::warn("Trying to bind the same index buffer again!");
}

void Context::UnbindVertexArray(const VertexArray* vertexArray)
{
	if (m_bound_vertex_array == vertexArray)
	{
		m_bound_vertex_array = nullptr;
		this->UnbindVertexArrayInternal(vertexArray);
	}
	else
		spdlog::warn("Trying to unbind vertex array that's currently not bound!");
}

void Context::BindVertexArray(const VertexArray* vertexArray)
{
	if (!m_bound_vertex_array)
	{
		const IndexBuffer* ib = m_bound_index_buffer;
		this->UnbindIndexBuffer(m_bound_index_buffer);
		//m_bound_index_buffer = nullptr; // reset index buffer
		m_bound_vertex_array = vertexArray;
		if (ib)
			this->BindIndexBuffer(ib);
		this->BindVertexArrayInternal(vertexArray);
	}
	else if (m_bound_vertex_array->GetResourceId() != vertexArray->GetResourceId())
	{
		const IndexBuffer* ib = m_bound_index_buffer;
		this->UnbindIndexBuffer(m_bound_index_buffer);
		//m_bound_index_buffer = nullptr; // reset index buffer
		m_bound_vertex_array = vertexArray;
		if (ib)
			this->BindIndexBuffer(ib);
		this->BindVertexArrayInternal(vertexArray);
	}
	else
		spdlog::warn("Trying to bind the same vertex array again!");

	}

void Context::UnbindIndexBuffer(const IndexBuffer* indexBuffer)
{
	if (m_bound_index_buffer == indexBuffer)
	{
		m_bound_index_buffer = nullptr;
		this->UnbindIndexBufferInternal(indexBuffer);
	}
	else
		spdlog::warn("Trying to unbind index buffer that's currently not bound!");
}

void Context::BindShaderProgram(const ShaderProgram* shaderProgram)
{
	if (!m_bound_shader_program)
	{
		m_bound_shader_program = shaderProgram;
		this->BindShaderProgramInternal(shaderProgram);
	}
	else if (m_bound_shader_program != shaderProgram)
	{
		m_bound_shader_program = shaderProgram;
		this->BindShaderProgramInternal(shaderProgram);
	}
	else 
		spdlog::warn("Trying to bind the same shader program again!");

}

void Context::UnbindShaderProgram(const ShaderProgram* shaderProgram)
{
	if (m_bound_shader_program == shaderProgram)
	{
		m_bound_shader_program = nullptr;
		this->UnbindShaderProgramInternal(shaderProgram);
	}
	else
		spdlog::warn("Tring to unbind shader program that's currently not bound!");
}

Context* Context::GetCurrent()
{
	return s_current.get();
}

Context::API Context::GetApi()
{
	return Context::GetCurrent()->GetApiType();
}

Renderer* Context::GetRenderer() const
{
	return m_renderer.get();
}

ShaderController* Context::GetShaderController() const
{
	return m_shader_controller.get();
}

VertexArrayController* Context::GetVertexArrayController() const
{
	return m_vertex_array_controller.get();
}

IndexBufferController* const Context::GetIndexBufferController() const
{
	return m_index_buffer_controller.get();
}

VertexBufferController* const Context::GetVertexBufferController() const
{
	return m_vertex_buffer_controller.get();
}

VertexLayoutController* const Context::GetVertexLayoutController() const
{
	return m_vertex_layout_controller.get();
}
