#include "stdafx.h"

#include "OpenGLShaderProgram.h"
#include "OpenGLVertexShader.h"
#include "OpenGLFragmentShader.h"

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

OpenGLShaderProgram::OpenGLShaderProgram(VertexShader* vs, FragmentShader* fs) :
	ShaderProgram(vs, fs)
{
	OpenGLVertexShader* gl_vs = dynamic_cast<OpenGLVertexShader*>(vs);
	OpenGLFragmentShader* gl_fs = dynamic_cast<OpenGLFragmentShader*>(fs);

	assert(gl_vs);
	assert(gl_fs);

	this->Create({ gl_vs->GetId(), gl_fs->GetId() });
	this->LoadUniforms();
}


OpenGLShaderProgram::~OpenGLShaderProgram()
{
	GLCALL(glDeleteProgram(m_id));
}

void OpenGLShaderProgram::LoadUniforms()
{
	int count;
	GLCALL(glGetProgramiv(m_id, GL_ACTIVE_UNIFORMS, &count));

	std::vector<std::unique_ptr<ShaderUniform>> uniforms;

	for (int i = 0; i < count; ++i)
	{
		GLint size;
		GLenum type;

		const GLsizei bufSize = 16; //maximum name length
		GLchar name[bufSize]; //uniform name
		GLsizei nameLength; //uniform name length

		GLCALL(glGetActiveUniform(m_id, i, bufSize, &nameLength, &size, &type, name));

		uniforms.push_back(std::make_unique<ShaderUniform>(name, GLUniformDataTypeToCustomDataType(type), size));
	}

	m_uniforms = std::move(uniforms);
}

void OpenGLShaderProgram::Create(const std::vector<unsigned int>& shaderIDs)
{
	GLCALL(m_id = glCreateProgram());

	for (unsigned int id : shaderIDs)
	{
		GLCALL(glAttachShader(m_id, id));
	}

	GLCALL(glLinkProgram(m_id));
	GLCALL(glValidateProgram(m_id));

	int result = GL_FALSE;
	int messageLength = 0;

	GLCALL(glGetProgramiv(m_id, GL_LINK_STATUS, &result));
	GLCALL(glGetProgramiv(m_id, GL_INFO_LOG_LENGTH, &messageLength));

	if (messageLength > 0)
	{
		std::string error;
		error.resize(messageLength);
		GLCALL(glGetProgramInfoLog(m_id, messageLength, nullptr, error.data()));
		spdlog::error(error);
		return;
	}
}

void OpenGLShaderProgram::Bind() const
{
	GLCALL(glUseProgram(m_id));
}

void OpenGLShaderProgram::Unbind() const
{
	GLCALL(glUseProgram(0));
}

void OpenGLShaderProgram::UploadVariables()
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

void OpenGLShaderProgram::UploadBoolUniform(const std::string& name, bool uniform)
{
	GLCALL(glUniform1i(GetUniformLocation(name), uniform));
}

void OpenGLShaderProgram::UploadUShortUniform(const std::string& name, unsigned short & uniform)
{
	GLCALL(glUniform1ui(GetUniformLocation(name), uniform));
}

void OpenGLShaderProgram::UploadShortUniform(const std::string& name, short uniform)
{
	GLCALL(glUniform1i(GetUniformLocation(name), uniform));
}

void OpenGLShaderProgram::UploadIntUniform(const std::string& name, int uniform)
{
	GLCALL(glUniform1i(GetUniformLocation(name), uniform));
}

void OpenGLShaderProgram::UploadUIntUniform(const std::string& name, unsigned int & uniform)
{
	GLCALL(glUniform1ui(GetUniformLocation(name), uniform));
}

void OpenGLShaderProgram::UploadFloatUniform(const std::string& name, float uniform)
{
	GLCALL(glUniform1f(GetUniformLocation(name), uniform));
}

void OpenGLShaderProgram::UploadDoubleUniform(const std::string& name, double uniform)
{
	GLCALL(glUniform1d(GetUniformLocation(name), uniform));
}

void OpenGLShaderProgram::UploadVec2Uniform(const std::string& name, float* values)
{
	GLCALL(glUniform2f(GetUniformLocation(name), values[0], values[1]));
}

void OpenGLShaderProgram::UploadVec3Uniform(const std::string& name, float* values)
{
	GLCALL(glUniform3f(GetUniformLocation(name), values[0], values[1], values[2]));
}

void OpenGLShaderProgram::UploadVec4Uniform(const std::string& name, float* values)
{
	GLCALL(glUniform4f(GetUniformLocation(name), values[0], values[1], values[2], values[3]));
}

void OpenGLShaderProgram::UploadMat3Uniform(const std::string& name, float* values)
{
	GLCALL(glUniformMatrix3fv(GetUniformLocation(name), 1, GL_FALSE, values));
}

void OpenGLShaderProgram::UploadMat4Uniform(const std::string& name, float* values)
{
	GLCALL(glUniformMatrix3fv(GetUniformLocation(name), 1, GL_FALSE, values));
}

int OpenGLShaderProgram::GetUniformLocation(const std::string& name)
{
	if (m_uniform_location_cache.find(name) != m_uniform_location_cache.end())
		return m_uniform_location_cache[name];

	GLCALL(int location = glGetUniformLocation(m_id, name.c_str()));
	if (location == -1)
	{
		spdlog::warn("Uniform '{}' not found in shader or doesn't exists", name);
		return -1;
	}

	m_uniform_location_cache[name] = location;
	return location;
}

