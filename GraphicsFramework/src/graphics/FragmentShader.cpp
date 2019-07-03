#include "stdafx.h"

#include "FragmentShader.h"
#include "Context.h"

#include "platform/opengl/OpenGLFragmentShader.h"
#include "platform/directx11/Dx11FragmentShader.h"

#include "controllers/ShaderController.h"

FragmentShader::FragmentShader(const std::string& path):
	Shader(path)
{

}

FragmentShader::~FragmentShader()
{

}

FragmentShader* FragmentShader::Create(const std::string& shaderName)
{
	ASSERT(shaderName.find(".") == std::string::npos, "shader name can't have an extention!");

	std::string path = "data/shaders/";
	if (Context::GetApi() == Context::API::OPENGL)
		path += "opengl/" + shaderName + ".glsl";
	else
		path += "directx11/" + shaderName + ".hlsl";


	ShaderController* shader_controller = Context::GetCurrent()->GetShaderController();
	FragmentShader* shader = shader_controller->GetFragmentShader(path);

	if (shader)
		return shader;

	std::unique_ptr<FragmentShader> unique_shader;

	switch (Context::GetCurrent()->GetApiType())
	{
	case Context::API::OPENGL:
	{
		unique_shader = std::make_unique<OpenGLFragmentShader>(path);
		shader = unique_shader.get();
		shader_controller->PushFragmentShader(unique_shader);
		break;
	}
	case Context::API::DIRECTX11:
		unique_shader = std::make_unique<Dx11FragmentShader>(path);
		shader = unique_shader.get();
		shader_controller->PushFragmentShader(unique_shader);
		break;

	}

	return shader;
}

Shader::Type FragmentShader::GetType() const
{
	return Type::VERTEX;
}