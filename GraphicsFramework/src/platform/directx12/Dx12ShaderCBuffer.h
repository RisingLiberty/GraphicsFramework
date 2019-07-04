#pragma once

#include "graphics/Shader.h"
#include "Dx12ShaderUniform.h"

struct Dx12ShaderCBuffer
{
public:
	// reg = register
	Dx12ShaderCBuffer(const std::string& name, unsigned int reg, std::vector<std::unique_ptr<Dx12ShaderUniform>>& uniforms);
	Dx12ShaderCBuffer(Dx12ShaderCBuffer& buffer);
	Dx12ShaderCBuffer(Dx12ShaderCBuffer&& buffer);
	Dx12ShaderCBuffer& operator=(Dx12ShaderCBuffer& buffer);
	Dx12ShaderCBuffer& operator=(Dx12ShaderCBuffer&& buffer);
	~Dx12ShaderCBuffer();

public:
	std::string name;
	unsigned int reg;
	Shader::Type shader_type;
	std::vector<std::unique_ptr<Dx12ShaderUniform>> uniforms;
	unsigned int size;
	unsigned int real_size;
	ComPtr<ID3D11Buffer> buffer;
};