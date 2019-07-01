#include "stdafx.h"

#include "OpenGLVertexShader.h"
#include "HelperMethods.h"

#include <GL/glew.h>

namespace
{

	std::string LoadCode(const std::string& path)
	{
		std::fstream file(path, std::ios::in | std::ios::ate);

		if (!file.is_open())
		{
			spdlog::warn("Failed to open shader at {}", path);
			return "";
		}

		size_t size = file.tellg();

		std::vector<char> content(size);
		file.seekg(0);

		file.read(content.data(), size);

		return content.data();
	}
}

OpenGLVertexShader::OpenGLVertexShader(const std::string& path) :
	VertexShader(path)
{
	this->Compile();
}

OpenGLVertexShader::~OpenGLVertexShader()
{

}

void OpenGLVertexShader::Bind() const
{

}

void OpenGLVertexShader::Unbind() const
{

}

int OpenGLVertexShader::Compile()
{
	GLCALL(m_id = glCreateShader(GL_VERTEX_SHADER));
	std::string source = LoadCode(m_path);
	const char* code = source.c_str();

	GLCALL(glShaderSource(m_id, 1, &code, nullptr));
	GLCALL(glCompileShader(m_id));

	int result = GL_FALSE;
	int infoLength = 0;
	GLCALL(glGetShaderiv(m_id, GL_COMPILE_STATUS, &result));
	GLCALL(glGetShaderiv(m_id, GL_INFO_LOG_LENGTH, &infoLength));

	if (infoLength > 0)
	{
		std::string message;
		message.resize(infoLength);
		GLCALL(glGetShaderInfoLog(m_id, infoLength, nullptr, message.data()));
		spdlog::error(message);
	}

	return result;
}

unsigned int OpenGLVertexShader::GetId() const
{
	return m_id;
}