#include "stdafx.h"

#include "VertexShader.h"

#include "Context.h"

#include "platform/opengl/OpenGLVertexShader.h"

#include "controllers/ShaderController.h"

VertexShader::VertexShader(const std::string& path):
	Shader(path)
{
}

VertexShader::~VertexShader()
{

}

VertexShader* VertexShader::Create(const std::string& path)
{
	ShaderController* shader_controller = Context::GetCurrent()->GetShaderController();
	VertexShader* shader = shader_controller->GetVertexShader(path);

	if (shader)
		return shader;

	switch (Context::GetCurrent()->GetApiType())
	{
	case Context::API::OpenGL:
	{
		std::unique_ptr<VertexShader> unique_shader = std::make_unique<OpenGLVertexShader>(path);
		shader = unique_shader.get();
		shader_controller->PushVertexShader(unique_shader);
	}
	}

	return shader;
}