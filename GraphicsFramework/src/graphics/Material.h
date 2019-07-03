#pragma once

#include "MaterialParameter.h"

class ShaderProgram;

class Material
{
public:
	Material(ShaderProgram* program);
	~Material();

	void Use();

	void AddParameter(const std::string& name, MaterialParameterDataType type, void* data = nullptr);
	void SetParameter(const std::string& name, void* data);
	void SetParameter(const std::string& name, void* data, size_t size);

private:
	ShaderProgram* m_program;
	std::vector<MaterialParameter> m_parameters;
	
};