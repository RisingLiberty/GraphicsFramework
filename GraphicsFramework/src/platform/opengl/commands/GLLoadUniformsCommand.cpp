#include "stdafx.h"

#include "GLLoadUniformsCommand.h"
#include "../GLHelperMethods.h"

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

GLLoadUniformsCommand::GLLoadUniformsCommand(unsigned int programId, std::vector<std::unique_ptr<ShaderUniform>>& uniforms) :
	m_program_id(programId),
	m_uniforms(uniforms)
{

}

GLLoadUniformsCommand::~GLLoadUniformsCommand()
{

}

std::string GLLoadUniformsCommand::ToString() const
{
	return "Load uniforms";
}

void GLLoadUniformsCommand::Execute()
{
	int count;
	GLCALL(glGetProgramiv(m_program_id, GL_ACTIVE_UNIFORMS, &count));

	for (int i = 0; i < count; ++i)
	{
		GLint size;
		GLenum type;

		const GLsizei bufSize = 16; //maximum name length
		GLchar name[bufSize]; //uniform name
		GLsizei nameLength; //uniform name length

		GLCALL(glGetActiveUniform(m_program_id, i, bufSize, &nameLength, &size, &type, name));

		m_uniforms.push_back(std::make_unique<ShaderUniform>(name, GLUniformDataTypeToCustomDataType(type)));
	}
}