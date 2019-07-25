#pragma once

#include "VkShaderStruct.h"
#include "VkShaderCBuffer.h"

struct VkParsedShader
{
public:
	VkParsedShader();
	~VkParsedShader();

public:
	std::vector<VkShaderStruct> structs;
	std::vector<VkShaderCBuffer> buffers;
};

class VkShaderParser
{
public:
	VkShaderParser();
	~VkShaderParser();

	void Parse(const std::string& path);
};