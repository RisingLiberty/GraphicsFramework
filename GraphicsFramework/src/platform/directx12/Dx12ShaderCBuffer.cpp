#include "stdafx.h"

#include "Dx12ShaderCBuffer.h"
#include "Dx12Context.h"
#include "Dx12HelperMethods.h"

namespace
{
	const int BUFFER_SIZE_ROUND_UP = 256;

	unsigned int CalculateConstantBufferByteSize(unsigned int byteSize)
	{
		// Constant buffers must be a multiple of the minimum hardware
		// allocation size (usually 256 bytes).  So round up to nearest
		// multiple of 256.  We do this by adding 255 and then masking off
		// the lower 2 bytes which store all bits < 256.
		// Example: Suppose byteSize = 300 == 0x012c.
		// (300 + 255) & ~255
		// 555 & ~255
		// 0x022B & ~0x00ff
		// 0x022B & 0xff00
		// 0x0200
		// 512
		return (byteSize + (BUFFER_SIZE_ROUND_UP - 1)) & ~(BUFFER_SIZE_ROUND_UP - 1);
	}

	unsigned int CalculateBufferSize(const std::vector<std::unique_ptr<Dx12ShaderUniform>>& uniforms)
	{
		return uniforms.back()->offset + uniforms.back()->size;
	}
}

Dx12ShaderCBuffer::Dx12ShaderCBuffer(const std::string& name, unsigned int reg, std::vector<std::unique_ptr<Dx12ShaderUniform>>& uniforms) :
	name(name),
	reg(reg),
	uniforms(std::move(uniforms))
{
	if (this->uniforms.empty())
		size = 0;
	else
	{
		size = CalculateBufferSize(this->uniforms);
		real_size = CalculateConstantBufferByteSize(size);
	}

}

Dx12ShaderCBuffer::Dx12ShaderCBuffer(Dx12ShaderCBuffer& buffer)
{
	name = buffer.name;
	reg = buffer.reg;
	shader_type = buffer.shader_type;
	uniforms = std::move(buffer.uniforms);
	size = buffer.size;
	real_size = buffer.real_size;

	this->buffer = buffer.buffer;

}

Dx12ShaderCBuffer::Dx12ShaderCBuffer(Dx12ShaderCBuffer&& buffer)
{
	name = buffer.name;
	reg = buffer.reg;
	shader_type = buffer.shader_type;
	uniforms = std::move(buffer.uniforms);
	size = buffer.size;
	real_size = buffer.real_size;

	this->buffer = buffer.buffer;
}

Dx12ShaderCBuffer& Dx12ShaderCBuffer::operator=(Dx12ShaderCBuffer& buffer)
{
	name = buffer.name;
	reg = buffer.reg;
	shader_type = buffer.shader_type;
	uniforms = std::move(buffer.uniforms);
	size = buffer.size;
	real_size = buffer.real_size;

	this->buffer = buffer.buffer;

	return *this;
}

Dx12ShaderCBuffer& Dx12ShaderCBuffer::operator=(Dx12ShaderCBuffer&& buffer)
{
	name = buffer.name;
	reg = buffer.reg;
	shader_type = buffer.shader_type;
	uniforms = std::move(buffer.uniforms);
	size = buffer.size;
	real_size = buffer.real_size;

	this->buffer = buffer.buffer;

	return *this;
}

Dx12ShaderCBuffer::~Dx12ShaderCBuffer() = default;
