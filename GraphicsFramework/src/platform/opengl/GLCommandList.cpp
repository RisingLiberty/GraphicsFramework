#include "stdafx.h"

#include "GLCommandList.h"
#include "GLHelperMethods.h"

namespace
{
	unsigned int* NULL_OFFSET = nullptr;
}

GLCommandList::GLCommandList()
{

}

GLCommandList::~GLCommandList()
{

}

unsigned int GLCommandList::CreateShader(unsigned int type)
{
	return glCreateShader(type);
}

unsigned int GLCommandList::CreateProgram()
{
	return glCreateProgram();
}

unsigned int GLCommandList::CreateVertexArray()
{
	unsigned int id;
	GLCALL(glGenVertexArrays(1, &id));
	return id;
}

void GLCommandList::DeleteShader(unsigned int id)
{
	GLCALL(glDeleteShader(id));
}

void GLCommandList::DeleteProgram(unsigned int id)
{
	GLCALL(glDeleteProgram(id));
}

void GLCommandList::DeleteVertexArray(unsigned int id)
{
	GLCALL(glDeleteVertexArrays(1, &id));
}

void GLCommandList::SetShaderSource(unsigned int id, const char* code)
{
	GLCALL(glShaderSource(id, 1, &code, nullptr));
}

void GLCommandList::CompileShader(unsigned int id)
{
	GLCALL(glCompileShader(id));
}

//void GLCommandList::AttachShader(unsigned int programId, unsigned int shaderId)
//{
//	GLCALL(glAttachShader(programId, shaderId));
//}
//
//void GLCommandList::LinkProgram(unsigned int id)
//{
//	GLCALL(glLinkProgram(id));
//}

void GLCommandList::ValidateProgram(unsigned int id)
{
	GLCALL(glValidateProgram(id));
}

//int GLCommandList::GetShaderCompileStatus(unsigned int id)
//{
//	int result = GL_FALSE;
//	GLCALL(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
//	return result;
//}
//
//int GLCommandList::GetShaderInfoLogLength(unsigned int id)
//{
//	int info_length = 0;
//	GLCALL(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &info_length));
//	return info_length;
//}
//
//std::string GLCommandList::GetShaderInfoLog(unsigned int id)
//{
//	std::string message;
//	int info_length = this->GetShaderInfoLogLength(id);
//	message.resize(info_length);
//
//	GLCALL(glGetShaderInfoLog(id, info_length, nullptr, message.data()));
//	return message;
//}

//int GLCommandList::GetProgramLinkStatus(unsigned int id)
//{
//	int result = 0;
//	GLCALL(glGetProgramiv(id, GL_LINK_STATUS, &result));
//	return result;
//}
//
//int GLCommandList::GetProgramInfoLength(unsigned int id)
//{
//	int message_length = 0;
//	GLCALL(glGetProgramiv(id, GL_INFO_LOG_LENGTH, &message_length));
//	return message_length;
//}
//
//std::string GLCommandList::GetProgramInfoLog(unsigned int id)
//{
//	std::string message;
//	int message_length = this->GetProgramInfoLength(id);
//
//	message.resize(message_length);
//	GLCALL(glGetProgramInfoLog(id, message_length, nullptr, message.data()));
//	return message;
//}
//
unsigned int GLCommandList::GetUniformLocation(unsigned int id, const char* name)
{
	return glGetUniformLocation(id, name);
}

void GLCommandList::BindShaderProgram(unsigned int id)
{
	GLCALL(glUseProgram(id));
}
