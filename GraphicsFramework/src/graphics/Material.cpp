#include "stdafx.h"

#include "Material.h"

#include "ShaderProgram.h"
#include "ShaderUniform.h"

namespace
{
	MaterialParameterDataType UniformDataTypeToMaterialDataType(UniformDataType dataType)
	{
		switch (dataType)
		{
		case UniformDataType::INT:			return MaterialParameterDataType::INT;
		case UniformDataType::FLOAT:		return MaterialParameterDataType::FLOAT;
		case UniformDataType::VEC2:			return MaterialParameterDataType::VEC2;
		case UniformDataType::VEC3:			return MaterialParameterDataType::VEC3;
		case UniformDataType::VEC4:			return MaterialParameterDataType::VEC4;
		case UniformDataType::MAT3:			return MaterialParameterDataType::MAT3;
		case UniformDataType::MAT4:			return MaterialParameterDataType::MAT4;
		}

		return MaterialParameterDataType::UNSPECIFIED;
	}
}

Material::Material(ShaderProgram* program) :
	m_program(program)
{
	const std::vector<std::unique_ptr<ShaderUniform>>& uniforms = m_program->GetUniforms();
	m_parameters.reserve(uniforms.size());

	for (const std::unique_ptr<ShaderUniform>& uniform : uniforms)
	{
		m_parameters.emplace_back(uniform->name, UniformDataTypeToMaterialDataType(uniform->type));
	}
}

Material::~Material()
{

}

void Material::AddParameter(const std::string& name, MaterialParameterDataType type, void* data)
{
	this->AddParameterInternal(MaterialParameter(name, type, data));
}

void Material::AddParameterInternal(const MaterialParameter& param)
{
	auto it = std::find(m_parameters.begin(), m_parameters.end(), param);

	if (it == m_parameters.cend())
		m_parameters.push_back(param);
	else
		spdlog::warn("parameter with name '{}' already exists in material", param.name);
}

void Material::SetParameter(const std::string& name, void* data)
{
	for (MaterialParameter& parameter : m_parameters)
	{
		if (parameter.name == name)
		{
			parameter.SetData(data);
			return;
		}
	}

	spdlog::warn("material parameter not found!");
}

void Material::SetParameter(const std::string& name, void* data, size_t size)
{
	for (MaterialParameter& parameter : m_parameters)
	{
		if (parameter.name == name)
		{
			parameter.SetData(data);
			return;
		}
	}

	spdlog::warn("material parameter not found!");
}

void Material::Use()
{
	m_program->Bind();

	for (const MaterialParameter& parameter : m_parameters)
	{
		m_program->SetUniform(parameter.name, parameter.data, parameter.GetSize());
	}

	m_program->UploadVariables();
}