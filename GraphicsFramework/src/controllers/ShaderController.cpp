#include "stdafx.h"

#include "ShaderController.h"

#include "graphics/VertexShader.h"
#include "graphics/FragmentShader.h"
#include <graphics/ShaderProgram.h>

namespace
{
	size_t ShaderProgramHash(VertexShader* vs, FragmentShader* fs)
	{
		size_t a = (size_t)vs;
		size_t b = (size_t)fs;

		//Cantor pairing function
		return (a + b) * (a + b + 1) / 2 + a;
	}

	size_t ShaderProgramHash(ShaderProgram* program)
	{
		return ShaderProgramHash(program->GetVertexShader(), program->GetFragmentShader());
	}
}

ShaderController::ShaderController() = default;

ShaderController::~ShaderController() = default;

void ShaderController::PushVertexShader(std::unique_ptr<VertexShader>& vertexShader)
{
	m_vertex_shaders[vertexShader->GetPath()] = std::move(vertexShader);
}

void ShaderController::PushFragmentShader(std::unique_ptr<FragmentShader>& fragmentShader)
{
	m_fragment_shaders[fragmentShader->GetPath()] = std::move(fragmentShader);
}

void ShaderController::PushShaderProgram(std::unique_ptr<ShaderProgram>& shaderProgram)
{
	m_shader_programs[ShaderProgramHash(shaderProgram.get())] = std::move(shaderProgram);
}

VertexShader* ShaderController::GetVertexShader(const std::string& path) const
{
	auto it = m_vertex_shaders.find(path);

	if (it != m_vertex_shaders.cend())
		return it->second.get();
	else
		return nullptr;
}

FragmentShader* ShaderController::GetFragmentShader(const std::string& path) const
{
	auto it = m_fragment_shaders.find(path);

	if (it != m_fragment_shaders.cend())
		return it->second.get();
	else
		return nullptr;
}

ShaderProgram* ShaderController::GetShaderProgram(VertexShader* vs, FragmentShader* fs) const
{
	auto it = m_shader_programs.find(ShaderProgramHash(vs, fs));

	if (it != m_shader_programs.cend())
		return it->second.get();
	else
		return nullptr;
	return m_shader_programs.at(ShaderProgramHash(vs, fs)).get();
}