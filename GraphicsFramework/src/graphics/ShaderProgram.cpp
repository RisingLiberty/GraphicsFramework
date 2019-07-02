#include "stdafx.h"

#include "ShaderProgram.h"

#include "Context.h"
#include "controllers/ShaderController.h"

#include "platform/opengl/OpenGLShaderProgram.h"

ShaderProgram::ShaderProgram(VertexShader* vertexShader, FragmentShader* fragmentShader):
	m_vertex_shader(vertexShader),
	m_fragment_shader(fragmentShader)
{

}

ShaderProgram::~ShaderProgram()
{

}

VertexShader* ShaderProgram::GetVertexShader() const
{
	return m_vertex_shader;
}

FragmentShader* ShaderProgram::GetFragmentShader() const
{
	return m_fragment_shader;
}

ShaderProgram* ShaderProgram::Create(VertexShader* vs, FragmentShader* fs)
{
	ShaderController* shader_controller = Context::GetCurrent()->GetShaderController();
	ShaderProgram* program = shader_controller->GetShaderProgram(vs, fs);

	if (program)
		return program;

	switch (Context::GetCurrent()->GetApiType())
	{
	case Context::API::OPENGL:
	{
		std::unique_ptr<ShaderProgram> unique_program = std::make_unique<OpenGLShaderProgram>(vs, fs);
		program = unique_program.get();
		shader_controller->PushShaderProgram(unique_program);
	}
	}

	return program;
}
