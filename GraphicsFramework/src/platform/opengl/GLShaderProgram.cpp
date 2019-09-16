#include "stdafx.h"

#include "GLShaderProgram.h"
#include "GLVertexShader.h"
#include "GLFragmentShader.h"

#include "commands/GLAttachShaderCommand.h"
#include "commands/GLLinkProgramCommand.h"
#include "commands/GLValidateProgramCommand.h"
#include "commands/GLDetachShaderCommand.h"
#include "commands/GLLoadUniformsCommand.h"

#include "commands/GLCreateShaderProgramCommand.h"
#include "commands/GLDeleteShaderProgramCommand.h"

#include "commands/GLBindShaderProgramCommand.h"
#include "commands/GLUploadUniformCommand.h"

#include "commands/GLGetUniformLocationCommand.h"
#include "GLDirectCommandList.h"

GLShaderProgram::GLShaderProgram(VertexShader* vs, FragmentShader* fs) :
	ShaderProgram(vs, fs)
{
	GLVertexShader* gl_vs = vs->As<GLVertexShader>();
	GLFragmentShader* gl_fs = fs->As<GLFragmentShader>();

	assert(gl_vs);
	assert(gl_fs);

	this->Create({ gl_vs->GetId(), gl_fs->GetId() });
	this->LoadUniforms();
}


GLShaderProgram::~GLShaderProgram()
{
	GetGLContext()->ExecuteDirectCommand(std::make_unique<GLDeleteShaderProgramCommand>(m_id));
}

void GLShaderProgram::LoadUniforms()
{
	GetGLContext()->ExecuteDirectCommand(std::make_unique<GLLoadUniformsCommand>(m_id, m_uniforms));
}

void GLShaderProgram::Create(const std::vector<unsigned int>& shaderIDs)
{
	GetGLContext()->ExecuteDirectCommand(std::make_unique<GLCreateShaderProgramCommand>(&m_id));

	std::unique_ptr<GLDirectCommandList> direct_cmd_list = GetGLContext()->CreateDirectCommandList();

	for (unsigned int id : shaderIDs)
		direct_cmd_list->Push(std::make_unique<GLAttachShaderCommand>(m_id, id));

	direct_cmd_list->Push(std::make_unique<GLLinkProgramCommand>(m_id));
	direct_cmd_list->Push(std::make_unique<GLValidateProgramCommand>(m_id));

	for (unsigned int id : shaderIDs)
		direct_cmd_list->Push(std::make_unique<GLDetachShaderCommand>(m_id, id));
}

void GLShaderProgram::UploadVariables() const
{
	std::unique_ptr<GLDirectCommandList> direct_cmd_list = GetGLContext()->CreateDirectCommandList();
	direct_cmd_list->Push(std::make_unique<GLBindShaderProgramCommand>(this));

	for (const std::unique_ptr<ShaderUniform>& uniform : m_uniforms)
		direct_cmd_list->Push(std::make_unique<GLUploadUniformCommand>(GetUniformLocation(uniform->name), uniform.get()));
}

int GLShaderProgram::GetUniformLocation(const std::string& name) const
{
	if (m_uniform_location_cache.find(name) != m_uniform_location_cache.end())
		return m_uniform_location_cache[name];

	int location;
	GetGLContext()->ExecuteDirectCommand(std::make_unique<GLGetUniformLocationCommand>(m_id, name, &location));

	if (location == -1)
	{
		spdlog::warn("Uniform '{}' not found in shader or doesn't exists", name);
		return -1;
	}

	m_uniform_location_cache[name] = location;
	return location;
}

