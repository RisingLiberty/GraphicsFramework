#include "stdafx.h"

#include "GLShaderProgram.h"
#include "GLVertexShader.h"
#include "GLFragmentShader.h"

namespace
{
	UniformDataType GLUniformDataTypeToCustomDataType(unsigned int type)
	{
		switch (type)
		{
		case GL_INT:		return UniformDataType::INT; 
		case GL_FLOAT:		return UniformDataType::FLOAT; 
		case GL_FLOAT_VEC2: return UniformDataType::VEC2; 
		case GL_FLOAT_VEC3: return UniformDataType::VEC3; 
		case GL_FLOAT_VEC4: return UniformDataType::VEC4; 
		case GL_FLOAT_MAT3: return UniformDataType::MAT3; 
		case GL_FLOAT_MAT4: return UniformDataType::MAT4; 
		}

		return UniformDataType::UNSPECIFIED;
	}
}

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
	int count;
	GLCALL(glGetProgramiv(m_id, GL_ACTIVE_UNIFORMS, &count));

	for (int i = 0; i < count; ++i)
	{
		GLint size;
		GLenum type;

		const GLsizei bufSize = 16; //maximum name length
		GLchar name[bufSize]; //uniform name
		GLsizei nameLength; //uniform name length

		GLCALL(glGetActiveUniform(m_id, i, bufSize, &nameLength, &size, &type, name));

		m_uniforms.push_back(std::make_unique<ShaderUniform>(name, GLUniformDataTypeToCustomDataType(type)));
	}
}

void GLShaderProgram::Create(const std::vector<unsigned int>& shaderIDs)
{
	m_id = GetGLCommandList()->CreateProgram();

	for (unsigned int id : shaderIDs)
		GetGLCommandList()->AttachShader(m_id, id);

	GetGLCommandList()->LinkProgram(m_id);
	GetGLCommandList()->ValidateProgram(m_id);

	std::string info_log = GetGLCommandList()->GetProgramInfoLog(m_id);
	if (!info_log.empty())
		spdlog::error(info_log);
}

void GLShaderProgram::UploadVariables() const
{
	for (const std::unique_ptr<ShaderUniform>& uniform : m_uniforms)
	{
		switch (uniform->type)
		{
		case UniformDataType::UNSPECIFIED: ASSERT(false, "uniform data type unspecified!");			break;
		case UniformDataType::INT:		UploadIntUniform(uniform->name, (int)*uniform->data);		break;
		case UniformDataType::FLOAT:	UploadFloatUniform(uniform->name, (float)*uniform->data);	break;
		case UniformDataType::VEC2:		UploadVec2Uniform(uniform->name, (float*)uniform->data);	break;
		case UniformDataType::VEC3:		UploadVec3Uniform(uniform->name, (float*)uniform->data);	break;
		case UniformDataType::VEC4:		UploadVec4Uniform(uniform->name, (float*)uniform->data);	break;
		case UniformDataType::MAT3:		UploadMat3Uniform(uniform->name, (float*)uniform->data);	break;
		case UniformDataType::MAT4:		UploadMat4Uniform(uniform->name, (float*)uniform->data);	break;
		}
	}
}

void GLShaderProgram::UploadBoolUniform(const std::string& name, bool uniform) const
{
	GLCALL(glUniform1i(GetUniformLocation(name), uniform));
}

void GLShaderProgram::UploadUShortUniform(const std::string& name, unsigned short & uniform) const
{
	GLCALL(glUniform1ui(GetUniformLocation(name), uniform));
}

void GLShaderProgram::UploadShortUniform(const std::string& name, short uniform) const
{
	GLCALL(glUniform1i(GetUniformLocation(name), uniform));
}

void GLShaderProgram::UploadIntUniform(const std::string& name, int uniform) const
{
	GLCALL(glUniform1i(GetUniformLocation(name), uniform));
}

void GLShaderProgram::UploadUIntUniform(const std::string& name, unsigned int & uniform) const
{
	GLCALL(glUniform1ui(GetUniformLocation(name), uniform));
}

void GLShaderProgram::UploadFloatUniform(const std::string& name, float uniform) const
{
	GLCALL(glUniform1f(GetUniformLocation(name), uniform));
}

void GLShaderProgram::UploadDoubleUniform(const std::string& name, double uniform) const
{
	GLCALL(glUniform1d(GetUniformLocation(name), uniform));
}

void GLShaderProgram::UploadVec2Uniform(const std::string& name, float* values) const
{
	GLCALL(glUniform2f(GetUniformLocation(name), values[0], values[1]));
}

void GLShaderProgram::UploadVec3Uniform(const std::string& name, float* values) const
{
	GLCALL(glUniform3f(GetUniformLocation(name), values[0], values[1], values[2]));
}

void GLShaderProgram::UploadVec4Uniform(const std::string& name, float* values) const
{
	GLCALL(glUniform4f(GetUniformLocation(name), values[0], values[1], values[2], values[3]));
}

void GLShaderProgram::UploadMat3Uniform(const std::string& name, float* values) const
{
	GLCALL(glUniformMatrix3fv(GetUniformLocation(name), 1, GL_FALSE, values));
}

void GLShaderProgram::UploadMat4Uniform(const std::string& name, float* values) const
{
	GLCALL(glUniformMatrix3fv(GetUniformLocation(name), 1, GL_FALSE, values));
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

