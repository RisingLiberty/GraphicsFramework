#include "stdafx.h"

#include "GLVertexShader.h"
#include "GLHelperMethods.h"

#include "GLCreateShaderCommand.h"
#include "GLCompileShaderCommand.h"
#include "GLSetShaderSourceCommand.h"
#include "GLDeleteShaderCommand.h"

#include "GLDirectCommandList.h"

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

GLVertexShader::GLVertexShader(const std::string& path) :
	VertexShader(path)
{
	GetGLContext()->ExecuteDirectCommand(std::make_unique<GLCreateShaderCommand>(ShaderType::VERTEX, &m_id));
	this->Compile();
}

GLVertexShader::~GLVertexShader()
{
	GetGLContext()->ExecuteDirectCommand(std::make_unique<GLDeleteShaderCommand>(m_id));
}

void GLVertexShader::Compile()
{
	std::string source = LoadCode(m_path);
	const char* code = source.c_str();

	std::unique_ptr<GLDirectCommandList> direct_cmd_list = GetGLContext()->CreateDirectCommandList();

	direct_cmd_list->Push(std::make_unique<GLSetShaderSourceCommand>(m_id, source));
	direct_cmd_list->Push(std::make_unique<GLCompileShaderCommand>(m_id));
}

unsigned int GLVertexShader::GetId() const
{
	return m_id;
}