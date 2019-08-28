#include "stdafx.h"

#include "VertexShader.h"

#include "Context.h"

#include "platform/opengl/GLVertexShader.h"
#include "platform/directx11/Dx11VertexShader.h"
#include "platform/directx12/Dx12VertexShader.h"
#include "platform/vulkan/VkVertexShader.h"

#include "controllers/ShaderController.h"

VertexShader::VertexShader(const std::string& path):
	Shader(path)
{
}

VertexShader::~VertexShader()
{

}

VertexShader* VertexShader::Create(const std::string& shaderName)
{
	ASSERT(shaderName.find(".") == std::string::npos, "shader name can't have an extention!");

	std::string path = "data/shaders/";

	switch (Context::GetApi())
	{
	case API::OPENGL:
		path += "opengl/" + shaderName + ".glsl";
		break;
	case API::DIRECTX11:
		path += "directx11/" + shaderName + ".hlsl";
		break;
	case API::DIRECTX12:
		path += "directx12/" + shaderName + ".hlsl";
		break;
	case API::VULKAN:
		path += "vulkan/bin/" + shaderName + ".spv";
		break;
	}

	ShaderController* shader_controller = Context::GetCurrent()->GetShaderController();
	VertexShader* shader = shader_controller->GetVertexShader(path);

	if (shader)
		return shader;

	std::unique_ptr<VertexShader> unique_shader;

	switch (Context::GetCurrent()->GetApiType())
	{
	case API::OPENGL:
		unique_shader = std::make_unique<GLVertexShader>(path);
		shader = unique_shader.get();
		shader_controller->PushVertexShader(unique_shader);
		break;
	case API::DIRECTX11:
		unique_shader = std::make_unique<Dx11VertexShader>(path);
		shader = unique_shader.get();
		shader_controller->PushVertexShader(unique_shader);
		break;
	case API::DIRECTX12:
		unique_shader = std::make_unique<Dx12VertexShader>(path);
		shader = unique_shader.get();
		shader_controller->PushVertexShader(unique_shader);
		break;
	case API::VULKAN:
		unique_shader = std::make_unique<VkVertexShader>(path);
		shader = unique_shader.get();
		shader_controller->PushVertexShader(unique_shader);
		break;
	}

	return shader;
}

Shader::Type VertexShader::GetType() const
{
	return Type::VERTEX;
}