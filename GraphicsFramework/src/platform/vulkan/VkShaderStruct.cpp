#include "stdafx.h"

#include "VkShaderStruct.h"

namespace
{
	unsigned int CalculateSizeOfBuffer(const std::vector<ShaderUniform>& uniforms)
	{
		unsigned int size = 0;
		for (const ShaderUniform& uniform : uniforms)
			size += uniform.size;

		return size;
	}
}

VkShaderStruct::VkShaderStruct(const std::string& name, const std::vector<ShaderUniform>& uniforms, unsigned int offset):
	ShaderUniform(name, UniformDataType::UNSPECIFIED, CalculateSizeOfBuffer(uniforms))
{

}

VkShaderStruct::~VkShaderStruct()
{

}