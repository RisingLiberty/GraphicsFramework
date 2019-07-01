#include "stdafx.h"

#include "OpenGLShaderProgram.h"

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
		case GL_FLOAT_MAT3: return UniformDataType::MAT33; 
		case GL_FLOAT_MAT4: return UniformDataType::MAT44; 
		}

		return UniformDataType::UNSPECIFIED;
	}
}

OpenGLShaderProgram::OpenGLShaderProgram()
{
}


OpenGLShaderProgram::~OpenGLShaderProgram()
{
	GLCALL(glDeleteProgram(m_id));
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

void OpenGLShaderProgram::UnBind() const
{
	GLCALL(glUseProgram(0));
}

int OpenGLShaderProgram::GetUniformLocation(const std::string& name)
{
	if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
		return m_UniformLocationCache[name];

	GLCALL(int location = glGetUniformLocation(m_id, name.c_str()));
	if (location == -1)
	{
		spdlog::warn("Uniform '{}' not found in shader or doesn't exists", name);
		return -1;
	}

	m_UniformLocationCache[name] = location;
	return location;
}

std::vector<ShaderUniform> OpenGLShaderProgram::GetUniforms() const
{
	int count;
	GLCALL(glGetProgramiv(m_id, GL_ACTIVE_UNIFORMS, &count));

	std::vector<ShaderUniform> uniforms;

	for (int i = 0; i < count; ++i)
	{
		GLint Count;
		GLenum type;

		const GLsizei bufSize = 16; //maximum name length
		GLchar name[bufSize]; //uniform name
		GLsizei nameLength; //uniform name length

		GLCALL(glGetActiveUniform(m_id, i, bufSize, &nameLength, &Count, &type, name));

		uniforms.push_back(ShaderUniform(Count, GLUniformDataTypeToCustomDataType(type), name));
	}

	return uniforms;
}
