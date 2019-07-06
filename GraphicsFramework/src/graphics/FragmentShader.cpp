#include "stdafx.h"

#include "FragmentShader.h"
#include "Context.h"

#include "platform/opengl/OpenGLFragmentShader.h"
#include "platform/directx11/Dx11FragmentShader.h"
#include "platform/directx12/Dx12FragmentShader.h"

#include "controllers/ShaderController.h"

FragmentShader* FragmentShader::Create(const std::string& shaderName)
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
	FragmentShader* shader = shader_controller->GetFragmentShader(path);

	if (shader)
		return shader;

	std::unique_ptr<FragmentShader> unique_shader;

	switch (Context::GetCurrent()->GetApiType())
	{
	case Context::API::OPENGL:
		unique_shader = std::make_unique<OpenGLFragmentShader>(path);
		shader = unique_shader.get();
		shader_controller->PushFragmentShader(unique_shader);
		break;
	case Context::API::DIRECTX11:
		unique_shader = std::make_unique<Dx11FragmentShader>(path);
		shader = unique_shader.get();
		shader_controller->PushFragmentShader(unique_shader);
		break;
	case Context::API::DIRECTX12:
		unique_shader = std::make_unique<Dx12FragmentShader>(path);
		shader = unique_shader.get();
		shader_controller->PushFragmentShader(unique_shader);
		break;
	}

	return shader;
}

FragmentShader::FragmentShader(const std::string& path):
	Shader(path)
{

}

FragmentShader::~FragmentShader()
{

}

Shader::Type FragmentShader::GetType() const
{
	return Type::VERTEX;
}