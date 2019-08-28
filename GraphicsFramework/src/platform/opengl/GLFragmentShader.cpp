#include "stdafx.h"

#include "GLFragmentShader.h"
#include "GLHelperMethods.h"

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

GLFragmentShader::GLFragmentShader(const std::string& path):
	FragmentShader(path)
{
	this->Compile();
}

GLFragmentShader::~GLFragmentShader()
{
	GetGLCommandList()->DeleteShader(m_id);
}

int GLFragmentShader::Compile()
{
	m_id = GetGLCommandList()->CreateShader(GL_FRAGMENT_SHADER);
	std::string source = LoadCode(m_path);
	const char* code = source.c_str();

	GetGLCommandList()->SetShaderSource(m_id, code);
	GetGLCommandList()->CompileShader(m_id);

	std::string info_log = GetGLCommandList()->GetShaderInfoLog(m_id);
	if (!info_log.empty())
		spdlog::error(info_log);

	return !info_log.empty();
}

unsigned int GLFragmentShader::GetId() const
{
	return m_id;
}