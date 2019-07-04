#pragma once

#include "Dx12ShaderUniform.h"
#include "Dx12ShaderStruct.h"
#include "Dx12ShaderCBuffer.h"
#include "Dx12ShaderTexture.h"
#include "Dx12ShaderSamplerState.h"

struct Dx12ParsedShader
{
public:
	Dx12ParsedShader();
	~Dx12ParsedShader();

public:
	std::vector<Dx12ShaderStruct> structs;
	std::vector<Dx12ShaderCBuffer> buffers;
	std::vector<Dx12ShaderTexture> textures;
	std::vector<Dx12ShaderSamplerState> sampler_states;
};

class Dx12ShaderParser
{
public:
	Dx12ShaderParser();
	~Dx12ShaderParser();

	Dx12ParsedShader Parse(const std::string& code);
};
