#pragma once

#include "graphics/ShaderProgram.h"

class Dx12ShaderProgram : public ShaderProgram
{
public:
	Dx12ShaderProgram(VertexShader* vs, FragmentShader* fs);
	virtual ~Dx12ShaderProgram();

private:
	ComPtr<ID3D12DescriptorHeap> m_constant_buffer_heap;

};