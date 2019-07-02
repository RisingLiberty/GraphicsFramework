#include "stdafx.h"

#include "Context.h"
#include "Renderer.h"

#include "platform/opengl/OpenGLContext.h"
#include "platform/directx11/Dx11Context.h"

#include "controllers/ShaderController.h"
#include "controllers/VertexArrayController.h"

std::unique_ptr<Context> Context::s_current = nullptr;

Context::Context()
{
	m_shader_controller = std::make_unique<ShaderController>();
	m_vertex_array_controller = std::make_unique<VertexArrayController>();
}

Context::~Context() = default;

void Context::Create(API api, Window* window)
{
	switch (api)
	{
	case Context::API::DirectX11:
		s_current = std::make_unique<Dx11Context>(window);
		break;
	case Context::API::DirectX12:
		break;
	case Context::API::OpenGL:
		s_current = std::make_unique<OpenGLContext>(window);
		break;
	case Context::API::Vulkan:
		break;
	default:
		break;
	}
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