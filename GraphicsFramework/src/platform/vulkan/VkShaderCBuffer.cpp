#include "stdafx.h"

#include "VkShaderCBuffer.h"

namespace
{
	unsigned int CalculateConstantBufferSize(const std::vector<std::unique_ptr<ShaderUniform>>& uniforms)
	{
		unsigned int size = 0;
		for (const std::unique_ptr<ShaderUniform>& uniform : uniforms)
			size += uniform->size;

		return size;
	}
}

VkShaderCBuffer::VkShaderCBuffer(const std::string& name, unsigned int boundLocation, std::vector<std::unique_ptr<ShaderUniform>>& uniforms):
	name(name),
	binding(boundLocation),
	uniforms(std::move(uniforms))
{
	if (this->uniforms.empty())
		size = 0;
	else
		size = CalculateConstantBufferSize(this->uniforms);
}

VkShaderCBuffer::~VkShaderCBuffer()
{

}