#include "stdafx.h"

#include "GLUploadUniformCommand.h"
#include "../GLHelperMethods.h"

GLUploadUniformCommand::GLUploadUniformCommand(int location, ShaderUniform* uniform):
	m_location(location),
	m_uniform(uniform)
{
}

GLUploadUniformCommand::~GLUploadUniformCommand()
{
}

std::string GLUploadUniformCommand::ToString() const
{
	return "Upload uniform";
}

void GLUploadUniformCommand::Execute()
{
	switch (m_uniform->type)
	{
	case UniformDataType::UNSPECIFIED:	ASSERT(false, "unspecified uniform data type"); break;
	case UniformDataType::INT:			GLCALL(glUniform1i(m_location, *(int*)m_uniform->data)); break;
	case UniformDataType::UNSIGNED_INT: GLCALL(glUniform1ui(m_location, *(unsigned int*)m_uniform->data)); break;
	case UniformDataType::FLOAT:		GLCALL(glUniform1f(m_location, *(float*)m_uniform->data)); break;
	case UniformDataType::VEC2: 
	{
		float* data = (float*)m_uniform->data;
		GLCALL(glUniform2f(m_location, data[0], data[1]));
		break;
	}
	case UniformDataType::VEC3:
	{
		float* data = (float*)m_uniform->data;
		GLCALL(glUniform3f(m_location, data[0], data[1], data[2]));
		break;
	}
	case UniformDataType::VEC4:
	{
		float* data = (float*)m_uniform->data;
		GLCALL(glUniform4f(m_location, data[0], data[1], data[2], data[3]));
		break;
	}
	case UniformDataType::MAT3:			GLCALL(glUniformMatrix3fv(m_location, 1, GL_FALSE, (float*)m_uniform->data)); break;
	case UniformDataType::MAT4:			GLCALL(glUniformMatrix4fv(m_location, 1, GL_FALSE, (float*)m_uniform->data)); break;

	default: ASSERT(false, "Invalid uniform data type"); break;
	}
}