#pragma once

#include "Dx11ShaderCBuffer.h"
#include "Dx11ShaderStruct.h"
#include "Dx11ShaderTexture.h"
#include "Dx11ShaderSamplerState.h"

struct Dx11ParsedShader
{
	Dx11ParsedShader();
	~Dx11ParsedShader();

	std::vector<Dx11ShaderStruct> structs;
	std::vector<Dx11ShaderCBuffer> buffers;
	std::vector<Dx11ShaderTexture> textures;
	std::vector<Dx11ShaderSamplerState> sampler_states;
};

class Dx11ShaderParser
{
public:
	Dx11ShaderParser();
	~Dx11ShaderParser();

	Dx11ParsedShader Parse(const std::string& code);
};