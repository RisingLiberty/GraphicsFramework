#include "stdafx.h"

#include "Dx11CBuffer.h"

namespace
{
	const int BUFFER_SIZE_ROUND_UP = 256;

	UINT CalculateConstantBufferByteSize(UINT byteSize)
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
}

Dx11CBuffer::Dx11CBuffer(unsigned int reg, Shader::Type shaderType) :
	m_register(reg),
	m_shader_type(shaderType)
{
	
}

Dx11CBuffer::~Dx11CBuffer()
{

}

void Dx11CBuffer::PushUniform(const Dx11ShaderUniform& uniform)
{
	m_uniforms.push_back(uniform);
	m_size += uniform.size;
	m_real_size = CalculateConstantBufferByteSize(m_size);
}