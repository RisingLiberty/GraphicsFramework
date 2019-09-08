#include "stdafx.h"

#include "GLShaderProgram.h"
#include "GLVertexShader.h"
#include "GLFragmentShader.h"

#include "GLAttachShaderCommand.h"
#include "GLLinkProgramCommand.h"
#include "GLValidateProgramCommand.h"
#include "GLDetachShaderCommand.h"
#include "GLLoadUniformsCommand.h"

#include "GLBindShaderProgramCommand.h"
#include "GLUploadUniformCommand.h"

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
	GetGLCommandList()->DeleteProgram(m_id);
}

void GLShaderProgram::LoadUniforms()
{
	std::unique_ptr<GLDirectCommandList> direct_cmd_list = GetGLContext()->CreateDirectCommandList();
	direct_cmd_list->Push(std::make_unique<GLLoadUniformsCommand>(m_id, m_uniforms));
}

void GLShaderProgram::Create(const std::vector<unsigned int>& shaderIDs)
{
	std::unique_ptr<GLDirectCommandList> direct_cmd_list = GetGLContext()->CreateDirectCommandList();
	m_id = direct_cmd_list->CreateProgram();

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
	direct_cmd_list->Push(std::make_unique<GLBindShaderProgramCommand>(m_id));

	for (const std::unique_ptr<ShaderUniform>& uniform : m_uniforms)
		direct_cmd_list->Push(std::make_unique<GLUploadUniformCommand>(GetUniformLocation(uniform->name), uniform.get()));
}

int GLShaderProgram::GetUniformLocation(const std::string& name) const
{
	if (m_uniform_location_cache.find(name) != m_uniform_location_cache.end())
		return m_uniform_location_cache[name];

	int location = GetGLCommandList()->GetUniformLocation(m_id, name.c_str());
	if (location == -1)
	{
		spdlog::warn("Uniform '{}' not found in shader or doesn't exists", name);
		return -1;
	}

	m_uniform_location_cache[name] = location;
	return location;
}

