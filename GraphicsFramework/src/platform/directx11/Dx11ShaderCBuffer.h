#pragma once

#include "graphics/Shader.h"
#include "Dx11ShaderUniform.h"

struct Dx11ShaderCBuffer
{
public:
	// reg = register
	Dx11ShaderCBuffer(const std::string& name, unsigned int reg, Shader::Type shaderType, std::vector<std::unique_ptr<Dx11ShaderUniform>>& uniforms);
	Dx11ShaderCBuffer(Dx11ShaderCBuffer& buffer);
	Dx11ShaderCBuffer(Dx11ShaderCBuffer&& buffer);
	Dx11ShaderCBuffer& operator=(Dx11ShaderCBuffer& buffer);
	Dx11ShaderCBuffer& operator=(Dx11ShaderCBuffer&& buffer);
	~Dx11ShaderCBuffer();

public:
	std::string name;
	unsigned int reg;
	Shader::Type shader_type;
	std::vector<std::unique_ptr<Dx11ShaderUniform>> uniforms;
	unsigned int size;
	unsigned int real_size;
	ComPtr<ID3D11Buffer> buffer;
};