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
	bool parameter_already_exists = false;
	for (const MaterialParameter& parameter : m_parameters)
	{
		if (parameter.name == name)
		{
			parameter_already_exists = true;
			break;
		}
	}

	if (parameter_already_exists)
		spdlog::warn("parameter with name '{}' already exists in material", name);
	else
		m_parameters.emplace_back(name, type, data);

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
		switch (parameter.data_type)
		{
		case MaterialParameterDataType::BOOL:			m_program->SetBoolUniform(parameter.name, *(bool*)parameter.data); 				break;
		case MaterialParameterDataType::SHORT:			m_program->SetShortUniform(parameter.name, *(short*)parameter.data);				break;
		case MaterialParameterDataType::UNSIGNED_SHORT:	m_program->SetUShortUniform(parameter.name, *(unsigned short*)parameter.data);	break;
		case MaterialParameterDataType::INT:			m_program->SetIntUniform(parameter.name, *(int*)parameter.data);					break;
		case MaterialParameterDataType::UNSIGNED_INT:	m_program->SetUIntUniform(parameter.name, *(unsigned int*)parameter.data);		break;
		case MaterialParameterDataType::FLOAT:			m_program->SetFloatUniform(parameter.name, *(float*)parameter.data);				break;
		case MaterialParameterDataType::DOUBLE:			m_program->SetDoubleUniform(parameter.name, *(double*)parameter.data);			break;
		case MaterialParameterDataType::VEC2:			m_program->SetVec2Uniform(parameter.name, (float*)parameter.data);				break;
		case MaterialParameterDataType::VEC3:			m_program->SetVec3Uniform(parameter.name, (float*)parameter.data);				break;
		case MaterialParameterDataType::VEC4:			m_program->SetVec4Uniform(parameter.name, (float*)parameter.data);				break;
		case MaterialParameterDataType::MAT3:			m_program->SetMat3Uniform(parameter.name, (float*)parameter.data);				break;
		case MaterialParameterDataType::MAT4:			m_program->SetMat4Uniform(parameter.name, (float*)parameter.data);				break;
		}
	}
}