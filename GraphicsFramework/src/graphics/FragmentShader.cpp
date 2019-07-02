#include "stdafx.h"

#include "FragmentShader.h"
#include "Context.h"

#include "platform/opengl/OpenGLFragmentShader.h"

#include "controllers/ShaderController.h"

FragmentShader::FragmentShader(const std::string& path):
	Shader(path)
{

}

FragmentShader::~FragmentShader()
{

}

FragmentShader* FragmentShader::Create(const std::string& path)
{
	ShaderController* shader_controller = Context::GetCurrent()->GetShaderController();
	FragmentShader* shader = shader_controller->GetFragmentShader(path);

	if (shader)
		return shader;

	switch (Context::GetCurrent()->GetApiType())
	{
	case Context::API::OpenGL:
	{
		std::unique_ptr<FragmentShader> unique_shader = std::make_unique<OpenGLFragmentShader>(path);
		shader = unique_shader.get();
		shader_controller->PushFragmentShader(unique_shader);
	}
	}

	return shader;
}