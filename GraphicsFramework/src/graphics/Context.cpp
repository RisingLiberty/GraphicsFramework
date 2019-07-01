#include "stdafx.h"

#include "Context.h"

#include "platform/opengl/OpenGLContext.h"

std::unique_ptr<Context> Context::s_current = nullptr;

Context::Context() = default;

Context::~Context() = default;

void Context::Create(API api, void* windowHandle)
{
	switch (api)
	{
	case Context::API::DirectX11:
		break;
	case Context::API::DirectX12:
		break;
	case Context::API::OpenGL:
		s_current = std::make_unique<OpenGLContext>(windowHandle);
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