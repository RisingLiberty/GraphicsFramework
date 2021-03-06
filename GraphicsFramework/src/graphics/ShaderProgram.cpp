#include "stdafx.h"

#include "ShaderProgram.h"

#include "Context.h"
#include "controllers/ShaderController.h"

#include "platform/opengl/GLShaderProgram.h"
#include "platform/directx11/Dx11ShaderProgram.h"
#include "platform/directx12/Dx12ShaderProgram.h"
#include "platform/vulkan/VkShaderProgram.h"

ShaderProgram* ShaderProgram::Create(VertexShader* vs, FragmentShader* fs)
{
	ShaderController* shader_controller = Context::GetCurrent()->GetShaderController();
	ShaderProgram* program = shader_controller->GetShaderProgram(vs, fs);

	if (program)
		return program;

	std::unique_ptr<ShaderProgram> unique_program;
	switch (Context::GetCurrent()->GetApiType())
	{
	case API::OPENGL:
	{
		unique_program = std::make_unique<GLShaderProgram>(vs, fs);
		program = unique_program.get();
		shader_controller->PushShaderProgram(unique_program);
		break;
	}
	case API::DIRECTX11:
	{
		unique_program = std::make_unique<Dx11ShaderProgram>(vs, fs);
		program = unique_program.get();
		shader_controller->PushShaderProgram(unique_program);
		break;
	}
	case API::DIRECTX12:
	{
		unique_program = std::make_unique<Dx12ShaderProgram>(vs, fs);
		program = unique_program.get();
		shader_controller->PushShaderProgram(unique_program);
		break;
	}
	case API::VULKAN:
		unique_program = std::make_unique<VkShaderProgram>(vs, fs);
		program = unique_program.get();
		shader_controller->PushShaderProgram(unique_program);
		break;
	}

	return program;
}

ShaderProgram::ShaderProgram(VertexShader* vertexShader, FragmentShader* fragmentShader):
	m_vertex_shader(vertexShader),
	m_fragment_shader(fragmentShader)
{

}

ShaderProgram::~ShaderProgram()
{

}

void ShaderProgram::SetUniform(const std::string& name, void* data, unsigned int size) const
{
	for (std::unique_ptr<ShaderUniform>& uniform : m_uniforms)
	{
		if (uniform->name == name)
		{
			memcpy(uniform->data, data, size);
			return;
		}

	}
	ASSERT(false, "uniform not found in buffer");
}

void ShaderProgram::Bind() const
{
	Context::GetCurrent()->BindShaderProgram(this);
}

void ShaderProgram::Unbind() const
{
	Context::GetCurrent()->BindShaderProgram(this);
}

VertexShader* ShaderProgram::GetVertexShader() const
{
	return m_vertex_shader;
}

FragmentShader* ShaderProgram::GetFragmentShader() const
{
	return m_fragment_shader;
}

const std::vector<std::unique_ptr<ShaderUniform>>& ShaderProgram::GetUniforms() const
{
	return m_uniforms;
}


