#include "stdafx.h"

#include "Shader.h"

Shader::Shader(const std::string& path):
	m_path(path)
{
}

Shader::~Shader() = default;

const std::string& Shader::GetPath() const
{
	return m_path;
}

