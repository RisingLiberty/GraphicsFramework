#include "stdafx.h"

#include "VertexShader.h"

#include "Context.h"

#include "platform/opengl/OpenGLVertexShader.h"
#include "platform/directx11/Dx11VertexShader.h"
#include "platform/directx12/Dx12VertexShader.h"

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
	case Context::API::OPENGL:
		path += "opengl/" + shaderName + ".glsl";
		break;
	case Context::API::DIRECTX11:
		path += "directx11/" + shaderName + ".hlsl";
		break;
	case Context::API::DIRECTX12:
		path += "directx12/" + shaderName + ".hlsl";
		break;
	}

	ShaderController* shader_controller = Context::GetCurrent()->GetShaderController();
	VertexShader* shader = shader_controller->GetVertexShader(path);

	if (shader)
		return shader;

	std::unique_ptr<VertexShader> unique_shader;

	switch (Context::GetCurrent()->GetApiType())
	{
	case Context::API::OPENGL:
		unique_shader = std::make_unique<OpenGLVertexShader>(path);
		shader = unique_shader.get();
		shader_controller->PushVertexShader(unique_shader);
		break;
	case Context::API::DIRECTX11:
		unique_shader = std::make_unique<Dx11VertexShader>(path);
		shader = unique_shader.get();
		shader_controller->PushVertexShader(unique_shader);
		break;
	case Context::API::DIRECTX12:
		unique_shader = std::make_unique<Dx12VertexShader>(path);
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